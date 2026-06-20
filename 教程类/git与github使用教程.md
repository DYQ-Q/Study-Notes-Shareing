# 1. Git 介绍和核心概念

## 1.1 什么是 Git

Git 是目前最流行的：

```text
分布式版本控制系统
```

由 Linus Torvalds 于 2005 年创建，用于 Linux 内核开发。

---

## 1.2 为什么需要版本控制

```text
• 记录文件的历史变更
• 可以随时回退到任何历史版本
• 多人协作时合并各自修改
• 分支管理，支持并行开发
• 备份和恢复代码
```

---

## 1.3 核心概念

### 工作区、暂存区、仓库

```text
┌─────────────────┐     git add    ┌─────────────┐    git commit  ┌─────────────┐
│                 │ ─────────────> │             │ ──────────────>│             │
│Working Directory│                │Index/Staging│                │  Repository │
│                 │ <───────────── │             │ <──────────────│             │
└─────────────────┘   git restore  └─────────────┘   git reset    └─────────────┘
```

三个区域说明：

```text
工作区   ：电脑上实际看到的文件和目录
暂存区   ：临时存放准备提交的修改（索引区）
仓库     ：存储所有提交历史的数据库（.git 目录）
```

---

### 文件状态

Git 中的文件有四种状态：

```text
┌──────────┐      ┌──────────┐      ┌──────────┐      ┌──────────┐
│ Untracked│ ────>│ Modified │ ────>│ Staged   │ ────>│ Committed│
└──────────┘      └──────────┘      └──────────┘      └──────────┘
```

状态说明：

```text
Untracked : 新文件，Git 尚未跟踪
Modified  : 已跟踪的文件发生了修改，但未暂存
Staged    : 已暂存，等待提交
Committed : 已提交到仓库
```

---

### 分支

```text
分支是指向某个提交记录的指针
默认分支通常为 main 或 master
HEAD 指向当前所在分支的最新提交
```

---

### 提交

```text
提交（commit）是 Git 的基本单位
每次提交都有一个唯一的哈希值（如 3a2b1c0）
包含：快照、作者信息、时间戳、提交信息、父提交引用
```

---

### 远程仓库

```text
本地仓库在开发者的机器上
远程仓库托管在服务器上（如 GitHub）
本地与远程通过 push/pull 同步
```

---

# 2. 基础指令

## 2.1 安装与配置

### 安装 Git

| 平台 | 命令/方法 |
| ---- | --------- |
| Ubuntu/Debian | `sudo apt install git` |
| CentOS/RHEL | `sudo yum install git` |
| macOS | `brew install git` |
| Windows | 下载安装包 https://git-scm.com/download/win |

### 配置用户信息

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

### 查看配置

```bash
git config --list
```

---

## 2.2 初始化仓库

```bash
# 在当前目录创建新仓库
git init
```

输出：

```text
Initialized empty Git repository in /path/to/project/.git/
```

---

## 2.3 克隆仓库

```bash
# 从远程克隆到本地
git clone https://github.com/user/repo.git
git clone git@github.com:user/repo.git   # SSH 方式
```

---

## 2.4 查看状态

```bash
git status
git status -s   # 简短格式
```

---

## 2.5 添加文件（暂存）

```bash
git add file.txt         # 添加单个文件
git add .                # 添加所有改动
git add -A               # 添加所有改动（包括删除）
git add *.txt            # 添加所有 .txt 文件
```

---

## 2.6 提交

```bash
git commit -m "提交信息"
git commit -am "信息"     # 跳过暂存（仅已跟踪文件）
git commit --amend       # 修改上一次提交
```

---

## 2.7 查看历史

```bash
git log                          # 完整日志
git log --oneline                # 简洁一行
git log --oneline --graph        # 图形化显示分支
git log -n 5                     # 最近5次
git log --author="名字"           # 按作者过滤
```

---

## 2.8 差异比较

```bash
git diff                # 工作区 vs 暂存区
git diff --staged       # 暂存区 vs HEAD
git diff HEAD           # 工作区 vs HEAD
#HEAD:指向当前所在分支的最新一次提交
```

---

## 2.9 撤销操作

