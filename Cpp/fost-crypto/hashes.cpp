/**
    Copyright 2007-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/crypto.hpp>
#include <fost/exception/out_of_range.hpp>

#include <crypto++/keccak.h>
#include <crypto++/ripemd.h>
#include <crypto++/sha.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>


namespace {
    template<typename H>
    inline fostlib::string hash(const f5::u8view &text) {
        boost::array<unsigned char, H::DIGESTSIZE> result;
        H().CalculateDigest(
                result.data(),
                reinterpret_cast<const CryptoPP::byte *>(text.data()),
                text.bytes());
        return fostlib::coerce<fostlib::string>(
                fostlib::coerce<fostlib::hex_string>(result));
    }
}


fostlib::string fostlib::keccak256(const f5::u8view &text) {
    return hash<CryptoPP::Keccak_256>(text);
}


fostlib::string fostlib::md5(const f5::u8view &text) {
    return hash<CryptoPP::Weak::MD5>(text);
}


fostlib::string fostlib::sha1(const f5::u8view &text) {
    return hash<CryptoPP::SHA1>(text);
}


fostlib::string fostlib::sha256(const f5::u8view &text) {
    return hash<CryptoPP::SHA256>(text);
}


fostlib::string fostlib::ripemd256(const f5::u8view &text) {
    return hash<CryptoPP::RIPEMD256>(text);
}
