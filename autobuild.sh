#!/bin/bash

set -e

rm -rf `pwd`/build/*
cd `pwd`/build &&
	cmake . -DCMAKE_CXX_STANDARD=17 ..&&
	cmake .. &&
	make
cd ..
cp -r `pwd`/include `pwd`/lib