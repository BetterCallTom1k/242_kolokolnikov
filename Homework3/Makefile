CC     = gcc
CFLAGS = -Wall

.PHONY: all run clean

all: program interpose_runt.so

# ---------------------------------------------------------------
# Static library: fred + john  (use -lm at link time for math)
# ---------------------------------------------------------------
fred.o: fred.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

john.o: john.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

libstatic.a: fred.o john.o
	ar crv $@ $^

# ---------------------------------------------------------------
# Shared library: bill + sam
# ---------------------------------------------------------------
bill.o: bill.c lib.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

sam.o: sam.c lib.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

libshared.so: bill.o sam.o
	$(CC) -shared -o $@ $^

# ---------------------------------------------------------------
# Main program object
# ---------------------------------------------------------------
program.o: program.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

# Link-time interpositioning wrappers for fred and john
interpose_link.o: interpose_link.c lib.h
	$(CC) $(CFLAGS) -DLINKTIME -c $< -o $@

# Final executable:
#   -Wl,--wrap,fred / --wrap,john  -> route fred/john calls through wrappers
#   libstatic.a                    -> provides real fred and john
#   -L. -lshared                   -> provides bill and sam (load-time interposed at run)
#   -lm                            -> math library (required by john and sam)
program: program.o interpose_link.o libstatic.a libshared.so
	$(CC) $(CFLAGS) \
		-Wl,--wrap,fred -Wl,--wrap,john \
		-o $@ program.o interpose_link.o libstatic.a \
		-L. -lshared -lm

# ---------------------------------------------------------------
# Load-time interpositioning shared library for bill and sam
# ---------------------------------------------------------------
interpose_runt.so: interpose_runt.c lib.h
	$(CC) $(CFLAGS) -DRUNTIME -shared -fPIC -o $@ $< -ldl

# ---------------------------------------------------------------
# Run: link-time wrapping already baked in; LD_PRELOAD adds load-time
# ---------------------------------------------------------------
run: program interpose_runt.so
	LD_LIBRARY_PATH=. LD_PRELOAD=./interpose_runt.so ./program

# Show shared library dependencies
ldd: program
	ldd ./program

clean:
	rm -f *.o *.a *.so program
