#!/bin/tcsh

echo "Circuit number"
set name = $<

echo "The number of LUTs"
set lut = $<

echo "The number of sub-circuit"
set last = $<
@ last++

set i = 1;

g++ adjacent_swap.cpp -mcmodel=large

echo "Sub-circuit number : ${i}" > result.txt
echo "subcircuit/iscas_c${name}_lut${lut}_${i}.blif" | ./a.out >> result.txt
printf "\n" >> result.txt
@ i++

while ( ${i} < ${last} )
echo "Sub-circuit number : ${i}" >> result.txt
echo "${i}"

echo "subcircuit/iscas_c${name}_lut${lut}_${i}.blif" | ./a.out >> result.txt
printf "\n" >> result.txt
@ i++
end