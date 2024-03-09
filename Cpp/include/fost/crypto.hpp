#ifndef FOST_CRYPTO_HPP
#define FOST_CRYPTO_HPP
#pragma once


#include <fost/array>
#include <fost/filesystem.hpp>
#include <fost/pointers>

#include <fstream>
#include <span>

// TODO Older libc6-dev packages don't provide this header :(
// This needs to be fixed using C++17's `__has_include`
// #include <sys/random.h>


namespace fostlib {


    extern const module c_fost_crypto;


    /// Constant time comparison of two memory buffers
    bool crypto_compare(
            array_view<const unsigned char> left,
            array_view<const unsigned char> right);
    /// We want to be able to do this with std::string instances
    inline bool
            crypto_compare(const std::string &left, const std::string &right) {
        return crypto_compare(
                array_view<const unsigned char>(
                        reinterpret_cast<const unsigned char *>(left.c_str()),
                        left.length()),
                array_view<const unsigned char>(
                        reinterpret_cast<const unsigned char *>(right.c_str()),
                        right.length()));
    }
    /// Allow comparison of strings
    inline bool crypto_compare(felspar::u8view left, felspar::u8view right) {
        return crypto_compare(
                array_view<const unsigned char>(left),
                array_view<const unsigned char>(right));
    }
    /// Allow us to compare tagged string
    template<typename T, typename U>
    inline bool crypto_compare(
            const tagged_string<T, U> &left, const tagged_string<T, U> &right) {
        return crypto_compare(
                static_cast<felspar::u8view>(left),
                static_cast<felspar::u8view>(right));
    }


    /// Return the requested number of cryptographically secure random bytes
    template<std::size_t N>
    std::array<felspar::byte, N> crypto_bytes() {
        std::array<felspar::byte, N> buffer;
        std::ifstream urandom("/dev/urandom");
        urandom.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
        // TODO Until we can check for the presence of the sys/random.h
        // header we have to use the slower implementation
        // for ( auto n = N; n > 0; n = n - getrandom(buffer.data() + (N - n),
        // n, 0) );
        return buffer;
    }


    /// ## Cryptographic hashing functions
    FOST_CRYPTO_DECLSPEC string keccak256(const felspar::u8view &);
    FOST_CRYPTO_DECLSPEC string md5(const felspar::u8view &);
    FOST_CRYPTO_DECLSPEC string sha1(const felspar::u8view &);
    FOST_CRYPTO_DECLSPEC string sha256(const felspar::u8view &);
    FOST_CRYPTO_DECLSPEC string ripemd256(const felspar::u8view &);

    void sha1_into(
            std::span<const unsigned char> data,
            std::span<unsigned char, 20> into);


    /// The type of a digester used as an argument
    using digester_fn = string (*)(const felspar::u8view &);


    /// Generic digester for hash algorithms.
    class FOST_CRYPTO_DECLSPEC digester {
      public:
        /// Construct the digester from the wanted digest function
        digester(digester_fn);
        /// Make movable
        digester(digester &&);
        ~digester();

        digester &operator<<(const const_memory_block &);
        digester &operator<<(const std::vector<unsigned char> &v) {
            if (v.size()) {
                const unsigned char *begin = v.data();
                return *this << const_memory_block(begin, begin + v.size());
            } else {
                return *this;
            }
        }
        template<std::size_t N>
        digester &operator<<(std::array<std::byte, N> const &a) {
            return *this << const_memory_block(a.data(), a.data() + a.size());
        }

        digester &operator<<(felspar::u8view);
        digester &operator<<(std::string_view sv) {
            return *this << const_memory_block{
                           sv.data(), sv.data() + sv.size()};
        }
        digester &operator<<(string const &str) {
            return *this << felspar::u8view{str};
        }
        template<std::size_t N>
        digester &operator<<(char const (&s)[N]) {
            return *this << felspar::u8view{s};
        }
        digester &operator<<(std::filesystem::path const &filename);

        std::vector<unsigned char> digest() const;

        struct impl;

      private:
        std::unique_ptr<impl> m_implementation;
    };


    /// Signatures
    FOST_CRYPTO_DECLSPEC
    string sha1_hmac(const string &key, const string &data);

    class FOST_CRYPTO_DECLSPEC hmac {
      public:
        /// Construct a HMAC with the given digest and secret
        hmac(digester_fn, felspar::buffer<const felspar::byte>);
        hmac(digester_fn d, const string &key)
        : hmac(d, felspar::buffer<const felspar::byte>{felspar::u8view{key}}) {}
        hmac(digester_fn d, const void *key, std::size_t key_length)
        : hmac(d,
               felspar::buffer<const felspar::byte>(
                       reinterpret_cast<unsigned char const *>(key),
                       key_length)) {}
        template<std::size_t N>
        hmac(digester_fn digest_function, const std::array<unsigned char, N> &s)
        : hmac(digest_function,
               reinterpret_cast<const void *>(s.data()),
               s.size()) {}
        /// Make movable
        hmac(hmac &&);
        ~hmac();

        hmac &operator<<(const const_memory_block &);
        hmac &operator<<(const std::vector<unsigned char> &v) {
            if (v.size()) {
                const unsigned char *begin = v.data();
                return *this << const_memory_block(begin, begin + v.size());
            } else {
                return *this;
            }
        }
        hmac &operator<<(fostlib::nliteral n) {
            return *this << fostlib::utf8_string(n);
        }
        hmac &operator<<(felspar::u8view);
        hmac &operator<<(std::filesystem::path const &filename);

        std::vector<unsigned char> digest() const;

        struct impl;

      private:
        std::unique_ptr<impl> m_implementation;
    };


}


#endif // FOST_CRYPTO_HPP
