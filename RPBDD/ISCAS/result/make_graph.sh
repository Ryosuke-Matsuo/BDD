#!/bin/tcsh

set Cid = $<
set lut = $<

g++ plt_make_reverse_func-class.cpp

./a.out <<file
${Cid}
${lut}
file

gnuplot reverse_func-class_make.plt