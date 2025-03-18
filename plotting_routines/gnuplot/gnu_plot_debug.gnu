
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

y_min = STATS_min_y;
y_max = STATS_max_y;

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
# print sprintf("x_min = %.6e, x_max = %.6e\n",x_min,x_max);
xtick_increment = (x_max - x_min)/max_num_ticks;
# print sprintf("max_num_ticks = %d, base = %.1f, xtick_increment = %.6e,",max_num_ticks,base,xtick_increment);
xtick_increment = base**(floor(log10(xtick_increment)/log10(base) + 0.50));
# print sprintf("xtick_increment = %.6e\n",xtick_increment);

x_limit_min = xtick_increment * floor(x_min/xtick_increment + 0.50);
x_limit_max = xtick_increment * ceil(x_max/xtick_increment - 0.50);
# print sprintf("x_limit_min = %.6e, x_limit_max = %.6e\n",x_limit_min,x_limit_max);

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
# print sprintf("x_limit_min = %.6e, x_limit_max = %.6e\n",x_limit_min,x_limit_max);

max_num_ticks = 8;
base = 10.0;
ytick_increment = (y_max - y_min)/max_num_ticks;
# print sprintf("y_min = %.3f, y_max = %.3f\n",y_min,y_max);
# print sprintf("max_num_ticks = %d, base = %.1f, ytick_increment = %.3f,",max_num_ticks,base,ytick_increment);
ytick_increment = base**(floor(log10(ytick_increment)/log10(base) + 0.50));

# print sprintf("ytick_increment = %.3f\n",ytick_increment);

y_limit_min = ytick_increment * (floor(y_min/ytick_increment) - 1.0);
y_limit_max = ytick_increment * (4.0 + ceil(y_max/ytick_increment)); # Allow added height for legend

# print sprintf("y_limit_min = %.3f, y_limit_max = %.3f\n",y_limit_min,y_limit_max);

if ((y_limit_max - y_limit_min)/ytick_increment < max_num_ticks) {
set ytics y_limit_min,ytick_increment,y_limit_max;
} else {
	if ((y_limit_max - y_limit_min)/ytick_increment >= 2*max_num_ticks) {
		y_limit_max = y_limit_min + ceil((y_limit_max - y_limit_min)/(4*ytick_increment))*(4*ytick_increment);
		set ytics y_limit_min,4*ytick_increment,y_limit_max;
		set mytics 2;
	} else {
		set ytics y_limit_min,2*ytick_increment,y_limit_max;
		set mytics 2;
	}	
set grid mytics;
}
set yrange [y_limit_min:y_limit_max];
# print sprintf("y_limit_min = %.3f, y_limit_max = %.3f\n",y_limit_min,y_limit_max);

set y2range [-1200:1600];
set grid x lw 1.5;
set grid y lw 1.5;
set grid xtics;
set grid mxtics;
set grid ytics;

if ((x_limit_max - x_limit_min > 1) && (x_limit_max - x_limit_min < 10000)) {
num_digits = floor(log10(abs(x_limit_max)/xtick_increment)) - 1;
format_specifier = sprintf("%%.%df",num_digits);
set format x format_specifier;
} else {
num_digits = floor(log10(abs(x_limit_max)/xtick_increment));
format_specifier = sprintf("%%g");
set format x format_specifier;
}

if ((y_limit_max - y_limit_min > 1) && (y_limit_max - y_limit_min < 10000)) {
num_digits = floor(log10(abs(y_limit_max)/ytick_increment)) - 1;
format_specifier = sprintf("%%.%df",num_digits);
set format y format_specifier;
} else {
num_digits = floor(log10(abs(y_limit_max)/ytick_increment));
format_specifier = sprintf("%%.%de",num_digits);
set format y format_specifier;
}

set key center top default opaque;
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

