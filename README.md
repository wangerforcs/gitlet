根据CS61B实现的gitlet C++版本。
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
├── src
│   ├── Utils.cpp
│   ├── blob.cpp
│   ├── commit.cpp
│   ├── repo.cpp
│   ├── sha1.cpp
│   └── stage.cpp
├── test.bash
├── testJava.bash
├── testing
    ├── Makefile
    ├── out.txt
    ├── runner.py
    ├── samples
    ├── src
    └── tester.py

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

虽然gitlet和git的实现上存在不同，但对于了解git的原理、实现和基础命令有很大帮助。

最开始使用了自己编写的小型测试并没出现问题，出于对自己的不信任，我还是费力找到了autograder，然后东拼西凑找到了缺少的测试文件，并使用Main.java包装测试了一下:)

~~其实开始是准备直接改测试代码的，但难度有点大，果断放弃~~

不出意外，44个基础测试只有22个通过，接下来是面向测试用例编程时间。主要修改了Fail输出的信息，以及补上了对于一些特殊情况的处理，大多是merge，比如an untracked file in the current commit would be overwritten or deleted by the merge的情况以及merge conflict的情况。
~~中间还被"\n"坑了一下导致43一直没过，原因是复制的conflict文件末尾没有"\n"~~

其中merge涉及在有向无环图找到最近公共祖先的问题,简单的做法就是对一个branch进行bfs，每到一个节点就dfs判断节点是否是另一个branch的祖先；或者直接先获得另一个branch的所有祖先集合。

补充一下测试中in文件的阅读问题
I: 导入预置命令
+: 写入文件
\>: gitlet命令 
=: 测试文件内容

> 疑惑  
> 实现merge时，spec要求的是1和5中都要求相应的文件staged，这点不清楚是什么意思。  
> merge操作涉及重写文件时，不应该直接在commit和工作目录中修改文件，然后提交commit并且移动HEAD吗？和暂存区有什么关系？