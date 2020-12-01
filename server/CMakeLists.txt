cmake_minimum_required(VERSION 3.0)

find_package(OpenSSL REQUIRED)
find_package(unofficial-secp256k1 CONFIG REQUIRED)

set(TJFOC_SPROUT_SOURCE ../src)
link_directories(${TJFOC_SPROUT_SOURCE}/3rd/lib)
link_directories(
	${ZROOT}/deps/root/lib 
	${ZROOT}/root/lib)
#一些调试信息
message(threads=${CMAKE_THREAD_LIBS_INIT})
message(rust=${TJFOC_SPROUT_SOURCE}/3rd/lib)
#包含src的项目文件
set(TJFOC_SPROUT_SOURCE ../src)
include_directories(../
        ${TJFOC_SPROUT_SOURCE}
        ${TJFOC_SPROUT_SOURCE}/3rd/include
        ${TJFOC_SPROUT_SOURCE}/rust/include)

# define the project
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
LINK_DIRECTORIES(../model)
include_directories(../model
)
set(MODEL_FILES myModel)
aux_source_directory(. SRC_LIST)
add_executable(test_server server.cpp server.h) 

target_link_libraries(test_server PUBLIC
        gtest_lib  ${CMAKE_THREAD_LIBS_INIT} PRIVATE 
        rustzcash m dl -lboost_thread -lboost_system -lboost_filesystem -lboost_program_options -lboost_chrono -lboost_prg_exec_monitor -lboost_test_exec_monitor -lboost_unit_test_framework   sodium  PRIVATE OpenSSL::SSL OpenSSL::Crypto -lrelic -lrelic_ec -lopenabe
        PocoNet PocoUtil PocoFoundation)
target_link_libraries(test_server PRIVATE unofficial::secp256k1)