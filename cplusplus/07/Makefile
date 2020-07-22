CC=g++

tests.o: tests.cpp iterator.h allocator.h vector.h tests.h
	$(CC) -std=c++14 -c tests.cpp

mainTests.o: mainTests.cpp tests.h vector.h allocator.h iterator.h
	$(CC) -std=c++14 -c mainTests.cpp

clean:
	rm -rf *.o test

make test: tests.o mainTests.o
	$(CC) -std=c++14 -o test tests.o mainTests.o
	./test
