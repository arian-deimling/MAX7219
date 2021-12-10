from invoke import task
import os
import glob

targets = ['matrix', 'terminal',]

library_source_dir = './libraries/'

library_output_dir = './external/'
library_header_dir = f'{library_output_dir}include/'
library_files_dir = f'{library_output_dir}lib/'

# generate a list of names of all of the external libraries 
libraries = [p.replace(library_source_dir, '')[:-1]
             for p in glob.glob(f'{library_source_dir}*/')]

project_source_dir = './src/'
project_header_dir = './include/'
project_build_dir = './build/'

optimization = 'O0'

CC = 'gcc'
CCFLAGS = f'-Wall -Wextra -Wpedantic -g -{optimization}'
CXX = 'g++'
CXXFLAGS = f'{CCFLAGS} --std=c++17'
LDFLAGS = f'-L{library_files_dir} ' + ' '.join([f'-l{lib}' for lib in libraries])
AR = 'ar'
AROPTS = 'rcs'


@task
def clean(c):

    # delete all object files generated from the build process
    c.run(f'rm -rf {project_build_dir}')

    # delete all output executable files
    c.run(f'rm -rf {" ".join(targets)}')

@task
def clean_libraries(c):

    # delete all files generated from building external libraries
    c.run(f'rm -rf {library_output_dir}')

    # remove object files generated when building the libraries
    for library_name in libraries:
        c.run(f'rm -rf {library_source_dir}{library_name}/build/')

@task
def clean_docs(c):
    c.run('rm -rf html latex')

@task(pre=[clean, clean_libraries, clean_docs])
def clean_all(c):
    pass

@task
def build_docs(c):
    c.run('doxygen config')

@task
def build_directories(c):

    # create a directory to hold external libraries
    c.run('mkdir -p external/include external/lib')

    # create a directory to hold object files from build
    c.run('mkdir -p build')

    # create a directory to hold library source files compiled to object files
    for library_name in libraries:
        c.run(f'mkdir -p {library_source_dir}{library_name}/build/')

@task(pre=[build_directories])
def build_libraries(c, library=None):

    # this target can be used to build a single library or all libraries if no
    # specific library name is given
    if library is None:
        libs_to_build = libraries
    else:
        if library not in libraries:
            raise FileNotFoundError(f'{library} could not be located.')
        libs_to_build = [library]

    # for each library that needs to be built
    for library_name in libs_to_build:

        header_dir = f'{library_source_dir}{library_name}/include/'
        source_dir = f'{library_source_dir}{library_name}/src/'
        object_dir = f'{library_source_dir}{library_name}/build/'

        # get paths for all of the header files for the library
        headers = glob.glob(f'{header_dir}*')

        for header_file in headers:

            # copy header files from the library source to the external library
            # include directory
            source_file = header_file
            destination_file = header_file.replace(
                f'{header_dir}',
                f'{library_header_dir}'
            )
            c.run(f'cp {source_file} {destination_file}')

        # get paths for all of the source files for the library
        sources = glob.glob(f'{source_dir}*')

        for source_code_file in sources:

            file_ext = os.path.splitext(source_code_file)[-1]
            is_c_file = file_ext == '.c'

            # compile each source code file to a corresponding object file
            output_file = (source_code_file
                            .replace('/src/', '/build/')
                            .replace('.cc', '.o')
                            .replace('.cpp', '.o')
                            .replace('.c', '.o'))

            if is_c_file:
                c.run(
                    f'{CC} {CCFLAGS} -I{header_dir} -c -o {output_file} {source_code_file}'
                )
            
            else:
                c.run(
                    f'{CXX} {CXXFLAGS} -I{header_dir} -c -o {output_file} {source_code_file}'
                )

        object_files = glob.glob(f'{object_dir}*')

        c.run(
            f'{AR} {AROPTS} {library_files_dir}lib{library_name}.a '
            f'{" ".join(object_files)}'
        )

@task(pre=[build_directories, build_libraries])
def build(c):

    # generate a list of source code files for the project
    source_files = glob.glob(f'{project_source_dir}*')

    for source_code_file in source_files:
    
        output_file = (source_code_file
                        .replace('/src/', '/build/')
                        .replace('.cc', '.o')
                        .replace('.cpp', '.o')
                        .replace('.c', '.o'))

        c.run(
            f'{CXX} {CXXFLAGS} -I{project_header_dir} -I{library_header_dir} '
            f'-c -o {output_file} {source_code_file}'
        )

    target_objects = [f'{project_build_dir}{target_file}.o' 
                      for target_file in targets]
    non_target_objects = [object_file for object_file in 
                          glob.glob(f'{project_build_dir}*') 
                          if object_file not in target_objects]

    for target_output in targets:

        c.run(
            f'{CXX} {CXXFLAGS} -I{project_header_dir} -I{library_header_dir} '
            f' -o {target_output} {" ".join(non_target_objects)} '
            f'{project_build_dir}{target_output}.o {LDFLAGS}'
        )
