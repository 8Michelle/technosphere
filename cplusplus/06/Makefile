CC=g++

format.o: format.cpp format.h
	$(CC) -c format.cpp

tests.o: tests.cpp tests.h
	$(CC) -c tests.cpp

mainTests.o: mainTests.cpp format.h tests.h
	$(CC) -c mainTests.cpp

clean:
	rm -rf *.o test

make test: format.o mainTests.o tests.o
	$(CC) -o test mainTests.o format.o tests.o
	./test
