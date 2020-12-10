#!/bin/tcsh

set i = 1
set subcircuit = $<

g++ reverse_sharing.cpp

@ i = 1
while ( $i < $subcircuit )
./a.out <<file
${i}
file
@ i++
end

g++ compare_reverse.cpp
./a.out