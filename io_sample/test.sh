rm -rf log
mkdir log
rm -f ./html/new/*

./a.out &  > "log/exe.log" 2> "log/error.log"

# curl -X GET localhost:4242 > "log/root.log"
# curl -X GET localhost:4242/index.html > "log/index.log"
# curl -X GET localhost:4242/fuga.html > "log/fuga.log"
# curl -X GET localhost:4242/hoge/ > "log/hoge_dir.log"
# curl -X GET localhost:4242/hoge/index.html > "log/hoge_index.log"

# cat ./html/new_index.html | curl -v -H "Content-type: text/html" -d - http://localhost:4242/new/ 2>&1 > "log/post_0.log"
# cat ./html/new_index.html | curl -v -H "Content-type: text/html" -d - http://localhost:4242/new/hogehoge.html  2>&1 > "log/post_1.log"
# cat ./html/new_index.html | curl -v -H "Content-type: text/html" -d - http://localhost:4242/unknown/hogehoge.html  2>&1 > "log/post_2.log"

touch html/delete.html
curl -v -X DELETE localhost:4242/delete.html
curl -v -X DELETE localhost:4242/oubwoeufbowuebf.html

kill `ps -aux | grep a.out | grep -v grep | awk '{print $2}'`
