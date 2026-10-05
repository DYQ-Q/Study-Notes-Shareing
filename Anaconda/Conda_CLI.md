# 1. Conda 简介

Conda 是 Python 中最常用的：

```text
包管理和环境管理工具
```

由 Anaconda 公司开发，主要用于：

* 管理 Python 包（安装、更新、卸载）
* 创建和管理隔离的虚拟环境
* 管理不同版本的 Python
* 管理非 Python 包（如 C 库、R 包等）

与 pip 和 venv 不同：

```text
pip：仅管理 Python 包，依赖关系处理有限
venv：仅创建虚拟环境，不管理包
Conda：同时管理环境和包，支持非 Python 依赖
```

Conda 与其他工具对比：

```text
conda   : 全能型（环境 + 包管理，跨语言）
pip     : Python 专用包管理
venv    : Python 专用环境管理
poetry  : Python 依赖管理与打包
```

---

# 2. Conda 版本选择

## 2.1 Anaconda vs Miniconda

| 版本 | 说明 | 适用场景 |
| ---- | ---- | -------- |
| **Anaconda** | 完整发行版，预装 150+ 常用包（NumPy、Pandas、Scikit-learn 等），安装包约 3-5 GB | 新手、数据科学初学者、不想手动安装包 |
| **Miniconda** | 精简版，仅包含 conda 和 Python，安装包约 50 MB | 老手、定制化环境、节省磁盘空间 |

推荐：初学者先从 **Miniconda** 开始，按需安装包更灵活。

---

## 2.2 安装 Miniconda

### Linux/macOS

```bash
# 下载安装脚本
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh

# 运行安装
bash Miniconda3-latest-Linux-x86_64.sh

# 重启终端或激活
source ~/.bashrc
```

### Windows

```text
1. 访问 https://docs.conda.io/en/latest/miniconda.html
2. 下载 Windows 64-bit 安装包（.exe）
3. 运行安装程序，按提示安装
4. 安装完成后打开 Anaconda Prompt 或 PowerShell
```

---

## 2.3 验证安装

```bash
conda --version
```

输出示例：

```text
conda 24.5.0
```

---

# 3. 环境管理

## 3.1 为什么需要虚拟环境？

```text
• 不同项目依赖不同包版本（如项目A需要 TensorFlow 2.10，项目B需要 2.15）
• 避免系统 Python 被污染
• 方便项目迁移和复现
• 方便清理和删除
```

---

## 3.2 查看环境信息

```bash
# 查看 conda 版本
conda --version

# 查看当前环境信息
conda info

# 查看所有环境
conda env list
conda info --envs
```

输出示例：

```text
# conda environments:
#
base                  *  /opt/miniconda3
myproject                /opt/miniconda3/envs/myproject
tf2                     /opt/miniconda3/envs/tf2
```

`*` 表示当前激活的环境。

---

## 3.3 创建环境

```bash
# 创建环境并指定 Python 版本
conda create -n myproject python=3.10

# 创建环境并安装包
conda create -n myproject python=3.10 numpy pandas

# 从文件创建环境
conda env create -f environment.yml
```

---

## 3.4 激活与退出环境

```bash
# 激活环境
conda activate myproject

# 退出环境（回到 base）
conda deactivate
```

---

## 3.5 复制环境

```bash
# 复制已有环境
conda create -n newenv --clone oldenv
```

---

## 3.6 删除环境

```bash
conda env remove -n myproject
```

---

## 3.7 重命名环境

```bash
# Conda 没有直接的 rename 命令，通过克隆+删除实现
conda create -n newname --clone oldname
conda env remove -n oldname
```

---

# 4. 包管理

## 4.1 搜索包

```bash
# 搜索包
conda search numpy

# 搜索特定版本的包
conda search numpy=1.24
```

---

## 4.2 安装包

