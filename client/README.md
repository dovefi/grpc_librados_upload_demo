# 构建
```shell script
go build client.go
```

# 运行
- 客户端
```shell script
$ ./client ~/Desktop/pcbook.png
upload file: /Users/dovefi/Desktop/pcbook.png success
```
- 服务端输出
```shell script
$ ./file_server -c /home/dovefi/ceph12/build/ceph.conf
Server listening on 0.0.0.0:6888. Press Ctrl-C to end.
call UploadFile
upload file name /Users/dovefi/Desktop/pcbook.png
start write object
create object file succes
```
