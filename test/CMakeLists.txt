cmake_minimum_required(VERSION 3.10)
enable_testing()

find_package(GTest CONFIG REQUIRED)#查找GTest
find_package(Threads)#查找Threads
set(ZCASH_SPROUT_SOURCE ../src)
link_directories(${ZCASH_SPROUT_SOURCE}/3rd/lib)
#一些调试信息
message(gtest_include_dir=${GTEST_INCLUDE_DIR})
message(gtest_library=${GTEST_LIBRARY})
message(threads=${CMAKE_THREAD_LIBS_INIT})
message(rust=${ZCASH_SPROUT_SOURCE}/3rd/lib)
#包含GTest的库文件
include_directories(${GTEST_INCLUDE_DIR})
#包含src的项目文件
set(ZCASH_SPROUT_SOURCE ../src)
include_directories(../
        ${ZCASH_SPROUT_SOURCE}
        ${ZCASH_SPROUT_SOURCE}/3rd/include
        ${ZCASH_SPROUT_SOURCE}/rust/include)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/zcash APP_SRC)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/support APP_SRC2)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/secp256k1 APP_SRC3)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/script APP_SRC4)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/rust/include APP_SRC5)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/primitives APP_SRC6)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/crypto APP_SRC7)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/consensus APP_SRC8)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/compat APP_SRC9)
set(APP_ZCASH_SPROUT_SRC  ${APP_SRC} ${APP_SRC2} ${APP_SRC3} ${APP_SRC4} ${APP_SRC5} ${APP_SRC6} ${APP_SRC7} ${APP_SRC8} ${APP_SRC9}  )
find_package(OpenSSL REQUIRED)
find_package(unofficial-secp256k1 CONFIG REQUIRED)
message(APP_ZCASH_SPROUT_SRC=${APP_ZCASH_SPROUT_SRC})
add_executable(gtest_simple_test main.cpp test_joinsplit.cpp utils.cpp   )
set(gtest_simple_lib   /mnt/f/github.com/zcash-sprout/src/cmake-build-debug/gtest_simple_lib.a)
target_link_libraries(gtest_simple_test PUBLIC
        gtest_simple_lib ${GTEST_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
        rustzcash m dl boost_thread boost_system boost_filesystem -lboost_program_options -lboost_chrono -lboost_prg_exec_monitor -lboost_test_exec_monitor -lboost_unit_test_framework   sodium  PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(gtest_simple_test PRIVATE unofficial::secp256k1)
add_test(NAME gtest_simple_test COMMAND gtest_simple_test)

