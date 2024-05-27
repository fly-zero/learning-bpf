#include <sys/sdt.h>
#include <unistd.h>

#include <stdio.h>

int main()
{
    // 观察反汇编代码，可以看到此处的 DTRACE_PROBE 会被替换为 nop 指令
    // 该 nop 指令将作为一个占位符，等待 BPF 程序注入真正的指令
    DTRACE_PROBE(hello-usdt, probe-main);
    printf("%u: Hello, BPF World!\n", getpid());
    return 0;
}