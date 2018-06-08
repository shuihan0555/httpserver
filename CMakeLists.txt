cmake_minimum_required(VERSION 2.8)
project(marketsolutiongateway)
find_package(gflags REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11  -DKEEP_ALIVE_TEST -DDEBUG -DFUNCTIONTIME")


include_directories(${PROJECT_SOURCE_DIR}/http-parser)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../xflagger/include)

include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/config/tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/market/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/include)
include_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/include)

set(SOURCE_FILE main.cc
	${PROJECT_SOURCE_DIR}/../comm-lib/config/tnode/src/tnodeconfig_v0.cc
	${PROJECT_SOURCE_DIR}/../xflagger/src/xconfig.cc
	src/marketsolution_config.cc
	http-parser/http_parser.cc
	src/tnode_adapter.cc
	src/http_request.cc
	src/connection.cc)


link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/market/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/tnode/lib)
link_directories(${PROJECT_SOURCE_DIR}/../comm-lib/xlogger/lib)


add_executable(marketsolutiongateway ${SOURCE_FILE})

#target_link_libraries(HttpSolutionGateway tnode marketagent xlogger mysqlop redisop hiredis uv amqp-cpp ev gflags glog pthread mysqlclient)
target_link_libraries(marketsolutiongateway tnode marketagent xlogger uv amqp-cpp ev gflags glog pthread )
