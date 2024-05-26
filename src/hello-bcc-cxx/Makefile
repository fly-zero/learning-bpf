.PHONY: all

all: execve-kprobe execve-tracepoint

execve-kprobe.o: execve-kprobe.cpp
	g++ -g -c $< -o $@

execve-tracepoint.o: execve-tracepoint.cpp
	g++ -g -c $< -o $@

execve-kprobe: execve-kprobe.o
	g++ -g $< -o $@ -lbcc

execve-tracepoint: execve-tracepoint.o
	g++ -g $< -o $@ -lbcc

clean:
	rm -f execve-kprobe.o execve-kprobe execve-tracepoint.o execve-tracepoint