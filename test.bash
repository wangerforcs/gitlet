# 在gitlet根目录使用
make
make del
./gitlet init

echo "master write test1" > test1.txt
./gitlet add test1.txt

./gitlet commit "master write test1"

./gitlet branch dev
./gitlet checkout dev

echo "dev write test1" > test1.txt
echo "dev write test2" > test2.txt
./gitlet add test1.txt
./gitlet add test2.txt

./gitlet commit "dev write test1 test2"

./gitlet checkout master
./gitlet checkout dev

./gitlet log
./gitlet status

./gitlet checkout master
echo "master write test3" > test3.txt
./gitlet add test3.txt
./gitlet rm test1.txt
./gitlet commit "master write test3 rm test1"

./gitlet merge dev
./gitlet log

./gitlet checkout dev
./gitlet reset $(./gitlet find "master write test1")
./gitlet log

./gitlet checkout master
./gitlet rm-branch dev