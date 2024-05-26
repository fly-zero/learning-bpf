from bcc import BPF
import os

bpf_source = """
BPF_HASH(cache, u64, u64);

int trace_start_time(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 start_time = bpf_ktime_get_ns();
    cache.update(&pid, &start_time);
    return 0;
}
"""

bpf_source += """
int trace_end_time(struct pt_regs *ctx) {
    u64 pid = bpf_get_current_pid_tgid();
    u64 *start_time = cache.lookup(&pid);
    if (!start_time) {
        return 0;
    }
    u64 end_time = bpf_ktime_get_ns();
    u64 delta = end_time - *start_time;
    bpf_trace_printk("pid %d: %d ns", pid, delta);
    cache.delete(&pid);
    return 0;
}
"""

# 获取当前工作目录
cwd = os.getcwd()

# 使用当前工作目录下的 a.out 程序
program = cwd + "/a.out"

# 使用 uprobe 跟踪 a.out 程序的 main 函数
bpf = BPF(text = bpf_source)
bpf.attach_uprobe(name = program, sym = "main", fn_name = "trace_start_time")
bpf.attach_uretprobe(name = program, sym = "main", fn_name = "trace_end_time")
bpf.trace_print()