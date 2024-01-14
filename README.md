# Learning BPF

该项目是《Linux 内核观测技术 BPF》的示例代码，并在新版本的内核基础上进行修改，使用 `libbpf` 开发了加载器。

# Prerequisites

```bash
apt install \
    clang \
    gcc-multilib \
    libbpf-dev \
    linux-tools-common \
    linux-tools-generic \
    linux-tools-`uname -r`
```

## Catalog

* `hello` 第一个 bpf 程序
* `hello-map` 使用 bpf map 在内核与用户空间传递数据