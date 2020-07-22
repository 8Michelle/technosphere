CC=g++ --std=c++14
CFLAGS = -c

sorter.o: sorter.cpp sorter.h
	$(CC) $(CFLAGS) sorter.cpp

test.o: sorter.cpp sorter.h
	$(CC) $(CFLAGS) test.cpp

test: sorter.o test.o
	$(CC) sorter.o test.o -o test -lpthread
	./test

clean:
	rm *.o test output input
