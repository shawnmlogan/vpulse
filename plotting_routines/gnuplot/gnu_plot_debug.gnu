
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

set y2label "Sinusoidal Amplitude (mV)";
set ytics nomirror;
set y2tics;

max_num_ticks = 10;
base = 10.0;
xtick_increment = (x_max - x_min)/max_num_ticks;
xtick_increment = base**(floor(log10(xtick_increment)/log10(base) + 0.50));

x_limit_min = xtick_increment * floor(x_min/xtick_increment);
x_limit_max = xtick_increment * ceil(x_max/xtick_increment);

if ((x_limit_max - x_limit_min)/xtick_increment < max_num_ticks) {
set xtics x_limit_min,xtick_increment,x_limit_max;
} else {
	if ((x_limit_max - x_limit_min)/xtick_increment >= 2*max_num_ticks) {
		x_limit_max = x_limit_min + ceil((x_limit_max - x_limit_min)/(4*xtick_increment))*(4*xtick_increment);
		set xtics x_limit_min,4*xtick_increment,x_limit_max;
		set mxtics 2;
	} else {
		set xtics x_limit_min,2*xtick_increment,x_limit_max;
		set mxtics 2;
	}	
}
set xrange [x_limit_min:x_limit_max];

set yrange [-200:1200];
set ytics -200,200,1200;

set y2range [-1200:1200];
set grid x lw 1.5;
set grid y lw 1.5;
set grid xtics;
set grid mxtics;
set grid ytics;
set format x "%g";
set format y "%.0f";
set grid y2;
set key left top default box opaque;
set border back;
set rmargin at screen 0.90;

plot input_filename u 1:($6/1e-03) axes x1y1 title columnhead(6) with lines noenhanced, input_filename u 1:($5/1e-03) axes x1y1 title columnhead(5) with lines noenhanced lw 2, input_filename u 1:($2/1e-03) axes x1y2 title columnhead(2) with lines noenhanced;
pause(3);
set terminal push;
set terminal pngcairo size 1200,600 font "Verdana,14";
set termopt enhanced;
set key center default opaque;

set output output_filename;
replot;
set terminal pop;

