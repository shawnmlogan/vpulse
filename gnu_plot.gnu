
set datafile separator ",";
set terminal qt size 1250,600 font "Verdana,14";
set termopt enhanced;

# input_filename = ARGV[1];
# plot_title = ARGV[2];
# timestamp = ARGV[3];

# input_filename = sprintf("square_wave_071523_22_26_09.csv");
# plot_title = sprintf("My title");

set title plot_title;

# Offset title using graph coordinates to better fit page (range 0.0 to 1.0)
set title offset graph -0.02,0.0; 

set xlabel 'Time (ns)';
set ylabel 'Square Wave Amplitude (mV)';

set tics out
set autoscale x
set yrange [-100:1200]

set grid x;
set grid y;
set grid xtics;
set grid ytics;
set grid mxtics;
set format x "%.1f"
set format y "%.0f"
set key center top default opaque;
set border back

plot input_filename u ($1/1e-09):($3/1e-03) title columnhead(3) with lines lw 2
pause(3);
set terminal push
set terminal pngcairo size 1250,600 font "Verdana,14";
set termopt enhanced;
set key center top default box opaque;
set key width -8
set output sprintf("sq_%s.png",timestamp);
replot
set terminal pop

