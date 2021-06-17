CC=g++
SRC=./src
TEST=./test
BIN=./bin

bt: $(TEST)/test1.cpp $(SRC)/waiterr.h
	mkdir -p $(BIN)
	$(CC) -Isrc -std=c++11 $(TEST)/test1.cpp -o $(BIN)/test.o

btr: bt
	$(BIN)/test.o

build:
	$(CC) -Isrc -o $(BIN)/server $(SRC)/test1.cpp

run: build
	$(BIN)/server

clean:
	rm -rf bin/
