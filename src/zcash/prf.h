/*
Zcash uses SHA256Compress as a PRF for various components
within the zkSNARK circuit.
*/

#ifndef ZC_PRF_H_
#define ZC_PRF_H_

#include "uint256.h"
#include "uint252.h"

#include <array>

//! Sprout functions
uint256 PRF_addr_a_pk(const uint252& a_sk);
uint256 PRF_addr_sk_enc(const uint252& a_sk);
uint256 PRF_nf(const uint252& a_sk, const uint256& rho);
uint256 PRF_pk(const uint252& a_sk, size_t i0, const uint256& h_sig);
uint256 PRF_rho(const uint252& phi, size_t i0, const uint256& h_sig);

//! Sapling functions
uint256 PRF_ask(const uint256& sk);
uint256 PRF_nsk(const uint256& sk);
uint256 PRF_ovk(const uint256& sk);
uint256 PRF_rcm(const uint256& rseed);
uint256 PRF_esk(const uint256& rseed);

const char PRF_ASK_TAG = 0;
const char PRF_NSK_TAG = 1;
const char PRF_OVK_TAG = 2;
const char PRF_RCM_TAG = 4;
const char PRF_ESK_TAG = 5;

std::array<unsigned char, 11> default_diversifier(const uint256& sk);

#endif // ZC_PRF_H_