```bash
# 安装包
conda install numpy

# 安装指定版本
conda install numpy=1.24.3

# 安装多个包
conda install numpy pandas matplotlib

# 从指定频道安装
conda install -c conda-forge scikit-learn

# 只从指定频道安装（不使用默认频道）
conda install -c conda-forge --no-defaults pytorch

# 安装当前环境的 requirements.txt
conda install --file requirements.txt
```

---

## 4.3 查看已安装的包

```bash
# 当前环境的包列表
conda list

# 指定环境的包列表
conda list -n myproject

# 查看包详细信息
conda list numpy

# 查看某个包是否安装
conda list | grep numpy
```

---

## 4.4 更新包

```bash
# 更新指定包
conda update numpy

# 更新所有包
conda update --all

# 更新 conda 本身
conda update conda

# 更新 anaconda 元包（完整发行版）
conda update anaconda
```

---

## 4.5 卸载包

```bash
# 卸载包
conda remove numpy

# 卸载多个包
conda remove numpy pandas matplotlib

# 卸载包并清理依赖
conda remove --force numpy
```

---

# 5. 环境导出与导入

## 5.1 导出环境（跨平台共享）

```bash
# 导出到 environment.yml（包含包名和版本）
conda env export > environment.yml

# 导出到 environment.yml（仅包含显式安装的包）
conda env export --from-history > environment.yml
```

environment.yml 示例：

```yaml
name: myproject
channels:
  - defaults
  - conda-forge
dependencies:
  - python=3.10
  - numpy=1.24
  - pandas=2.0
  - pip
  - pip:
    - requests
    - pyyaml
```

---

## 5.2 从文件创建环境

```bash
conda env create -f environment.yml
```

---

## 5.3 导出为 requirements.txt（pip 兼容）

```bash
# 导出当前环境的包（conda + pip）
conda list --export > requirements.txt

# 仅导出 pip 包（用于非 conda 环境）
pip freeze > requirements.txt
```

---

# 6. 频道管理

## 6.1 什么是频道？

```text
频道（Channel）是 Conda 包的存储仓库
类似于 PyPI 对于 pip
```

常用频道：

```text
defaults      ：Anaconda 官方频道（默认）
conda-forge   ：社区驱动频道，包更新最快、最全
bioconda      ：生物信息学专用频道
pytorch       ：PyTorch 官方频道
```

---

## 6.2 添加/删除频道

```bash
# 查看当前频道
conda config --show channels

# 添加频道（优先级最高）
conda config --add channels conda-forge

# 移除频道
conda config --remove channels conda-forge

# 设置频道优先级为严格模式
conda config --set channel_priority strict
```

---

## 6.3 频道优先级

```text
conda-forge 比 defaults 更新更快、包更全
建议：将 conda-forge 设为最高优先级
```

配置命令：

```bash
conda config --add channels conda-forge
conda config --set channel_priority strict
```

---

# 7. 进阶操作

## 7.1 清理缓存

```bash
# 查看缓存大小
conda info

# 清理索引缓存
conda clean -i

# 清理下载包缓存
conda clean -p

# 清理所有（下载包 + 索引 + 未使用的包）
conda clean -all
```

---

## 7.2 离线安装

```bash
# 下载包到本地缓存
conda install numpy --download-only

# 从本地安装
conda install /path/to/package.tar.bz2
```

---

## 7.3 锁定包版本

```bash
# 锁定 conda 版本
conda config --set conda_version 24.3.0
```

---

## 7.4 Conda 与 pip 混用

```text
• 先用 conda 安装，conda 没有再用 pip
• 避免在 conda 环境中使用 pip install --upgrade（可能破坏 conda 依赖）
• 导出环境时用 conda env export 包含 pip 包
```

```bash
# 使用 pip 安装包
pip install requests

# 查看混合安装的包
conda list
```

---

# 8. Conda 常用命令速查表

