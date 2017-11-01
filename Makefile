CC=g++
INCLUDE=include
SRC=src
CFLAGS=-O3 -Wall -std=c++11 -I./$(INCLUDE) -lpthread -pthread -lpqxx -lpq
DEPS=$(INCLUDE)/time_measurer.h
TARGETS=driver_main

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGETS)

driver_main: $(SRC)/driver_config.o $(SRC)/driver_program.o $(SRC)/driver_main.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(SRC)/*.o $(TARGETS)

