set(RPC_SRC_LIST
        RpcProvider.cpp
        RpcChannel.cpp
        RpcController.cpp
        ${CMAKE_SOURCE_DIR}/src/proto/rpc_header.pb.cc
        ${CMAKE_SOURCE_DIR}/src/utils/Logger.cpp
        ${CMAKE_SOURCE_DIR}/src/utils/Config.cpp
        ${CMAKE_SOURCE_DIR}/src/utils/HvProtocol.cpp
)

add_library(tinyrpc ${RPC_SRC_LIST})
target_link_libraries(tinyrpc hv pthread)
target_include_directories(tinyrpc PRIVATE ${CMAKE_SOURCE_DIR}/src)