```bash
# 撤销工作区的修改（回到最近 commit 的状态）
git restore file.txt
git checkout -- file.txt    # 旧语法

# 撤销暂存区的修改（回到工作区）
git restore --staged file.txt
git reset HEAD file.txt     # 旧语法

# 撤销上一次提交（保留修改）
git reset --soft HEAD~1

# 完全回退到上一次提交（丢弃修改，危险）
git reset --hard HEAD~1
```

---

## 2.10 分支操作

```bash
# 查看分支
git branch              # 本地分支
git branch -r           # 远程分支
git branch -a           # 所有分支

# 创建分支
git branch feature-xxx

# 切换分支
git checkout feature-xxx
git switch feature-xxx      # Git 2.23+

# 创建并切换
git checkout -b feature-xxx
git switch -c feature-xxx

# 合并分支
git checkout main
git merge feature-xxx

# 删除分支
git branch -d feature-xxx
git branch -D feature-xxx   # 强制删除（未合并）
```

---

## 2.11 查看远程仓库

```bash
git remote -v#查看当前连接的远程仓库
```

输出：

```text
origin  https://github.com/user/repo.git (fetch)
origin  https://github.com/user/repo.git (push)
```

---

## 2.12 拉取与推送

```bash
# 拉取远程更新
git pull origin main         # 拉取并合并
git pull --rebase            # 拉取并变基

# 只拉取不合并
git fetch origin

# 推送本地修改
git push origin main
git push -u origin main      # 首次推送并设置上游
```

---

## 2.13 忽略文件（.gitignore）

创建 `.gitignore` 文件：

```text
# 注释
*.log
*.tmp
/build/
/node_modules/
.DS_Store
!important.log    # 例外
```

常用 `.gitignore` 模板：

```text
Python:   https://github.com/github/gitignore/blob/main/Python.gitignore
Node:     https://github.com/github/gitignore/blob/main/Node.gitignore
Java:     https://github.com/github/gitignore/blob/main/Java.gitignore
```

---

## 2.14 暂存修改（stash）

```bash
git stash              # 暂存当前修改
git stash list         # 查看暂存列表
git stash pop          # 恢复并删除最近一次
git stash apply        # 恢复但保留
git stash drop         # 删除
```

---

## 2.15 标签

```bash
git tag v1.0.0                      # 轻量标签
git tag -a v1.0.0 -m "版本说明"      # 附注标签
git push origin v1.0.0              # 推送标签
git push --tags                     # 推送所有标签
```

---

# 3. GitHub

## 3.1 什么是 GitHub

```text
GitHub 是基于 Git 的代码托管平台
提供：代码托管、协作开发、Issue 跟踪、Pull Request、CI/CD 等
```

---

## 3.2 注册账号

```text
访问 https://github.com
点击 Sign up 注册账号
验证邮箱
```

---

## 3.3 SSH 密钥配置（推荐）

### 生成 SSH 密钥

```bash
ssh-keygen -t rsa -b 4096 -C "your.email@example.com"
```

### 查看公钥

```bash
cat ~/.ssh/id_rsa.pub
```

### 添加到 GitHub

```text
Settings → SSH and GPG keys → New SSH key
粘贴公钥内容 → Add SSH key
```

### 测试连接

```bash
ssh -T git@github.com
```

输出：

```text
Hi username! You've successfully authenticated...
```

---

## 3.4 GitHub 与本地仓库的关联方式

| 方式 | 地址格式 | 特点 |
| ---- | -------- | ---- |
| HTTPS | `https://github.com/user/repo.git` | 需输入用户名/密码或 token |
| SSH | `git@github.com:user/repo.git` | 配置密钥后免密 |

### 两种方式的切换

```bash
# 查看当前远程地址
git remote -v

# 修改为 SSH
git remote set-url origin git@github.com:user/repo.git

# 修改为 HTTPS
git remote set-url origin https://github.com/user/repo.git
```

---

# 4. 创建仓库

## 4.1 在 GitHub 上创建仓库

```text
1. 点击右上角 + → New repository
2. 填写仓库名称（Repository name）
3. 选择公开（Public）或私有（Private）
4. 可选择添加 README（初始化说明文件）
5. 可选添加 .gitignore（忽略文件模板）
6. 可选选择许可证（License）
7. 点击 Create repository
```

---

## 4.2 从命令行创建新仓库（本地已有项目）

