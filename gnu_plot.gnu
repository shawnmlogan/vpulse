
set datafile separator ",";
set terminal qt size 1200,600 font "Verdana,14";
set termopt enhanced;

# base_filename = ARGV[1];
# plot_title = ARGV[2];
# timestamp = ARGV[3];


input_filename = sprintf("./.%s.csv",base_filename);
output_filename = sprintf("%s.png",base_filename);

stats input_filename u 1:($2/1e-03) skip 1 nooutput;

x_min = STATS_min_x;
x_max = STATS_max_x;

set title plot_title;

# Offset title using graph coordinates to better fit page (range 0.0 to 1.0)
set title offset graph -0.02,0.0; 

set xlabel 'Time (s)';
set ylabel 'Square Wave Amplitude (mV)';

max_num_ticks = 10;
base = 10.0;
xtick_increment = (x_max - x_min)/max_num_ticks;
xtick_increment = base**(floor(log10(xtick_increment)/log10(base) + 0.50));

x_limit_min = xtick_increment * (floor(x_min/xtick_increment + 0.50) - 0.00);
x_limit_max = xtick_increment * (0.0 + ceil(x_max/xtick_increment + 0.00));

set xrange [x_limit_min:x_limit_max];
set xtics x_limit_min,xtick_increment,x_limit_max;

set yrange [-200:1200];
set ytics -200,200,1200;

set grid x lw 1.5;
set grid y lw 1.5;
set grid xtics;
set grid ytics;
set grid mxtics;
set format x "%.2e";
set format y "%.0f";
set key center top default opaque;
set border back;
set rmargin at screen 0.95;

plot input_filename u 1:($2/1e-03) title columnhead(2) with lines lw 2

pause(3);
set terminal push;
set terminal pngcairo size 1200,600 font "Verdana,14";
set termopt enhanced;
set key center default opaque;

set output output_filename;
replot;
set terminal pop

