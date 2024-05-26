.PHONY: all

all: hello-map.o loader

hello-map.o: hello-map.c
	clang -O2 -target bpf -c $< -o $@

loader: loader.c
	clang -g loader.c -o loader -lbpf -lelf
