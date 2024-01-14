#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static int running = 1;

static void sigint_handler(int sig) {
    running = 0;
}

int main() {
    struct bpf_object *obj;
    int trace_pipe_fd;

    // 初始化 BPF 程序
    obj = bpf_object__open("hello.o");
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        exit(EXIT_FAILURE);
    }

    // 加载 BPF 程序
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        exit(EXIT_FAILURE);
    }

    // 打开 trace_pipe
    trace_pipe_fd = open("/sys/kernel/debug/tracing/trace_pipe", O_RDONLY);
    if (trace_pipe_fd < 0) {
        fprintf(stderr, "Failed to open trace_pipe\n");
        exit(EXIT_FAILURE);
    }

    // 设置信号处理函数
    signal(SIGTERM, sigint_handler);

    // 读取 trace_pipe
    while (running) {
        char buf[4096];
        ssize_t n = read(trace_pipe_fd, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }

            fprintf(stderr, "Failed to read trace_pipe\n");
            exit(EXIT_FAILURE);
        }
        printf("%.*s", (int)n, buf);
    }

    // 关闭 trace_pipe
    close(trace_pipe_fd);

    // 卸载 BPF 程序
    bpf_object__unload(obj);

    // 释放 BPF 程序
    bpf_object__close(obj);

    return 0;
}