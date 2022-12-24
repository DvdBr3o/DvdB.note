# 小字符串优化 sso

LNK：[优化字符串String.md](优化字符串String.md)

在LNK提到过了，只是没提到它的正名 `sso` 。

这实际上是 `std` 库自己的优化，就是当你 `std::string` 大小不超过 `16 bytes` 时，它就是栈分配；否则就会被堆分配。

