from bcc import BPF, USDT

import os

bpf_source = """
#include <uapi/linux/ptrace.h>
int trace_binary_exec(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    bpf_trace_printk("New hello-usdt process running with PID %d", pid);
    return 0;
}
"""

cwd = os.getcwd()
path = cwd + "/hello-usdt"
usdt = USDT(path = path)
usdt.enable_probe(probe = "probe-main", fn_name = "trace_binary_exec") # probe-main 是 hello-usdt 程序中的 probe 名称
bpf = BPF(text = bpf_source, usdt_contexts = [usdt])
bpf.trace_print()