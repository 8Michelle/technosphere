CC=g++
MODE=c++11

serializer.o: serializer.cpp serializer.h
	$(CC) -std=$(MODE) -c serializer.cpp

tests.o: tests.cpp serializer.h tests.h
	$(CC) -std=$(MODE) -c tests.cpp

mainTests.o: mainTests.cpp tests.h serializer.h
	$(CC) -std=$(MODE) -c mainTests.cpp

clean:
	rm -rf *.o test

make test: tests.o serializer.o mainTests.o
	$(CC) -std=$(MODE) -o test tests.o serializer.o mainTests.o
	./test
