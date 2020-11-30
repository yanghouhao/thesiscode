cmake_minimum_required(VERSION 3.0)

find_package(OpenSSL REQUIRED)
 
# define the project
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(MODEL_FILES ../model/myBlockChain.cpp)
set(SOURCE_FILES server.cpp)
add_executable(test_server ${SOURCE_FILES} ${MODEL_FILES}) 

# set the POCO paths and libs
set(POCO_PREFIX      "/usr/local")
set(POCO_INCLUDE_DIR "${POCO_PREFIX}/include")
set(POCO_LIB_DIR     "${POCO_PREFIX}/lib")
set(POCO_LIBS        "${POCO_LIB_DIR}/libPocoNet.so"
                     "${POCO_LIB_DIR}/libPocoUtil.so"
                     "${POCO_LIB_DIR}/libPocoFoundation.so")

#target_include_directories(test_server PRIVATE "${POCO_INCLUDE_DIR}")
target_link_libraries(test_server PocoNet PocoUtil PocoFoundation OpenSSL::SSL OpenSSL::Crypto -lrelic -lrelic_ec -lopenabe)