#!/bin/sh

rm -rf build
python2 setup.py build
cp test.py build/lib*/
cd build/lib*
python2 test.py
cd ../..
