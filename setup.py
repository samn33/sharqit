# -*- coding: utf-8 -*-
import os
import pathlib
from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig

_VERSION = '0.2.1'

class CMakeExtension(Extension):

    def __init__(self, name):
        super().__init__(name, sources=[])

class build_ext(build_ext_orig):

    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()
        build_temp = pathlib.Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = pathlib.Path(self.get_ext_fullpath(ext.name))
        extdir.mkdir(parents=True, exist_ok=True)
        config = 'Debug' if self.debug else 'Release'
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + str(extdir.parent.absolute()),
        ]
        build_args = [
            '--config', config,
            '--', '-j4'
        ]
        os.chdir(str(build_temp))
        self.spawn(['cmake', str(cwd)] + cmake_args)
        if not self.dry_run:
            self.spawn(['cmake', '--build', '.'] + build_args)
        os.chdir(str(cwd))

setup(
    name='sharqit',
    version=_VERSION,
    author='Sam.N',
    author_email='saminriver33@gmail.com',
    description='Quantum Circuit Optimizer',
    long_description='',
    packages=find_packages(),
    include_package_data=True,
    install_requires=[
        'nanobind>=1.9.2',
    ],
    license='MIT',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Programming Language :: Python',
        'License :: OSI Approved :: MIT License',
    ],
    keywords=['Quantum Computer', 'Quantum Circit', 'Optimization'],
    ext_modules=[CMakeExtension('sharqit/sharqit_module')],
    cmdclass={
        'build_ext': build_ext,
    },
)
