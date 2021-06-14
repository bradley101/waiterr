CC=g++
SRC=./src
BIN=./bin

bt: $(SRC)/test1.cpp $(SRC)/waiterr.h
	mkdir -p $(BIN)
	$(CC) -Isrc -std=c++11 $(SRC)/test1.cpp -o $(BIN)/test.o

btr: bt
	$(BIN)/test.o

build:
	$(CC) -Isrc -o $(BIN)/server $(SRC)/test1.cpp

run: build
	$(BIN)/server

clean:
	rm -rf bin/
