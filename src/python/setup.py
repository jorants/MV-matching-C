from distutils.core import setup, Extension
 
module1 = Extension('mv', sources = ['mvmodule.c'],include_dirs = ['../../bin'],library_dirs = ['../../bin'],libraries = ['mv'])
 
setup (name = 'MV-Matching',
        version = '1.0',
        description = 'This library is a python binding of the C implementation of the MV-matching algorithm',
        ext_modules = [module1])
