/**
  ******************************************************************************
  * @file           : RpcController.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/22
  ******************************************************************************
  */

#include "RpcController.h"

void RpcController::SetFailed(const std::string &reason) {
	is_fail = true;
	fail_text = reason;
}

bool RpcController::Failed() const {
	return is_fail;
}

std::string RpcController::ErrorText() const {
	return fail_text;
}

void RpcController::Reset() {
	is_fail = false;
	fail_text.clear();
}
