CC=g++

parser.o: parser.cpp parser.h
	$(CC) -c parser.cpp

tests.o: tests.cpp tests.h parser.h
	$(CC) -c tests.cpp

mainTests.o: mainTests.cpp parser.h
	$(CC) -c mainTests.cpp

test: parser.o mainTests.o tests.o
	$(CC) -o test mainTests.o parser.o tests.o
	./test

clean:
	rm -rf *.o test
