[TOC]

# 环境准备

## ceph 环境准备
- 操作系统: debian 10.3
- ceph 版本 ：v12.2.13

### 编译过程

#### 编译依赖包和运行程序
```shell script
$ git clone -b v12.2.13 git://github.com/ceph/ceph

$ git submodule update --init --recursive

$ ./install-deps.sh

$ ./do_cmake.sh

$ cd build

$ make
```

#### 启动测试集群
- 启动测试集群目的是为了配套测试使用
    ```shell script

    $ cd build
    $ make vstart        # builds just enough to run vstart
    $ MON=3 MDS=0 OSD=3 RGW=1 ../src/vstart.sh -d -n -x -l -b
    $ ./bin/ceph -s
    ```
- 关闭测试集群

    ```shell script
    $ ../src/stop.sh
    ```

- 创建测试用 pool

    ```shell script
    $ ./bin/rados mkpool hello_world_pool 8
    ```
## grpc sever 环境准备
### 编译安装grpc
- 初始环境
```shell script
$ export MY_INSTALL_DIR=$HOME/.local

$ mkdir -p $MY_INSTALL_DIR

$ export PATH="$MY_INSTALL_DIR/bin:$PATH"

// 安装cmake
$ sudo apt install -y cmake

$ sudo apt install -y build-essential autoconf libtool pkg-config

```
- 安装grpc
```shell script
$ git clone --recurse-submodules -b v1.35.0 https://github.com/grpc/grpc

$ cd grpc
$ mkdir -p cmake/build
$ pushd cmake/build
$ cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
$ make -j
$ make install
$ popd
$ mkdir -p third_party/abseil-cpp/cmake/build
$ pushd third_party/abseil-cpp/cmake/build
$ cmake -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
      ../..
$ make -j
$ make install
$ popd
```
# 编译程序
使用 `Makefile` 中的编译命令，记得修改相应的路径
```shell script
make server
```
如果无法运行，则手动运行 Makefile中的命令

# 运行程序
```shell script
./file_server -c /home/dovefi/ceph12/build/ceph.conf

Server listening on 0.0.0.0:6888. Press Ctrl-C to end.
```
> - -c: 测试ceph集群的配置文件路径

### 参考文档
- [grpc 官网教程](https://grpc.io/docs/languages/cpp/quickstart/)
- [github 文件上传](https://github.com/yitzikc/grpc-file-exchange)
- [ceph 编译安装](https://github.com/ceph/ceph/tree/v12.2.13)
- [librados(C) api](https://docs.ceph.com/en/latest/rados/api/librados/#librados-c)
