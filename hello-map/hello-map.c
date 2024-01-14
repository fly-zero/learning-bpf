#include <linux/bpf.h>
#include <bpf/libbpf.h>
#define SEC(NAME) __attribute__((section(NAME), used))

static long (*bpf_map_update_elem)(void *map, const void *key, const void *value, __u64 flags) = (void *)BPF_FUNC_map_update_elem;
static void *(*bpf_map_lookup_elem)(void *map, const void *key) = (void *)BPF_FUNC_map_lookup_elem;

/* 用户空间通过 map 名称 my_map 获取 map 的文件描述符 */
struct bpf_map_def SEC("maps") my_map = {
    .type = BPF_MAP_TYPE_HASH, /* 类型为 BPF_MAP_TYPE_UNSPEC 时，需要用户空间程序对 maps 进行初始化 */
    .key_size = sizeof(int),
    .value_size = sizeof(int),
    .max_entries = 1,
};

/* 当 execve 系统调用被调用时，BPF 程序会被执行，将 execve 调用次数纪录在 map 中 */
SEC("tracepoint/syscalls/sys_enter_execve")
int bpf_prog1(void *ctx) {
    int key = 0;
    int new_value, *old_value;
    old_value = bpf_map_lookup_elem(&my_map, &key);
    new_value = old_value ? (*old_value + 1) : 1;
    bpf_map_update_elem(&my_map, &key, &new_value, BPF_ANY);
    return 0;
}