# 在gitlet上一级目录使用，面向测试用例编程时根据用例编写的
java gitlet.Main init
java gitlet.Main branch given
echo "This is a wug." > f.txt
java gitlet.Main add f.txt
java gitlet.Main commit "Add f.txt containing wug.txt"
java gitlet.Main checkout given
echo "This is not a wug." > f.txt
java gitlet.Main add f.txt
java gitlet.Main commit "Add f.txt containing notwug.txt"
java gitlet.Main branch B
java gitlet.Main merge master
cat f.txt
echo "This is not a wug." > f.txt
java gitlet.Main add f.txt
java gitlet.Main commit "reset f to notwug.txt"
java gitlet.Main rm f.txt
java gitlet.Main commit "given now empty"
java gitlet.Main checkout master
echo "Another wug." > g.txt
java gitlet.Main add g.txt
java gitlet.Main commit "Add g.txt containing wug.txt"
java gitlet.Main merge B
cat f.txt
echo "This is a wug." > f.txt
java gitlet.Main add f.txt
java gitlet.Main commit "reset f to wug.txt"
java gitlet.Main merge given
cat f.txt
cat g.txt