CC     = gcc
CFLAGS = -Wall

.PHONY: all run clean

all: program interpose_runt.so

fred.o: fred.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

john.o: john.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

libstatic.a: fred.o john.o
	ar crv $@ $^

bill.o: bill.c lib.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

sam.o: sam.c lib.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

libshared.so: bill.o sam.o
	$(CC) -shared -o $@ $^

program.o: program.c lib.h
	$(CC) $(CFLAGS) -c $< -o $@

interpose_link.o: interpose_link.c lib.h
	$(CC) $(CFLAGS) -DLINKTIME -c $< -o $@

program: program.o interpose_link.o libstatic.a libshared.so
	$(CC) $(CFLAGS) \
		-Wl,--wrap,fred -Wl,--wrap,john \
		-o $@ program.o interpose_link.o libstatic.a \
		-L. -lshared -lm

interpose_runt.so: interpose_runt.c lib.h
	$(CC) $(CFLAGS) -DRUNTIME -shared -fPIC -o $@ $< -ldl

run: program interpose_runt.so
	LD_LIBRARY_PATH=. LD_PRELOAD=./interpose_runt.so ./program

ldd: program
	ldd ./program

clean:
	rm -f *.o *.a *.so program
