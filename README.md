# Learning BPF

该项目是 BPF 的示例代码

# Books

[<img src=".assets/linx-observability-with-bpf.jpg" width="250" height="328">](https://www.oreilly.com/library/view/linux-observability-with/9781492050193/)

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

* `hello` 第一个 bpf 程序，并使用 `libbpf` 开发了一个简单的加载器
* `hello-map` 使用 bpf map 在内核与用户空间传递数据