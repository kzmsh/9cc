# 開発環境

自分のMacがM1でIntelのアセンブラを扱えない問題を回避するため、GCEインスタンスを使って開発を進める。

## Ubuntu 環境のセットアップ

筆者の Dockerfile を参考にした: https://www.sigbus.info/compilerbook/Dockerfile

```
$ sudo apt update -y && sudo apt upgrade -y
```

```
$ sudo apt install -y gcc make git binutils libc6-dev gdb
```
