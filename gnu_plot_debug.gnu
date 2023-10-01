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

set y2label "Sinusoidal Amplitude (mV)"
set ytics nomirror
set y2tics
set tics out
set autoscale  x
set yrange [-100:1100]
set y2range [-1200:1200]
# set autoscale y2
# set xrange [-5:35]
# set xtics (-5,0,5,10,15,20,25,30,35)
# set yrange [-10:2]
# set ytics (-10,-8,-6,-4,-2,0,2)
set grid x;
set grid y;
set grid y2;
set key left top default box opaque;
set border back
set title plot_title;
plot input_filename u ($1/1e-09):($4/1e-03) axes x1y1 title columnhead(4) with lines noenhanced, input_filename u ($1/1e-09):($6/1e-03) axes x1y1 title columnhead(6) with lines noenhanced lw 2, input_filename u ($1/1e-09):($5/1e-03) axes x1y2 title columnhead(5) with lines noenhanced;
pause(3);
set terminal push
set terminal pngcairo size 1200,600 # default
set output sprintf("sq_%s.png",timestamp);
replot
set terminal pop

