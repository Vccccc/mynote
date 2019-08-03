## git 工作原理
!["git"](./git.jpg)
WorkSpace: 工作区
Index/Stage: 暂存区
Repository: 仓库区(或本地仓库)
Remote: 远程仓库

## git commands
##### config [--global] user.name
git config user.name "cv": 将"cv"设置为该仓库的用户名。使用 --global 参数表示使用"cv"作为该机器下所有仓库的用户名。

##### config [--global] user.email
git config user.email "xxx@qq.com": 将"xxx@qq.com"设置为该仓库的用户email。使用 --global 参数表示使用"xxx@qq.com"作为该机器下所有仓库的用户email。

##### init
git init:将当前目录初始化为一个仓库。

##### add
git add file: 将file添加到暂存区。

##### commit
git commit [-m]: 将暂存区的文件提交到仓库。-m 参数用于注释。

##### Create a Branch
**Branching** is the way to work on different versions of a repository at one time.