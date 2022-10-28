CC = g++
CFLAGS = -Wall -std=c++11 -Iinclude

.PHONY : all
all: wordgenerator.o textreader.o partitioner.o strictpartitioner.o

libs: wordgenerator.a

wordgenerator.o: src/wordgenerator.cpp include/wordgenerator.h
	$(CC) $(CFLAGS) -c src/wordgenerator.cpp

textreader.o: src/textreader.cpp include/textreader.h
	$(CC) $(CFLAGS) -c src/textreader.cpp

partitioner.o: src/partitioner.cpp include/partitioner.h
	$(CC) $(CFLAGS) -c src/partitioner.cpp

strictpartitioner.o: src/strictpartitioner.cpp include/strictpartitioner.h partitioner.o
	$(CC) $(CFLAGS) -c src/strictpartitioner.cpp

partitionTester: tests/partitionTester.cpp partitioner.o wordgenerator.o strictpartitioner.o
	$(CC) $(CFLAGS) -o partitionTester tests/partitionTester.cpp partitioner.o wordgenerator.o strictpartitioner.o

wordgenerator.a: strictpartitioner.o partitioner.o textreader.o wordgenerator.o
	ar rcs wordgenerator.a strictpartitioner.o partitioner.o textreader.o wordgenerator.o

test: partitionTester
	./partitionTester

.PHONY: clean
clean:
	rm -f *.o *.a partitionTester
