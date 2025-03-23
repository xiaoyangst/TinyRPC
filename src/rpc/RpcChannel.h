/**
  ******************************************************************************
  * @file           : RpcChannel.h
  * @author         : xy
  * @brief          : 客户端使用
  * @attention      : None
  * @date           : 2025/3/21
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_RPC_RPCCHANNEL_H_
#define TINYRPC_SRC_RPC_RPCCHANNEL_H_

#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>


class RpcChannel : public google::protobuf::RpcChannel{
 public:
  void CallMethod(const google::protobuf::MethodDescriptor* method,
				  google::protobuf::RpcController* controller, const google::protobuf::Message* request,
				  google::protobuf::Message* response, google::protobuf::Closure* done);
};

#endif //TINYRPC_SRC_RPC_RPCCHANNEL_H_
