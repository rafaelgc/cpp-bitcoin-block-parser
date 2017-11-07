CC=g++
CFLAGS=-I.
DEPS = picosha.hpp Helpers.hpp Blob.hpp Input.hpp Output.hpp Transaction.hpp BlockHeader.hpp Block.hpp
OBJ = Helpers.cpp Input.cpp Output.cpp Transaction.cpp BlockHeader.cpp Block.cpp main.cpp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -std=c++11

btc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -std=c++11
