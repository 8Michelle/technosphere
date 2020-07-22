CC=g++ --std=c++14
CFLAGS = -c

counter.o: counter.cpp counter.h threadPool.h
	$(CC) $(CFLAGS) counter.cpp

test.o: counter.cpp counter.h
	$(CC) $(CFLAGS) test.cpp

test: counter.o test.o
	$(CC) counter.o test.o -o test -lpthread
	./test

clean:
	rm *.o test file
