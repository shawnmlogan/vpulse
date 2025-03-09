
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

max_num_ticks = 8;
base = 10.0;
ytick_increment = (y_max - y_min)/max_num_ticks;
ytick_increment = base**(floor(log10(ytick_increment)/log10(base) + 0.50));

y_limit_min = ytick_increment * (floor(y_min/ytick_increment) - 1.0);
y_limit_max = ytick_increment * (1.0 + ceil(y_max/ytick_increment)); # Allow added height for legend

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

set grid x lw 1.5;
set grid y lw 1.5;
set grid xtics;
set grid mxtics;
set grid ytics;

if ((x_limit_max - x_limit_min > 1) && (x_limit_max - x_limit_min < 10000)) {
num_digits = floor(log10(abs(x_limit_max)/xtick_increment));
format_specifier = sprintf("%%.%df",num_digits);
set format x format_specifier;
} else {
num_digits = floor(log10(abs(x_limit_max)/xtick_increment));
format_specifier = sprintf("%%g");
set format x format_specifier;
}

if ((y_limit_max - y_limit_min > 1) && (y_limit_max - y_limit_min < 10000)) {
num_digits = floor(log10(abs(y_limit_max)/ytick_increment));
format_specifier = sprintf("%%.%df",num_digits);
set format y format_specifier;
} else {
num_digits = floor(log10(abs(y_limit_max)/ytick_increment));
format_specifier = sprintf("%%.%de",num_digits);
set format y format_specifier;
}

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

