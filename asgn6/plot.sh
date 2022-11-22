#!/bin/bash

make clean && make banhammer

for i in {100000..600000..1000};
do
    echo -n "$i "
	cat test.txt | ./banhammer -s -f $i | awk '{print $(NF)}' | tr '\n' ' '
    echo ""
done > /tmp/bfsize.dat

for i in {5000..20000..100};
do
    echo -n "$i "
	cat test.txt | ./banhammer -s -t $i | awk '{print $(NF)}' | tr '\n' ' '
    echo ""
done > /tmp/htsize.dat


gnuplot <<END
	set terminal pdf
	set output "plots.pdf"
	set zeroaxis
    set title "Bits examined per miss on different Bloom filter sizes"
	set xlabel "BF size"
	set ylabel "Bits examined"
    plot "/tmp/bfsize.dat" using 1:10 with lines title ""
    set title "Hash table lookups on different Bloom filter sizes"
	set xlabel "BF size"
	set ylabel "HT lookups"
    plot "/tmp/bfsize.dat" using 1:(column(3)+column(4)) with lines title ""
    set title "links examined on different hash table sizes"
	set xlabel "HT size"
	set ylabel "links examined"
    plot "/tmp/htsize.dat" using 1:5 with lines title ""
END
