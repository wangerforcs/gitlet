# 在gitlet上一级目录使用，面向测试用例编程时根据用例编写的
java gitlet.Main init
java gitlet.Main branch B1
java gitlet.Main branch B2
java gitlet.Main checkout B1
echo "This is a wug." > h.txt
java gitlet.Main add h.txt
java gitlet.Main commit "Add h.txt"
java gitlet.Main checkout B2
echo "This is a wug." > f.txt
java gitlet.Main add f.txt
java gitlet.Main commit "f.txt added"
java gitlet.Main branch C1
echo "This is not a wug." > g.txt
java gitlet.Main add g.txt
java gitlet.Main rm f.txt
java gitlet.Main commit "g.txt added and f.txt removed"
java gitlet.Main checkout B1
java gitlet.Main merge C1
java gitlet.Main merge B2