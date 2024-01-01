CC = g++
CFLAGS = -std=c++17 -Wall

all: escalona

escalona: main.o schedule.o
	$(CC) main.o schedule.o -o escalona

main.o: main.cpp schedule.hpp
	$(CC) -c main.cpp $(CFLAGS)

schedule.o: schedule.cpp schedule.hpp
	$(CC) -c schedule.cpp $(CFLAGS)

clean:
	-rm -f *~ *.o

purge: clean
	-rm -f escalona
