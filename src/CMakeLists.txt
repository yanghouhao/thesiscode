cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-DMULTICORE -DCURVE_ALT_BN128 -fopenmp -O1 -DBINARY_OUTPUT -fPIC -DPERFORMANCE")
set(TJFOC_SPROUT_SOURCE .)
include_directories(${TJFOC_SPROUT_SOURCE}
                    ${TJFOC_SPROUT_SOURCE}/3rd/include
                    ${TJFOC_SPROUT_SOURCE}/rust/include
                    )
# libgmpxx.a 与 libgmp.a 的顺序不能互换
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/lib)
find_package(Boost 1.74.0 COMPONENTS system filesystem thread REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

#add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
include_directories(${BOOST_INCLUDE_DIRS})
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${Boost_LIBRARIES})
add_library(gtest_lib STATIC
        key.cpp    script/standard.cpp pubkey.cpp crypto/hmac_sha512.cpp consensus/params.cpp consensus/upgrades.cpp   key_io.cpp base58.cpp bech32.cpp
        zcash/Address.cpp  zcash/address/zip32.cpp
        primitives/transaction.cpp proof_verifier.cpp zcash/JoinSplit.cpp zcash/address/sprout.cpp zcash/address/sapling.cpp zcash/Note.cpp zcash/NoteEncryption.cpp zcash/IncrementalMerkleTree.cpp  zcash/prf.cpp
        crypto/sha256.cpp hash.cpp  chainparams.cpp  sync.cpp
        amount.cpp  zcash/util.cpp crypto/hmac_sha256.cpp  util.cpp  random.cpp support/pagelocker.cpp support/cleanse.cpp uint256.cpp utilstrencodings.cpp  script/script.cpp utiltime.cpp clientversion.cpp
        chainparamsbase.cpp  crypto/ripemd160.cpp crypto/hmac_sha512.cpp crypto/sha512.cpp
        univalue/univalue.cpp univalue/univalue_read.cpp univalue/univalue_write.cpp univalue/univalue_get.cpp
        )
