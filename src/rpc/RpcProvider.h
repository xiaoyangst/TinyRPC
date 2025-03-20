/**
  ******************************************************************************
  * @file           : RpcProvider.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/19
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_RPC_RPCPROVIDER_H_
#define TINYRPC_SRC_RPC_RPCPROVIDER_H_

#include <memory>
#include <map>
#include <string>
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <hv/TcpServer.h>

class RpcProvider {
 public:
  void NotifyService(google::protobuf::Service *service);
  void Run();
  void OnConnection(const hv::SocketChannelPtr &conn);
  void OnMessage(const hv::SocketChannelPtr &conn, hv::Buffer *buf);
  void SendRpcResponse(const hv::SocketChannelPtr &conn, google::protobuf::Message *response);
 private:
  unpack_setting_t *server_unpack_setting;
  struct ServiceInfo {
	google::protobuf::Service *service_ptr;
	std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> method_dic;
  };
  std::unordered_map<std::string, ServiceInfo> service_dic;    // 存储所有注册的 RPC 服务，方便后续根据服务名找到对应的方法
  std::shared_ptr<hv::EventLoop> loop_;
};

#endif //TINYRPC_SRC_RPC_RPCPROVIDER_H_
