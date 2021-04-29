//
// Created by dovefi on 22/04/2021.
//

#include <rados/librados.hpp>
#include <iostream>
#include <string>

#include "rados_client.h"

using std::cout;
using std::cerr;
using std::endl;


int RadosClient::write(std::string file_name, librados::bufferlist &data) {
  cout << "start write object" << endl;
  int ret;
  ret = io_ctx.write_full(file_name, data);
  if (ret < 0) {
    cerr << "could not write object! error" << ret << endl;
    ret = EXIT_FAILURE;
  } else {
    cout << "create object file success" << endl;
  }
  return ret;
}

int RadosClient::setup_pool(const char* pool_name) {
  int ret;
  pool = pool_name;
  ret = rados.pool_lookup(pool);
  if (ret < 0) {
    cerr << "couldn't lookup pool, maybe pool not exist! error " << ret << endl;
    return ret;
  } else {
    cout << "lookup success " << pool_name << endl;
  }

  librados::IoCtx ctx;
  ret = rados.ioctx_create(pool_name, ctx);
  if (ret < 0) {
    cerr << "could not set up ioctx error" << ret << endl;
    return ret;
  } else {
    cout << " created an ioctx for  pool" << endl;
  }
  io_ctx = ctx;

  return ret;
}

//int main(int argc, const char **argv) {
//  int ret = 0;
//  const char *conf;
//  const char *pool_name = "hello_world_pool";
//  std::string hello("hello world");
//  std::string object_name("hello_object");
//  librados::IoCtx io_ctx;
//
//  // read config file
//  for (int i = 0; i < argc; ++i) {
//    if ((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--conf") == 0)) {
//      conf = argv[1 + i];
//      break;
//    }
//  }
//
//  // init rados client
//  RadosClient rados_client(conf);
//  ret = rados_client.setup_pool(pool_name);
//  if (ret < 0) {
//    return ret;
//  }
//
//  // write object
//  librados::bufferlist bl;
//  bl.append(hello);
//  ret = rados_client.write(object_name, bl);
//  if (ret < 0) {
//    return ret;
//  }
//
//  return ret;
//}
