.PHONY: all

all: execve-kprobe

execve-kprobe.o: execve-kprobe.cpp
	g++ -g -c $< -o $@

execve-kprobe: execve-kprobe.o
	g++ -g $< -o $@ -lbcc

clean:
	rm -f execve-kprobe.o execve-kprobe