| 功能 | 命令 |
| ---- | ---- |
| 查看版本 | `conda --version` |
| 查看环境信息 | `conda info` |
| 列出所有环境 | `conda env list` |
| 创建环境 | `conda create -n env_name python=3.10` |
| 激活环境 | `conda activate env_name` |
| 退出环境 | `conda deactivate` |
| 删除环境 | `conda env remove -n env_name` |
| 克隆环境 | `conda create -n new --clone old` |
| 安装包 | `conda install numpy` |
| 安装指定版本 | `conda install numpy=1.24` |
| 安装多个包 | `conda install numpy pandas` |
| 从 conda-forge 安装 | `conda install -c conda-forge scikit-learn` |
| 更新包 | `conda update numpy` |
| 更新所有包 | `conda update --all` |
| 卸载包 | `conda remove numpy` |
| 列出包 | `conda list` |
| 搜索包 | `conda search numpy` |
| 导出环境 | `conda env export > environment.yml` |
| 从文件创建环境 | `conda env create -f environment.yml` |
| 添加频道 | `conda config --add channels conda-forge` |
| 清理缓存 | `conda clean -all` |

---

# 9. 常见问题

## 1. Conda 安装包太慢

```text
方案一：使用国内镜像源
方案二：使用 conda-forge 频道
方案三：使用 mamba（C++ 实现的更快版本）
```

配置清华镜像源：

```bash
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/
conda config --set show_channel_urls yes
```

---

## 2. 环境激活失败

```bash
# 重新初始化 conda
conda init bash   # Linux/macOS
conda init cmd.exe   # Windows
```

---

## 3. 包冲突（Solving environment 卡住）

```text
• 使用 mamba 代替 conda 解决（更快）
• 使用 strict channel priority
• 指定包版本再尝试
• 使用 conda update --all 更新基础包
```

---

## 4. Conda 与 pip 包冲突

```text
• 优先使用 conda 安装，conda 没有再用 pip
• 安装 pip 包前，建议先创建新的 conda 环境
• 使用 pip install --no-deps 避免破坏 conda 依赖
```

---

## 5. 环境太大，占用磁盘空间

```bash
# 清理缓存
conda clean -all

# 导出最小环境文件
conda env export --from-history > environment.yml
```

---

# 10. Conda 与其他工具对比

| 特性 | Conda | pip + venv | poetry |
| ---- | ----- | ---------- | ------ |
| 环境管理 | ✅ | ✅（venv） | ✅ |
| 包管理 | ✅ | ✅ | ✅ |
| 跨语言支持 | ✅（Python/R/C++） | ❌（仅 Python） | ❌（仅 Python） |
| 依赖解析 | ✅（SAT 求解器） | ❌ | ✅ |
| 锁定文件 | ✅（conda-lock） | ❌ | ✅ |
| 发布包 | ❌ | ✅（PyPI） | ✅（PyPI） |
| 速度 | 较慢 | 快 | 中等 |
| 非 Python 依赖 | ✅ | ❌ | ❌ |

---

# 11. 推荐学习路线

```text
安装 Conda（Miniconda/Anaconda）
↓
理解环境管理（为什么需要虚拟环境）
↓
创建、激活、退出、删除环境
↓
包管理（搜索、安装、更新、卸载）
↓
频道管理（defaults vs conda-forge）
↓
环境导出与导入（environment.yml）
↓
清理与优化
↓
与 pip 混用策略
```

---

# 12. 推荐练习项目

## 初级

* 创建两个不同 Python 版本的环境
* 在环境中安装 numpy 和 pandas

---

## 中级

* 导出环境文件并分享给同事
* 使用 conda-forge 频道安装 scikit-learn

---

## 高级

* 配置国内镜像源加速下载
* 使用 mamba 替代 conda
* 创建包含 conda 和 pip 包的混合环境文件

---

# 13. 总结

Conda 的核心思想：

```text
环境 + 包 + 依赖 = 一个命令搞定
隔离让项目互不影响
分享让复现变得简单
```