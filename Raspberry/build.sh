#!/bin/bash

rm -rf build/*
cd build
qmake ../RaspMx.pro
make

