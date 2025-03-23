/**
  ******************************************************************************
  * @file           : Zookeeper.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/23
  ******************************************************************************
  */

#include <cassert>
#include <semaphore.h>
#include "Zookeeper.h"
#include "Config.h"

void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *wwatcherCtx) {
	if (type == ZOO_SESSION_EVENT) {
		if (state == ZOO_CONNECTED_STATE) {

			sem_t *sem = (sem_t *)zoo_get_context(zh);
			sem_post(sem);
		}
	}
}

void Zookeeper::start() {
	auto ip = Config::getInstance()->get("zk_ip");
	assert(ip != std::nullopt);
	const std::string &zk_ip = ip.value();
	auto port = Config::getInstance()->get("zk_port");
	assert(port != std::nullopt);
	const auto& zk_port = port.value();

	auto conn = zk_ip + ":" + zk_port;

	m_handle = zookeeper_init(conn.c_str(), nullptr, 10000, nullptr, nullptr, 0);
	assert(m_handle != nullptr);

	sem_t sem;
	sem_init(&sem, 0, 0);
	zoo_set_context(m_handle, &sem);
	sem_wait(&sem);
}

void Zookeeper::create(const std::string& path, const std::string& data, int state) {
	char path_buffer[128];
	auto buffer_len = sizeof(path_buffer);
	auto flag = zoo_exists(m_handle, path.c_str(), 0, nullptr);
	if (flag == ZNONODE) { //该节点不存在
		flag = zoo_create(m_handle, path.c_str(), data.c_str(), data.size(),
						  &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, buffer_len);
		if (flag == ZOK) {
			std::cout << "zookeeper node create success" << std::endl;
		} else {
			std::cout << "flag::" << flag << std::endl;
			std::cout << "zookeeper node create failure" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

std::string Zookeeper::getData(const std::string& path) {
	int buffer_len = 0;
	// 先获取数据长度
	int flag = zoo_get(m_handle, path.c_str(), 0, nullptr, &buffer_len, nullptr);
	if (flag != ZOK) {
		std::cout << "get zookeeper node error" << std::endl;
		return "";
	}

	// 分配足够的空间
	std::vector<char> buffer(buffer_len);
	flag = zoo_get(m_handle, path.c_str(), 0, buffer.data(), &buffer_len, nullptr);
	if (flag != ZOK) {
		std::cout << "get zookeeper node error" << std::endl;
		return "";
	}

	return std::string(buffer.data(), buffer_len);
}


Zookeeper::~Zookeeper() {
	if (m_handle != nullptr) {
		zookeeper_close(m_handle);
	}
}