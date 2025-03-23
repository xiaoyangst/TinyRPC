#include <string>
#include <thread>
#include <iostream>
#include "user.pb.h"
#include "rpc/RpcController.h"
#include "rpc/RpcChannel.h"

int main() {

	auto channel = new RpcChannel();
	test::UserServiceRpc_Stub rpc_stub(channel);

	test::LoginRequest login_request;
	login_request.set_name("xy");
	login_request.set_pwd("123");

	test::LoginResponse login_response;

	std::cout << "rpc login request" << std::endl;

	RpcController rpc_controller;
	rpc_stub.Login(&rpc_controller, &login_request, &login_response, nullptr);

	std::cout << "rpc login response" << std::endl;

	if (!rpc_controller.Failed()) {
		if (!login_response.success()) {
			std::cout << "rpc login failed" << std::endl;
			std::cout << "errcode = " << login_response.result().errcode() << std::endl;
			std::cout << "errmsg = " << login_response.result().errmsg() << std::endl;
			return 0;
		}
		std::cout << "rpc login success" << std::endl;
		std::cout << "success = " << login_response.success() << std::endl;
	} else {
		std::cout << rpc_controller.ErrorText() << std::endl;
	}

	return 0;
}