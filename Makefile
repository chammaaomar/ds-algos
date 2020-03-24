vpath %.c src tests munit
vpath %.h include munit
ODIR := ./bin
SRC := ./src
TEST := ./tests
MUNIT := ./munit
array.o: array.c array.h 
	gcc -c $(SRC)/array.c -o $(ODIR)/$@
check: test_runner
	./$(ODIR)/test_runner
munit.o: munit.c munit.h
	gcc -c $(MUNIT)/munit.c -o $(ODIR)/$@
test.o: test.c test.h munit.h array.h
	gcc  -c $(TEST)/test.c -o $(ODIR)/$@
test_runner: test.o munit.o array.o
	gcc $(ODIR)/test.o $(ODIR)/munit.o $(ODIR)/array.o -o $(ODIR)/$@