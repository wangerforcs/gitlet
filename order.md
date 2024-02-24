鉴于git命令总是忘记，在此稍微记录一下

## git命令
### 创建仓库
git init 初始化本地仓库
git clone url 克隆远程仓库

### 配置仓库
查看全部配置
git config --list

常用修改用户和邮箱
本仓库配置
git config user.name "your-username"
git config user.email "your-email-address"
全局配置
git config --global user.name "your-username"
git config --global user.email "your-email-address"

也可以直接编辑文件，分别对应
.git/config
~/.gitconfig

### 文件增删
git add file1 file2 ~~ 将文件提交到暂存区
git add folder 将指定文件夹提交到暂存区，包括全部子文件
git add . 快捷方式，将全部文件改动提交到暂存区，包括新增，删除和修改
git add -u 将已经被git管理的文件提交到暂存区，不包括新的untracked文件
git add -A 将全部文件提交到暂存区，包括新增，删除和修改
2.0版本后.和-A是一样的.
git add * 忽略.开头的隐藏文件和文件夹，其他效果和git add .一样

git rm file1 file2 ~~ 将文件从工作区删除，并把删除操作添加到暂存区
git rm --cached file1 file2 ~~ 将文件从暂存区删除，但保留在工作区

git ls-files 查看暂存区的文件

### 提交
git commit -m "message" 将暂存区的记录提交到当前分支
git commit -a 将工作区自上次commit以来的所有变化提交到当前分支，不包括untracked新文件。相当于git add -u和git commit -m "message"的合并。
git commit -v 显式两次commit之间的diff信息
git commit --amend -m "message" 使用一次新的commit，替代上一次提交
如果代码没有任何新变化，则用来改写上一次commit的提交信息
git commit --amend file1 file2 ~~ 将指定文件的修改合并到上一次commit中，不产生新的commit

### 分支
git branch 查看本地分支
git branch -r 查看远程分支
git branch -a 查看全部分支
git branch name 创建新分支
git branch -m oldname newname 重命名分支
git checkout name 切换分支并更新工作区
git checkout -b name 创建新分支并切换
git branch -d name 删除分支
git push origin --delete name 删除远程分支
git branch --set-upstream name remote-name 在现有分支与指定的远程分支之间，建立追踪关系
git merge name 合并指定分支到当前分支

### 标签
相当于给commit起了别名，通常用于版本发布
git tag 列出所有tag
git tag name 在当前commit建立tag
git tag name commit 在指定commit建立tag
git show tag 查看tag信息
git push origin name 提交tag
git push origin --tags 提交所有tag
git tag -d name 删除tag
git push origin :refs/tags/name 删除远程tag

### 查看
git status 查看仓库状态，包括branch，stage，untracked file以及修改但未stage的文件等

git log 查看commit历史
git log --stat 查看commit历史，包括文件修改统计
git log -S name 查看指定字符串的commit
git log --follow file 查看文件的commit历史

git diff 查看工作区和暂存区的差异
git diff --cached name 查看暂存区和上次commit的文件差异
git diff HEAD 查看工作区和上次commit的文件差异
git diff branch1 branch2 查看两个分支的差异

git show commitid 查看指定commit的详细信息
git show commitid:file 查看指定commit的文件内容

git reflog 查看最近几次commit的操作记录

### 撤销
git checkout filename 撤销工作区对文件的修改，即恢复到暂存区的状态
git checkout commitid filename 恢复指定commit的文件到工作区和暂存区
git checkout . 恢复暂存区所有文件到工作区

git reset filename 重置暂存区的文件，和上一次commit一样
git reset --hard 重置工作区和暂存区到上一次commit一样
git reset commitid 重置HEAD到指定commit，重置暂存区，但不影响工作区
git reset --hard commitid 重置HEAD到指定commit，重置暂存区和工作区
git reset --keep commitid 重置HEAD到指定commit，暂存区和工作区不变

### 回退
git checkout commitid 切换到指定commit，会创建新的分支，使用git switch -c name将新分支重命名，用于保存回退版本的commit

### 远程
git remote -v 查看远程仓库
git remote show origin 查看远程仓库详细信息
git remote add name url 添加远程仓库，name是远程仓库的别名，通常为origin
git remote set-url name url 修改远程仓库的url
git remote rename oldname newname 重命名远程仓库
git push name branch 提交本地分支到远程仓库
git push name --force 强制提交到远程仓库，即使有冲突

git fetch 获取远程仓库的更新
git pull name branch 拉取远程分支到本地，并与本地分支合并


## git使用

通常的使用步骤
- 本地完成修改
- git add . 提交到暂存区
- git commit -m "message" 提交到本地仓库
- git push origin branch 提交到远程仓库

暂存区到工作区的撤销使用git checkout
本地仓库到暂存区的撤销，移动HEAD使用git reset

实战怎么这么多坑 :(，目前主要还是只用了简单的add，commit，branch命令，还有很多功能没有用到，以后再慢慢补充吧。