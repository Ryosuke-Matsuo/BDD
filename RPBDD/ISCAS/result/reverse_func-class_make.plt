set terminal png
set output 'graph/reverse_compare_function.png'
set xl "function"
set yl "Power [uW]"
set key left top
plot "reverse/c7552_lut10_function_class.txt" using 1:3 with points pointsize 1 title "Normal", "reverse/c7552_lut10_function_class.txt" using 1:5 with points pointsize 1 title "Reverse"
set terminal postscript enhanced color
set output 'graph/reverse_compare_function.eps'
set size 0.6,0.6
set xl "function"
set yl "Power [uW]"
set key left top
plot "reverse/c7552_lut10_function_class.txt" using 1:3 with points pointsize 1 title "Normal", "reverse/c7552_lut10_function_class.txt" using 1:5 with points pointsize 1 title "Reverse"
