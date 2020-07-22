CC=g++

tests.o: tests.cpp tests.h allocator.h
	$(CC) -c tests.cpp

allocator.o: allocator.cpp allocator.h
	$(CC) -c allocator.cpp

mainTests.o: mainTests.cpp tests.h allocator.h
	$(CC) -c mainTests.cpp

clean:
	rm -rf *.o test

make test: tests.o allocator.o mainTests.o
	$(CC) -o test allocator.o tests.o mainTests.o
	./test
