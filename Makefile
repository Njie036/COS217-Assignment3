# Dependency rules for non-file targets
all: testsymtablelist testsymtablehash
clobber: clean
	rm -f *~ \#*\#
clean:
	rm -f symtablelist *.o


# Dependency rules for file targets
testsymtablelist: testsymtablelist.o symtablelist.o
	gcc217 testsymtablelist.o symtablelist.o -o testsymtablelist
testsymtablelist.o: testsymtablelist.c symtable.h
	gcc217 -c testsymtablelist.c
symtablelist.o: symtablelist.c symtable.h
	gcc217 -c symtablelist.c