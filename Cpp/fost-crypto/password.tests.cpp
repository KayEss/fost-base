/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>


FSL_TEST_SUITE(password);


FSL_TEST_FUNCTION(pbkdf2_hmac_sha256) {
    fostlib::utf8_string key("passwd");
    std::array<unsigned char, 4> salt{'s', 'a', 'l', 't'};
    std::array<unsigned char, 64> expected = {
        0x55, 0xac, 0x04, 0x6e, 0x56, 0xe3, 0x08, 0x9f,
        0xec, 0x16, 0x91, 0xc2, 0x25, 0x44, 0xb6, 0x05,
        0xf9, 0x41, 0x85, 0x21, 0x6d, 0xde, 0x04, 0x65,
        0xe6, 0x8b, 0x9d, 0x57, 0xc2, 0x0d, 0xac, 0xbc,
        0x49, 0xca, 0x9c, 0xcc, 0xf1, 0x79, 0xb6, 0x45,
        0x99, 0x16, 0x64, 0xb3, 0x9d, 0x77, 0xef, 0x31,
        0x7c, 0x71, 0xb8, 0x45, 0xb1, 0xe3, 0x0b, 0xd5,
        0x09, 0x11, 0x20, 0x41, 0xd3, 0xa1, 0x97, 0x83
    };
    auto res = fostlib::pbkdf2_hmac_sha256(key, salt, 1u);
    FSL_CHECK(res == expected);
}

