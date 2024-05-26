#include <linux/bpf.h>
#define SEC(NAME) __attribute__((section(NAME), used))

static int (*bpf_trace_printk)(const char *fmt, int fmt_size, ...) = (void *)BPF_FUNC_trace_printk;

/* BPF 中字符串需要放在 .rodata 段中 */
char msg[] SEC(".rodata") = "Hello, BPF World!";

SEC("tracepoint/syscalls/sys_enter_execve")
int bpf_prog(void *ctx) {
    bpf_trace_printk(msg, sizeof msg);
    return 0;
}

char _license[] SEC("license") = "GPL";