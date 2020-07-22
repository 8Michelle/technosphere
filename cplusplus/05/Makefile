CC=g++

bigInt.o: bigInt.cpp bigInt.h
	$(CC) -c bigInt.cpp

tests.o: tests.cpp bigInt.h tests.h
	$(CC) -c tests.cpp

mainTests.o: mainTests.cpp bigInt.h tests.h
	$(CC) -c mainTests.cpp

clean:
	rm -rf *.o test

make test: tests.o bigInt.o mainTests.o
	$(CC) -o test tests.o bigInt.o mainTests.o
	./test
