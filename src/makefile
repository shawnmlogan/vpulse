IDIR =../include
CC=gcc
LIBS=-lm
CFLAGS=-I$(IDIR)

ODIR=obj

_DEPS = vpulse.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = add_units.o add_units_underscore.o append_filename_keep_N_characters.o check_executable.o check_inputs.o f3.o find_date.o find_modulation_type.o find_noise_location.o find_noise_type.o find_timestamp.o main.o mean.o pnoise_type.o random_gaussian_clocknoise.o random_uniform_clocknoise.o remove_carriage_return.o rkstep1.o sort_array_by_column_number.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

SH_RESULT:=$(shell set_install_dir.sh)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

vpulse: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

SH_RESULT:=$(shell install_symbolic_link.sh)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
#rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

