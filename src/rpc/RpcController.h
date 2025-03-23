/**
  ******************************************************************************
  * @file           : RpcController.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/22
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_RPC_RPCCONTROLLER_H_
#define TINYRPC_SRC_RPC_RPCCONTROLLER_H_

#include<google/protobuf/service.h>
#include<string>

 class RpcController : public google::protobuf::RpcController{
 public:
  void Reset();
  bool Failed() const;
  std::string ErrorText() const;
  void SetFailed(const std::string& reason);

  // 不实现，但必须存在
  void StartCancel(){}
  bool IsCanceled() const { return false; }
  void NotifyOnCancel(google::protobuf::Closure* callback){}
 private:
  bool is_fail=false;
  std::string fail_text;
};

#endif //TINYRPC_SRC_RPC_RPCCONTROLLER_H_
