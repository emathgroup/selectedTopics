---
title: 自建电子书的云端服务
date: 2019-09-09 11:06:57
thumbnail: "../thumbnails/bash.png"
tags:
    - "bash"
---

[calibre] 是一个很强大的电子书阅读器，自带了很多功能，其中一个就是calibre-server，可以轻松实现云端共享。

## 界面启动服务
打开[calibre]界面程序，依次点击**connect/share** -->**start content server**

## 命令行启动服务

` calibre-server --port=8080 /home/wayne/CalibreLibrary `

## systemd自启动服务
编写 **calibre-server.service** 文件。内容：

```
[Unit]
Description=calibre content server
After=network.target

[Service]
Type=simple
User=wayne
Group=wayne
ExecStart=/usr/bin/calibre-server --port=8080 /home/wayne/CalibreLibrary
[Install]
WantedBy=multi-user.target

```
然后enable起来，试一下.

```shell

sudo systemctl enable calibre-server
sudo systemctl daemon-reload
sudo systemctl restart calibre-server

sudo systemctl status calibre-server

```
[calibre]: https://calibre-ebook.com/download

## 阅读端
通过 `sudo systemctl status calibre-server` 查看服务地址。如：
```
● calibre-server.service - calibre content server
   Loaded: loaded (/etc/systemd/system/calibre-server.service; enabled; vendor preset: enabled)
   Active: active (running) since Sun 2018-09-09 11:17:29 CST; 2s ago
 Main PID: 7113 (calibre-server)
   CGroup: /system.slice/calibre-server.service
           └─7113 /opt/calibre/bin/calibre-server --port=8080 /home/wayne/CalibreLibrary

Sep 09 11:17:29 X6Ti systemd[1]: Started calibre content server.
Sep 09 11:17:30 X6Ti calibre-server[7113]: calibre server listening on 0.0.0.0:8080
Sep 09 11:17:31 X6Ti calibre-server[7113]: OPDS feeds advertised via BonJour at: 192.168.11.105 port: 8080
```
然后，打开浏览器 http://192.168.11.105:8080 ，即可。