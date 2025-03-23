#include <string>
#include "user.pb.h"
#include "utils/Log.h"
#include "rpc/RpcProvider.h"

// 服务端，提供服务，将远程调用服务注册，给客户端发现调用

class UserService : public test::UserServiceRpc {
 public:

  // 本地服务，在没有考虑分布式之前，单体架构下的本地服务调用
  bool Login(const std::string &name, const std::string &pwd) {
	  std::cout << "doing local service: Login" << std::endl;
	  std::cout << "name: " << name << "pwd: " << pwd << std::endl;
	  return true;
  }

  // Rpc 服务，但还需要注册
  void Login(::google::protobuf::RpcController *controller,
			 const ::test::LoginRequest *request,
			 ::test::LoginResponse *response,
			 ::google::protobuf::Closure *done) override {
	  // 业务逻辑
	  std::cout << "doing remote service: Login" << std::endl;
	  const auto &name = request->name();
	  const auto &pwd = request->pwd();
	  std::cout << "name: " << name << "pwd: " << pwd << std::endl;
	  auto login_result = Login(name, pwd);
	  test::ResultCode *code = response->mutable_result();
	  if (login_result) {
		  code->set_errcode(0);
		  code->set_errmsg("login success");
	  } else {
		  code->set_errcode(1);
		  code->set_errmsg("login failed");
	  }
	  done->Run();
  }
};

int main() {
	// 注册服务
	RpcProvider provider;
	provider.NotifyService(new UserService());
	provider.Run();
	return 0;
}