```bash
# 1. 进入项目目录
cd my-project

# 2. 初始化 Git 仓库
git init

# 3. 添加所有文件
git add .

# 4. 提交
git commit -m "首次提交"

# 5. 关联远程仓库
git remote add origin https://github.com/用户名/仓库名.git

# 6. 推送到远程
git push -u origin main
```

---

## 4.3 从命令行克隆已有仓库

```bash
git clone https://github.com/用户名/仓库名.git
git clone git@github.com:用户名/仓库名.git   # SSH
```

---

## 4.4 创建仓库时的初始化选项

```text
README.md   : 项目说明文档
.gitignore  : 忽略文件配置（选择语言模板）
License     : 开源许可证（MIT、Apache、GPL 等）
```

---

# 5. 上传文件到远端仓库

## 5.1 完整的上传流程

```bash
# 1. 查看当前状态
git status

# 2. 将文件添加到暂存区
git add .                    # 添加所有
git add 文件名               # 添加指定文件

# 3. 提交到本地仓库
git commit -m "提交信息"

# 4. 拉取远程最新代码（避免冲突）
git pull origin main

# 5. 推送到远程仓库
git push origin main
```

---

## 5.2 首次推送（设置上游分支）

```bash
git push -u origin main
```

`-u` 参数作用：

```text
建立本地分支与远程分支的跟踪关系
之后可以直接使用 git push / git pull
```

---

## 5.3 推送流程示意图

```text
┌──────────────┐     git add    ┌─────────────┐    git commit  ┌─────────────┐    git push   ┌─────────────┐
│Work Directory│ ──────────────>│   Staging   │ ──────────────>│ Local Repo  │ ─────────────>│ Remote repo │
└──────────────┘                └─────────────┘                └─────────────┘               └─────────────┘
```

---

## 5.4 处理推送冲突

当远程仓库有本地没有的提交时，推送会被拒绝：

```text
! [rejected] main -> main (fetch first)
error: failed to push some refs to '...'
```

解决步骤：

```bash
# 1. 拉取远程更新
git pull origin main

# 2. 解决冲突（如果有）
# 手动编辑冲突文件，删除 <<<<<<< ======= >>>>>>> 标记
git add .
git commit -m "解决冲突"

# 3. 再次推送
git push origin main
```

---

## 5.5 常用上传场景

### 场景一：首次上传项目

```bash
git init
git add .
git commit -m "initial commit"
git remote add origin git@github.com:用户名/仓库名.git
git push -u origin main
```

### 场景二：上传修改（日常）

```bash
git add .
git commit -m "修改说明"
git pull origin main      # 先拉取最新
git push origin main
```

### 场景三：上传到不同分支

```bash
git checkout -b feature-xxx
git add .
git commit -m "新功能开发"
git push -u origin feature-xxx
```

---

## 5.6 常见 Git 操作流程速查

| 步骤 | 命令 | 说明 |
| ---- | ---- | ---- |
| 初始化 | `git init` | 创建本地仓库 |
| 关联远程 | `git remote add origin <url>` | 关联 GitHub 仓库 |
| 添加 | `git add .` | 添加所有文件到暂存区 |
| 提交 | `git commit -m "msg"` | 提交到本地仓库 |
| 拉取 | `git pull origin main` | 获取远程最新更新 |
| 推送 | `git push origin main` | 推送到 GitHub |

---

# 6. 常用命令速查表

| 功能 | 命令 |
| ---- | ---- |
| 查看状态 | `git status` |
| 查看历史 | `git log --oneline` |
| 添加文件 | `git add .` |
| 提交 | `git commit -m "msg"` |
| 推送 | `git push origin main` |
| 拉取 | `git pull origin main` |
| 获取远程更新 | `git fetch origin` |
| 查看分支 | `git branch` |
| 创建分支 | `git branch <name>` |
| 切换分支 | `git switch <name>` |
| 合并分支 | `git merge <name>` |
| 删除分支 | `git branch -d <name>` |
| 撤销修改 | `git restore <file>` |
| 撤销暂存 | `git restore --staged <file>` |
| 撤销提交 | `git reset --soft HEAD~1` |
| 强制回退 | `git reset --hard <commit>` |
| 暂存修改 | `git stash` |
| 恢复暂存 | `git stash pop` |
| 打标签 | `git tag -a v1.0 -m "msg"` |
| 推送标签 | `git push --tags` |

---
