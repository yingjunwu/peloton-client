CC=g++
INCLUDE=include
SRC=src
CFLAGS=-O3 -Wall -std=c++11 -I./$(INCLUDE) -lpthread -pthread -lpqxx -lpq
DEPS=$(INCLUDE)/time_measurer.h
TARGETS=peloton_client

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGETS)

peloton_client: $(SRC)/client_config.o $(SRC)/client_program.o $(SRC)/client_main.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(SRC)/*.o $(TARGETS)

