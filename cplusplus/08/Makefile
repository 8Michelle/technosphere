CC=g++

tests.o: tests.cpp threadPool.h
	$(CC) -std=c++14 -c tests.cpp

clean:
	rm -rf *.o test

make test: tests.o threadPool.h
	$(CC) -std=c++14 -o test tests.o
	./test
