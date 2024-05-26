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
    obj = bpf_object__open("hello-map.o");
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        exit(EXIT_FAILURE);
    }

    // 加载 BPF 程序
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        exit(EXIT_FAILURE);
    }

    // 通过名称获取 prog
    struct bpf_program * prog = bpf_object__find_program_by_name(obj, "bpf_prog");
    if (!prog) {
        fprintf(stderr, "Failed to find prog\n");
        exit(EXIT_FAILURE);
    }

    // 将 BPF 程序附加到 tracepoint
    struct bpf_link *link = bpf_program__attach(prog);
    if (!link) {
        fprintf(stderr, "Failed to attach BPF program\n");
        exit(EXIT_FAILURE);
    }

    // 通过名称打开 map_fd
    int map_fd = bpf_object__find_map_fd_by_name(obj, "my_map");
    if (map_fd < 0) {
        fprintf(stderr, "Failed to find map_fd\n");
        exit(EXIT_FAILURE);
    }

    // 设置信号处理函数
    signal(SIGTERM, sigint_handler);

    // 读取 trace_pipe
    while (running) {
        // 查找 map
        int key = 0, err, value;
        err = bpf_map_lookup_elem(map_fd, &key, &value);
        if (err == 0) {
            printf("execve() was called %d times\n", value);
        } else {
            perror("bpf_map_lookup_elem");
        }
        sleep(1);
    }

    // 销毁 link
    bpf_link__destroy(link);

    // 关闭 trace_pipe
    close(trace_pipe_fd);

    // 卸载 BPF 程序
    bpf_object__unload(obj);

    // 释放 BPF 程序
    bpf_object__close(obj);

    return 0;
}