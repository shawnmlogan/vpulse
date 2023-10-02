clear
set datafile separator ","

input_filename = ARGV[1];
plot_title = ARGV[2];
timestamp = ARGV[3];

# input_filename = sprintf("square_wave_071523_22_26_09.csv");
# plot_title = sprintf("My title");

set key autotitle columnhead
set xlabel 'Time (ns)';
set ylabel 'Square Wave Amplitude (mV)';

set tics out
set autoscale  x
set yrange [-100:1100]

set grid x;
set grid y;
set key left top default box opaque;
set border back
set title plot_title;
plot input_filename u ($1/1e-09):($3/1e-03) axes x1y1 title columnhead(3) with lines lw 2 noenhanced
pause(3);
set terminal push
set terminal pngcairo size 1200,600 # default
set output sprintf("sq_%s.png",timestamp);
replot
set terminal pop

