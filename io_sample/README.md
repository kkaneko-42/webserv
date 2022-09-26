# I/O多重化サンプルプログラム
## 使い方
```
make
./a.out & telnet localhost 4242
```
でecho serverが起動し、クライアントが立ち上がります。通常のecho serverと同様、送信した文字列がレスポンスとなります。

## TODO
- 切断処理
- 正常に終了
    - 今はinvalid eventでexitしている

## 参考
ほぼ以下のサンプルと同じ内容です。

https://www.ibm.com/docs/ja/i/7.2?topic=designs-using-poll-instead-select
