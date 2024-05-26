.PHONY: all

all: hello.o loader

hello.o: hello.c
	clang -O2 -target bpf -c hello.c -o hello.o

loader: loader.c
	clang -g loader.c -o loader -lbpf