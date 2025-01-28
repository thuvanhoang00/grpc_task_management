cmake_minimum_required(VERSION 3.8)

project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include (./common.cmake)

# Proto files
# /home/thu/LearnGRPC/grpc_real_time_chat/source
get_filename_component(hw_proto "/src/greet.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# message("Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
# message("hw_proto: ${hw_proto}")
# message("hw_proto_path: ${hw_proto_path}")
# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/../src/greet.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/../src/greet.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/../src/greet.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/../src/greet.grpc.pb.h")
# message("hw_proto_srcs: ${hw_proto_srcs}")
# message("hw_proto_hdrs: ${hw_proto_hdrs}")
# message("hw_grpc_srcs: ${hw_grpc_srcs}")
# message("hw_grpc_hdrs: ${hw_grpc_hdrs}")
# message("----------------------------------")


# Pthread
find_package(Threads REQUIRED)

# PostgreSQL
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

if(PostgreSQL_FOUND)
message(STATUS "PostgreSQL Include: ${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "PostgreSQL Libraries: ${PostgreSQL_LIBRARIES}")
endif()

message("----------------------------------")


# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src/")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include/")
include_directories("${PostgreSQL_INCLUDE_DIRS} ${PQXX_INCLUDE_DIRS}")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${PostgreSQL_LIBRARIES}
  ${PQXX_LIBRARIES})

# Targets (client|server)
foreach(_target
  client server)
  add_executable(
    ${_target} 
    "../src/${_target}.cpp" 
    "../src/task.cpp"
    "../src/sqlmanager.cpp"
    "../src/admin.cpp"
    "../src/user.cpp"
    "../src/requesthandler.cpp"
    )

  target_link_libraries(${_target}
    hw_grpc_proto
    absl::check
    absl::flags
    absl::flags_parse
    absl::log
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    Threads::Threads)
endforeach()
