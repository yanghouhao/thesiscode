cmake_minimum_required(VERSION 3.10)
enable_testing()

find_package(GTest CONFIG REQUIRED)#查找GTest
find_package(Threads)#查找Threads
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

find_package(OpenSSL REQUIRED)
find_package(unofficial-secp256k1 CONFIG REQUIRED)

#add_executable(test_model test_model.cpp)

file(GLOB Sources "*.cpp")
add_library(model STATIC ${Sources})


