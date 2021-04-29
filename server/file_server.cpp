//
// Created by dovefi on 22/04/2021.
//

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <map>
#include <cstdint>
#include <stdexcept>

#include "grpc/grpc.h"
#include "grpc++/server.h"
#include "grpc++/server_builder.h"
#include "grpc++/server_context.h"
#include "grpc++/security/server_credentials.h"

#include "pb/file_service.pb.h"
#include "pb/file_service.grpc.pb.h"
#include "rados_client.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;

using pb::LaptopService;
using pb::UploadFileRequest;
using pb::UploadFileResponse;
using pb::FileInfo;




int count = 0;

class LaptopServiceImpl final : public LaptopService::Service {
private:
  RadosClient& rados_client;

public:
  LaptopServiceImpl(RadosClient& client):rados_client(client){}
  Status UploadFile(
          ServerContext *context, ServerReader <UploadFileRequest> *reader,
          UploadFileResponse *response) override {

    UploadFileRequest uploadFilePart;

    std::cout << "call UploadFile" << std::endl;
    count += 1;
    int ret;
    librados::bufferlist bl;
    std::string name = "/tmp/grpc_cpp_image" + std::to_string(count);
    while (reader->Read(&uploadFilePart)) {
      try {
        if (uploadFilePart.has_info()) {
          FileInfo* info = uploadFilePart.mutable_info();
          name = info->name();
          std::cout << "upload file name " << name << std::endl;
        }
        auto *const data = uploadFilePart.mutable_chunk_data();
        bl.append(*data);
        data->clear();
        response->set_id(name);

      } catch (const std::exception& e) {
        return Status(StatusCode::ABORTED, e.what());
      }
    }
    ret = rados_client.write(name, bl);
    if (ret < 0) {
      return Status::CANCELLED;
    }

    return Status::OK;
  }
};

int main(int argc, char **argv) {
  std::string server_address("0.0.0.0:6888");
  int ret = 0;
  const char *conf;
  const char *pool_name = "hello_world_pool";
  std::string hello("hello world");
  std::string object_name("hello_object");

  // read config file
  for (int i = 0; i < argc; ++i) {
    if ((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--conf") == 0)) {
      conf = argv[1 + i];
      break;
    }
  }

  // init rados client
  RadosClient rados_client(conf);
  ret = rados_client.setup_pool(pool_name);
  if (ret < 0) {
    return ret;
  }

  LaptopServiceImpl service(rados_client);
//  LaptopServiceImpl service;

  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << ". Press Ctrl-C to end." << std::endl;
  server->Wait();
}