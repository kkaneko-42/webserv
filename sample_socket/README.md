# ソケット通信 サンプルプログラム
## 使い方
### echo_server
```
./echo_server & telnet localhost 4242
```
で、echo server起動 & クライアント起動(curlで動作しませんでした...)
telnetで文字列を入力 + Enterすると、送信された文字列が返ってきます。

### mini_nginx
```
./mini_nginx
```
で、1回だけhtmlを送信するサーバーが起動します。
その後、ブラウザでhttp://localhost:4242に接続すると、nginxのhtmlが見られます。

※echo serverと同様、
```
telnet localhost 4242
```
としても、index.htmlが取得できます。

### 注意点
サーバーが終了してから約1分程度は、新しくサーバーが立てられない問題があります。下記URLが関係しているかもです。
https://ez-net.jp/article/FB/beofK19H/z5CwHgjxeJRA/
