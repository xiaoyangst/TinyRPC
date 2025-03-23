/**
  ******************************************************************************
  * @file           : RpcProvider.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/19
  ******************************************************************************
  */

#include <hv/EventLoop.h>

#include "RpcProvider.h"
#include "utils/Log.h"
#include "utils/Config.h"
#include "utils/HvProtocol.h"
#include "proto/rpc_header.pb.h"

void RpcProvider::Run() {
	// 从配置文件中读取 rpc_server 的 ip 和 port
	auto port = Config::getInstance()->get("rpc_port");
	if (port == std::nullopt) {
		LOG_ERROR("rpc_port not found in config file");
		return;
	}
	int rpc_port = std::stoi(port.value());

	auto ip = Config::getInstance()->get("rpc_ip");
	if (ip == std::nullopt) {
		LOG_ERROR("rpc_ip not found in config file");
		return;
	}
	const std::string &rpc_ip = ip.value();

	// 创建TcpServer
	hv::TcpServer tcp_server;
	auto listen_fd = tcp_server.createsocket(rpc_port, rpc_ip.c_str());
	if (listen_fd < 0) {
		LOG_ERROR("tcp_server.createsocket failed");
		return;
	}

	// 设置拆包规则
	server_unpack_setting = new unpack_setting_t();
	memset(server_unpack_setting, 0, sizeof(unpack_setting_t));
	server_unpack_setting->mode = UNPACK_BY_LENGTH_FIELD;
	server_unpack_setting->package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
	server_unpack_setting->body_offset = SERVER_HEAD_LENGTH;
	server_unpack_setting->length_field_offset = SERVER_HEAD_LENGTH_FIELD_OFFSET;
	server_unpack_setting->length_field_bytes = SERVER_HEAD_LENGTH_FIELD_BYTES;
	server_unpack_setting->length_field_coding = ENCODE_BY_BIG_ENDIAN;
	tcp_server.setUnpack(server_unpack_setting);

	//loop_ = tcp_server.loop();


	// 设置回调
	tcp_server.onConnection = [this](const hv::SocketChannelPtr &conn) {
	  this->OnConnection(conn);
	};
	tcp_server.onMessage = [this](const hv::SocketChannelPtr &conn, hv::Buffer *buf) {
	  this->OnMessage(conn, buf);
	};

	tcp_server.setThreadNum(4);
	tcp_server.start();

	std::cout << "RpcProvider start service at " << "ip: " << rpc_ip << " port: " << rpc_port << std::endl;

	while (getchar() != '\n');
}

/**
 * @brief 注册 RPC 服务
 * @param service
 */
void RpcProvider::NotifyService(google::protobuf::Service *service) {
	const auto service_ptr = service->GetDescriptor();    // 获取服务对象描述信息

	const std::string service_name = service_ptr->name();    // 获取服务名称
	auto method_count = service_ptr->method_count();        // 获得方法个数

	// 存储方法，便于查询
	std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> method_map;
	for (int i = 0; i < method_count; i++) {
		const auto method = service_ptr->method(i);
		const std::string method_name = method->name();
		method_map[method_name] = method;
	}

	ServiceInfo service_info = {service, method_map};
	service_dic[service_name] = service_info;
}

void RpcProvider::OnMessage(const hv::SocketChannelPtr &conn, hv::Buffer *buf) {
	auto data = std::string((char *)buf->data(), buf->size());

	std::string tmp_data;
	auto tmp_len = HvProtocol::unpackMessage(data, tmp_data);

	std::string actual_data;
	auto header_len = HvProtocol::unpackMessage(tmp_data, actual_data);

	auto header_data = actual_data.substr(0, header_len);
	auto args_data = actual_data.substr(header_len);

	tinyrpc::RpcHeader rpc_header = tinyrpc::RpcHeader();
	if (!rpc_header.ParseFromString(header_data)) {
		LOG_ERROR("ParseFromString failed");
		return;
	}

	// 反序列化
	auto service_name = rpc_header.service_name();
	auto method_name = rpc_header.method_name();
	auto args_len = rpc_header.args_len();

	// 遍历 service_dic
	for (auto &service : service_dic) {
		std::cout << "service_name: " << service.first << std::endl;
	}

	// 找到服务
	auto service_iter = service_dic.find(service_name);
	if (service_iter == service_dic.end()) {
		LOG_ERROR("service not found");
		return;
	}
	auto service_info = service_iter->second;
	auto service = service_info.service_ptr;

	// 找到服务对应的方法
	auto method_iter = service_info.method_dic.find(method_name);
	if (method_iter == service_info.method_dic.end()) {
		LOG_ERROR("method not found");
		return;
	}
	auto method = method_iter->second;

	// 方法所需的参数
	auto request = service->GetRequestPrototype(method).New();

	if (!request->ParseFromString(args_data)) {
		LOG_ERROR("ParseFromString failed");
		return;
	}

	auto response = service->GetResponsePrototype(method).New();

	// 调用服务提供的方法
	auto done = google::protobuf::NewCallback<RpcProvider, const hv::SocketChannelPtr &, google::protobuf::Message *>(
		this, &RpcProvider::SendRpcResponse, conn, response);

#if 1
	// 打印服务名、方法名、参数
	std::cout << "service_name: " << service_name << std::endl;
	std::cout << "method_name: " << method_name << std::endl;
	std::cout << "method_args: " << request->SerializeAsString() << std::endl;
#endif
	service->CallMethod(method, nullptr, request, response, done);		// 调用提供的 rpc 服务，其内部会调用本地 rpc 服务

}

void RpcProvider::SendRpcResponse(const hv::SocketChannelPtr &conn, google::protobuf::Message *response) {
	std::string response_str;
	std::cout << "response: " << response->DebugString() << std::endl;
	if (!response->SerializeToString(&response_str)) {
		LOG_ERROR("SerializeToString failed");
		return;
	}

	conn->write(HvProtocol::packMessageAsString(response_str));
	conn->close();
}

void RpcProvider::OnConnection(const hv::SocketChannelPtr &conn) {
	std::string peerAddr = conn->peeraddr();
	if (conn->isConnected()) {
		printf("%s connected! conn_fd=%d\n", peerAddr.c_str(), conn->fd());
	} else {
		printf("%s disconnected! conn_fd=%d\n", peerAddr.c_str(), conn->fd());
	}
}