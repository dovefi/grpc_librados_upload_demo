//
// Created by dovefi on 26/04/2021.
//

#ifndef GRPC_SERVER_DEMO_RADOS_CLIENT_H
#define GRPC_SERVER_DEMO_RADOS_CLIENT_H

#include <rados/librados.hpp>
#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;

class RadosClient {
public:
    RadosClient(const char* conf) : conf(conf) {
      int ret;

      // init rados user
      ret = rados.init("admin");
      if (ret < 0) {
        cerr << "couldn not initialize rados, error" << ret << endl;
        exit(ret);
      } else {
        cout << "we just initialize rados" << endl;
      }

      // init conf file
      ret = rados.conf_read_file(conf);
      if (ret < 0) {
        cerr << "failed to parse config file " << conf << " error" << ret << endl;
        exit(ret);
      }

      // connect to cluster
      ret = rados.connect();
      if (ret < 0) {
        cerr << "could not connect to cluster error" << ret << endl;
        exit(ret);
      } else {
        cout << "we just connected to the rados cluster" << endl;
      }
    };

    ~RadosClient() {
      cout << "rados client exit" << endl;
      rados.shutdown();
    };

    int write(std::string file_name, librados::bufferlist &data);

    int setup_pool(const char* pool_name);

private:
    librados::IoCtx io_ctx;
    librados::Rados rados;
    const char *pool;
    const char *conf;

};
#endif //GRPC_SERVER_DEMO_RADOS_CLIENT_H
