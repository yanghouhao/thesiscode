cmake_minimum_required(VERSION 2.8)
set(APP_ZCASH_SPROUT zcash-sprout)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-DMULTICORE -DCURVE_ALT_BN128 -fopenmp -O1 -DBINARY_OUTPUT -fPIC -DPERFORMANCE")
set(ZCASH_SPROUT_SOURCE .)

include_directories(${ZCASH_SPROUT_SOURCE}
                    ${ZCASH_SPROUT_SOURCE}/3rd/include
                    ${ZCASH_SPROUT_SOURCE}/rust/include
                    )
# libgmpxx.a 与 libgmp.a 的顺序不能互换
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib)
SET(LAPACK_DIR /usr/local/lib/)
SET(LAPACK_INCLUDE_DIRS /usr/local/include)
SET(LAPACK_LIBRARIES /usr/local/lib)
set(CMAKE_EXE_LINKER_FLAGS "-lm -ldl")
find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(unofficial-sodium CONFIG REQUIRED)
#add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
include_directories(${Boost_INCLUDE_DIRS})
set(ZCASH_DEPEND_LIBS  ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib/librustzcash.a
#                    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib/libmock.a
#                    ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib/libgtest.a

#                     ${ZCASH_SPROUT_SOURCE}/3rd/snark/build/lib/libsnark.a
#                      ${ZCASH_SPROUT_SOURCE}/3rd/libsodium-1.0.12/build/lib/libsodium.a
#                      ${ZCASH_SPROUT_SOURCE}/3rd/gmp-6.1.2/build/lib/libgmpxx.a
#                      ${ZCASH_SPROUT_SOURCE}/3rd/gmp-6.1.2/build/lib/libgmp.a
                      )
set(ZCASH_SPROUT_DEPEND_SRC ${ZCASH_SPROUT_SOURCE}/crypto/sha256.cpp
                             ${ZCASH_SPROUT_SOURCE}/support/cleanse.cpp
                             ${ZCASH_SPROUT_SOURCE}/uint256.cpp
                             ${ZCASH_SPROUT_SOURCE}/utilstrencodings.cpp
                             ${ZCASH_SPROUT_SOURCE}/random.cpp
                             ${ZCASH_SPROUT_SOURCE}/key_io.cpp
                             ${ZCASH_SPROUT_SOURCE}/util.cpp
                             ${ZCASH_SPROUT_SOURCE}/consensus/params.cpp
                             )
aux_source_directory(${ZCASH_SPROUT_SOURCE}/zcash APP_SRC)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/support APP_SRC2)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/secp256k1 APP_SRC3)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/script APP_SRC4)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/rust/include APP_SRC5)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/primitives APP_SRC6)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/crypto APP_SRC7)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/consensus APP_SRC8)
aux_source_directory(${ZCASH_SPROUT_SOURCE}/compat APP_SRC9)
list(REMOVE_ITEM APP_SRC ${ZCASH_SPROUT_SOURCE}/zcash/GenerateParams.cpp)
set(APP_ZCASH_SPROUT_SRC ${APP_SRC} ${APP_SRC2} ${APP_SRC3} ${APP_SRC4} ${APP_SRC5} ${APP_SRC6} ${APP_SRC7} ${APP_SRC8} ${APP_SRC9} ${ZCASH_SPROUT_DEPEND_SRC} )

MESSAGE(STATUS "APP_SRC:" ${APP_SRC})
MESSAGE(STATUS "APP_ZCASH_SPROUT_SRC:" ${APP_ZCASH_SPROUT_SRC})
MESSAGE(STATUS "ZCASH_DEPEND_LIBS:" ${ZCASH_DEPEND_LIBS})

#add_executable(${APP_ZCASH_SPROUT} ${APP_ZCASH_SPROUT_SRC} )
#
#target_link_libraries(${APP_ZCASH_SPROUT} ${ZCASH_DEPEND_LIBS}   rustzcash m dl boost_thread boost_filesystem boost_system pthread  sodium)
add_library(gtest_simple_lib STATIC
     #   ${APP_ZCASH_SPROUT_SRC}
        key.cpp    script/standard.cpp pubkey.cpp crypto/hmac_sha512.cpp consensus/params.cpp consensus/upgrades.cpp key_io.cpp base58.cpp bech32.cpp
        zcash/Address.cpp  zcash/address/zip32.cpp
        primitives/transaction.cpp proof_verifier.cpp zcash/JoinSplit.cpp zcash/address/sprout.cpp zcash/address/sapling.cpp zcash/Note.cpp zcash/NoteEncryption.cpp zcash/IncrementalMerkleTree.cpp  zcash/prf.cpp
        crypto/sha256.cpp hash.cpp  chainparams.cpp  sync.cpp
        amount.cpp  zcash/util.cpp crypto/hmac_sha256.cpp  util.cpp  random.cpp support/pagelocker.cpp support/cleanse.cpp uint256.cpp utilstrencodings.cpp  script/script.cpp utiltime.cpp clientversion.cpp
        chainparamsbase.cpp  crypto/ripemd160.cpp crypto/hmac_sha512.cpp crypto/sha512.cpp
        )