import distutils.command.build as _build
import os
import sys
from distutils import spawn
from distutils.sysconfig import get_python_lib

from setuptools import setup


def extend_build():
    class build(_build.build):
        def run(self):
            cwd = os.getcwd()
            if spawn.find_executable('cmake') is None:
                sys.stderr.write("CMake is required to build this package.\n")
                sys.exit(-1)
            _source_dir = os.path.join(os.path.split(__file__)[0], _this_package)
            _build_dir = os.path.join(_source_dir, 'build')
            _prefix = get_python_lib()
            try:
                cmake_configure_command = [
                    'cmake',
                    '-H{0}'.format(_source_dir),
                    '-B{0}'.format(_build_dir),
                    '-DCMAKE_INSTALL_PREFIX={0}'.format(_prefix),
                ]
                _generator = os.getenv('CMAKE_GENERATOR')
                if _generator is not None:
                    cmake_configure_command.append('-G{0}'.format(_generator))
                spawn.spawn(cmake_configure_command)
                spawn.spawn(
                    ['cmake', '--build', _build_dir, '--target', 'install'])
                os.chdir(cwd)
            except spawn.DistutilsExecError:
                sys.stderr.write("Error while building with CMake\n")
                sys.exit(-1)
            _build.build.run(self)

    return build


_here = os.path.abspath(os.path.dirname(__file__))

_this_package = 'Engine'

setup(
    name="PyCSPillEngine",
    version="0.0.1b",
    description='PyCSPill Engine',
    long_description="CSPill Engine with Python Script System built-in",
    author='Jeromy Zhang',
    author_email='zjdx1998@gmail.com',
    url='https://cspill.jeromy.site',
    license='MIT',
    packages=[_this_package],
    include_package_data=True,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Science/Research',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11'
    ],
    cmdclass={'build': extend_build()})
