HOME := /home/root
GRPC_INSTALL_DIR := $(HOME)/.local
GRPC_SOURCE_DIR := $(HOME)/grpc
LIBRADOS_DIR := /home/dovefi/ceph12

PKG_CONFIG_PATH=$(GRPC_SOURCE_DIR)/cmake/build/third_party/protobuf/:$(GRPC_SOURCE_DIR)/cmake/build/libs/opt/pkgconfig

proto_service:
	$(GRPC_INSTALL_DIR)/bin/protoc -I ./proto --proto_path=proto proto/*.proto --grpc_out=pb --plugin=protoc-gen-grpc=$(GRPC_INSTALL_DIR)/bin/grpc_cpp_plugin

proto_message:
	$(GRPC_INSTALL_DIR)/bin/protoc -I ./proto --proto_path=proto proto/*.proto --cpp_out=pb

proto: proto_service proto_message

clean:
	/bin/rm pb/*

server:
	export PKG_CONFIG_PATH=$(PKG_CONFIG_PATH)
	g++ --std=c++11   file_server.cpp rados_client.cpp pb/*.cc -o file_server -I $(LIBRADOS_DIR)/src/include/ -L $(LIBRADOS_DIR)/build/lib -lrados -lceph-common -Wl,-rpath,$(LIBRADOS_DIR)/build/lib/ -I $(GRPC_INSTALL_DIR)/include/ `pkg-config --libs protobuf grpc++ grpc`
