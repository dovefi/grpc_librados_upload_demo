# 通过s3接口实现文件上传到ceph 存储

# 背景
## client
client 从本地获取文件，并通过grpc 上传到 server

语言: go

## server 
server 接收grpc 客户端发送的文件，调用librados api 将文件上传到ceph 集群

语言: c++

