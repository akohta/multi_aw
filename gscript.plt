# Gnuplot script file
set terminal postscript eps color enhanced "Arial" 20 size 7in,14in
set output "I_example2.eps"

set pm3d map

set size square
set multiplot layout 4,2

set title "intensity of sound pressure on y=0 plane"
set xlabel "{/Arial-Italic x}"
set ylabel "{/Arial-Italic z}"
splot "Ip_xz.txt"

set title "intensity of sound pressure on x=0 plane"
set xlabel "{/Arial-Italic y}"
splot "Ip_yz.txt"

set title "intensity of sound pressure on z=0 plane"
set xlabel "{/Arial-Italic x}"
set ylabel "{/Arial-Italic y}"
splot "Ip_xy.txt"

clear

set title "intensity of particle velocity on y=0 plane"
set xlabel "{/Arial-Italic x}"
set ylabel "{/Arial-Italic z}"
splot "Iv_xz.txt"

set title "intensity of particle velocity on x=0 plane"
set xlabel "{/Arial-Italic y}"
splot "Iv_yz.txt"

set title "intensity of particle velocity on z=0 plane"
set xlabel "{/Arial-Italic x}"
set ylabel "{/Arial-Italic y}"
splot "Iv_xy.txt"

unset multiplot
set terminal x11
