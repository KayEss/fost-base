#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/crypto.hpp>
#include <fost/exception/out_of_range.hpp>

#include <crypto++/keccak.h>
#include <crypto++/ripemd.h>
#include <crypto++/sha.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>


using namespace fostlib;


namespace {
    template<typename H, std::size_t N>
    inline void hash_into(
            std::span<const unsigned char> data,
            std::span<unsigned char, N> into) {
        static_assert(H::DIGESTSIZE == N);
        H().CalculateDigest(
                into.data(),
                reinterpret_cast<const CryptoPP::byte *>(data.data()),
                data.size());
    }
    template<typename H>
    inline string hash(const felspar::u8view &text) {
        std::array<unsigned char, H::DIGESTSIZE> result;
        hash_into<H>(
                std::span<const unsigned char>{
                        text.memory().data(), text.memory().size()},
                std::span<unsigned char, H::DIGESTSIZE>{
                        result.data(), result.size()});
        return coerce<string>(coerce<hex_string>(result));
    }
    template<typename H>
    inline string hash(std::filesystem::path const &fn) {
        std::array<unsigned char, H::DIGESTSIZE> result;
        std::vector<unsigned char> d(std::filesystem::file_size(fn));
        std::ifstream{fn, std::ios::binary}.read(
                reinterpret_cast<char *>(d.data()), d.size());
        hash_into<H>(
                std::span<unsigned char const>{d.data(), d.size()},
                std::span<unsigned char, H::DIGESTSIZE>{
                        result.data(), result.size()});
        return coerce<string>(coerce<hex_string>(result));
    }
}


fostlib::string fostlib::keccak256(felspar::u8view const &text) {
    return hash<CryptoPP::Keccak_256>(text);
}


string fostlib::md5(const felspar::u8view &text) {
    return hash<CryptoPP::Weak::MD5>(text);
}


void fostlib::sha1_into(
        std::span<const unsigned char> data,
        std::span<unsigned char, 20> into) {
    hash_into<CryptoPP::SHA1>(data, into);
}
string fostlib::sha1(const felspar::u8view &text) {
    return hash<CryptoPP::SHA1>(text);
}


string fostlib::sha256(const felspar::u8view &text) {
    return hash<CryptoPP::SHA256>(text);
}

string fostlib::sha512(const felspar::u8view &text) {
    return hash<CryptoPP::SHA512>(text);
}
string fostlib::sha512(std::filesystem::path const &fn) {
    return hash<CryptoPP::SHA512>(fn);
}


string fostlib::ripemd256(const felspar::u8view &text) {
    return hash<CryptoPP::RIPEMD256>(text);
}
