## 创建密钥对

```bash
ssh key-gen
#对于新手，一直点回车就可以，密钥会放在C//Users/用户名/.ssh目录下，id_rsa是私钥，id_rsa.pub是公钥
```
## github配置密钥

1. 在github网站点击设置，找到SSH and GPG keys，点击New SSH key，将公钥复制到key中(打开id_rsa.pub，全选复制过去)，点击Add SSH key即可。
2. 测试连接
```bash
ssh -T git@github.com
#首次使用ssh，会询问你是否信任该主机，输入yes即可
#如果出现Hi username! You've successfully authenticated, but GitHub does not provide shell access.就说明连接成功
```

## 设置remote url

```bash
#将远程仓库地址设置为ssh方式
git remote add origin git@github.com:username/repository.git #这个ssh地址需要创建一个repository,然后复制ssh地址
#如果已经设置过http连接，可以使用以下命令修改
git remote set-url origin git@github.com:username/repository.git
```


