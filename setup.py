#!/usr/bin/env python

from distutils.core import setup, Extension
from Cython.Distutils import build_ext


module_fgen = Extension('fgen', sources = ['fgen.c'])

setup(
    name='fgen',
    version='1.0',
    description='Generator of complex multidimentional multiextremal functions',
    author='Vytautas Jancauskas and Albertas Gimbutas',
    author_email='vytautas.jancauskas@mif.vu.lt, albertas.gimbutas@mii.vu.lt',
    url='https://github.com/strazdas/fgen',
    cmdclass = {'build_ext' : build_ext},
    license="GPL",
    ext_modules=[module_fgen]
)
