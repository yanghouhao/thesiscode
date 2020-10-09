cmake_minimum_required(VERSION 3.10)
enable_testing()

find_package(GTest CONFIG REQUIRED)#查找GTest
find_package(Threads)#查找Threads
set(TJFOC_SPROUT_SOURCE ../src)
link_directories(${TJFOC_SPROUT_SOURCE}/3rd/lib)
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

add_executable(gtest_test main.cpp test_joinsplit.cpp test_merkletree.cpp test_pedersen_hash.cpp test_random.cpp test_transaction.cpp
       test_txid.cpp utils.cpp json_test_vectors.cpp   )

target_link_libraries(gtest_test PUBLIC
        gtest_lib  ${CMAKE_THREAD_LIBS_INIT} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
        rustzcash m dl boost_thread boost_system boost_filesystem -lboost_program_options -lboost_chrono -lboost_prg_exec_monitor -lboost_test_exec_monitor -lboost_unit_test_framework   sodium  PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(gtest_test PRIVATE unofficial::secp256k1)
add_test(NAME gtest_test COMMAND gtest_test)

