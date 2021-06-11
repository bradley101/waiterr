CC=g++
SRC=./src
BIN=./bin

build:
	$(CC) -o $(BIN)/server $(SRC)/server.cpp

run: build
	$(BIN)/server