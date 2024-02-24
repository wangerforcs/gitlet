根据CS61B实现的gitlet C++版本。
项目目录结构如下
```
.
├── Main.class
├── Main.java
├── Makefile
├── README.md
├── bin
├── gitlet
├── include
│   ├── Utils.h
│   ├── blob.h
│   ├── commit.h
│   ├── repo.h
│   ├── sha1.h
│   └── stage.h
├── main.cpp
├── order.md
├── out.txt
├── run.py
├── src
│   ├── Utils.cpp
│   ├── blob.cpp
│   ├── commit.cpp
│   ├── repo.cpp
│   ├── sha1.cpp
│   └── stage.cpp
├── test.bash
├── test1.txt
├── test2.txt
├── test3.txt
├── testJava.bash
├── testing
│   ├── Makefile
│   ├── out.txt
│   ├── runner.py
│   ├── samples
│   ├── src
│   └── tester.py
└── tree.txt

6 directories, 108 files
```
代码文件大小如下，共约1k行
```
   37 include/Utils.h
   27 include/blob.h
   38 include/commit.h
   51 include/repo.h
   32 include/sha1.h
   28 include/stage.h
  112 src/Utils.cpp
   57 src/blob.cpp
  135 src/commit.cpp
  513 src/repo.cpp
  115 src/sha1.cpp
   97 src/stage.cpp
   99 main.cpp
 1341 total
```
~~本人未使用autograder，只使用了简单的命令查看结果。~~

命令详见

[test.bash](test.bash)

```bash
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
```

运行完成后你应该会看到类似下面的输出

[out.txt](out.txt)

```txt
make: 'gitlet' is up to date.
rm -rf .gitlet
===
commit 2f9cdeb253e20732597fd2771b4c9e0287a211f8
Date: Sun Feb 25 00:17:13 2024 -0800
dev write test1 test2

===
commit d5eee8fc89cbe0ae24934e705da2e38795b223bb
Date: Sun Feb 25 00:17:13 2024 -0800
master write test1

===
commit 21090c2318e4465f8711209b59ab2d0eceea12c0
Date: Thu Jan 1 00:00:00 1970 -0800
initial commit

=== Branches ===
master
*dev

=== Staged Files ===

=== Removed Files ===

=== Modifications Not Staged For Commit ===

=== Untracked Files ===
Encountered a merge conflict.
===
commit 6cd207a2a2b1a4e4e585b3c71a0d17d58c69e7ae
Merge: 10300e3 2f9cdeb 
Date: Sun Feb 25 00:17:13 2024 -0800
Merged dev into master.

===
commit 10300e3af9090acbb2f58d7fe3aaf5ae5a193454
Date: Sun Feb 25 00:17:13 2024 -0800
master write test3 rm test1

===
commit d5eee8fc89cbe0ae24934e705da2e38795b223bb
Date: Sun Feb 25 00:17:13 2024 -0800
master write test1

===
commit 21090c2318e4465f8711209b59ab2d0eceea12c0
Date: Thu Jan 1 00:00:00 1970 -0800
initial commit

===
commit d5eee8fc89cbe0ae24934e705da2e38795b223bb
Date: Sun Feb 25 00:17:13 2024 -0800
master write test1

===
commit 21090c2318e4465f8711209b59ab2d0eceea12c0
Date: Thu Jan 1 00:00:00 1970 -0800
initial commit
```
~~以上均输出和文档内容均正确(未使用autograder盲目自信，所以我无耻地使用了代码修改后的输出。~~

> 疑惑  
> 实现merge时，spec要求的是1和5中都要求相应的文件staged，这点不清楚是什么意思。  
> merge操作涉及重写文件时，不应该直接在commit和工作目录中修改文件吗？和暂存区有什么关系？

虽然gitlet和git的实现上存在不同，但对于了解git的原理、实现和基础命令有很大帮助。

好吧出于对自己的不信任，我还是费力找到了autograder，并使用Main.java包装测试了一下:)

~~其实开始是准备直接改测试代码的，但难度有点大，果断放弃~~

不出意外，44个基础测试只有22个通过，接下来是面向测试用例编程时间。主要修改了Fail输出的信息，以及补上了对于一些特殊情况的处理，主要是merge，比如an untracked file in the current commit would be overwritten or deleted by the merge的情况。

ec之外的测试中33，36，43由于缺少src文件无法正常测试，其余均通过，应该完成的还算可以。

其中merge涉及在有向无环图找到最近公共祖先的问题,简单的做法就是对一个branch进行bfs，每到一个节点就dfs判断节点是否是另一个branch的祖先；或者直接先获得另一个branch的所有祖先集合。