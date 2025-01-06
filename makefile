all: bin/main

bin/main:  src/main.c src/heap.c
	gcc -o $@ $^

clean:
	rm -rf bin/*
