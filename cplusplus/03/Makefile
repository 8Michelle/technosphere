CC=g++

matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp

tests.o: tests.cpp tests.h matrix.h
	$(CC) -c tests.cpp

mainTests.o: mainTests.cpp matrix.h tests.h
	$(CC) -c mainTests.cpp

clean:
	rm -rf *.o test

make test: matrix.o mainTests.o tests.o
	$(CC) -o test matrix.o mainTests.o tests.o
	./test
