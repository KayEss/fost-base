#pragma once


#include <felspar/memory.hpp>
#include <fost/array>
#include <fost/json>
#include <fost/string>


namespace fostlib::ed25519 {


    /// A public or private key or other secret
    using secret = std::array<felspar::byte, 32>;


    /// A keypair for ed25519
    class keypair {
        /// The way the library we use thinks about the keys is that
        /// there is a public key (32 bytes) and a secret key (64 bytes).
        /// The secret is of course 32 bytes, but it's got the public key
        /// appended to it (!!). Lots of the library implementations
        /// rely on thiis :(
        ///
        /// We'll just store the two parts in a single 64 byte array
        /// and call it done. The `priv` and `pub` accessors return
        /// the correct parts.
        std::array<felspar::byte, 64> privkey;

      public:
        /// Create a new keypair
        explicit keypair();
        /// Create a keypair from a secret
        explicit keypair(secret const &);
        /// Create a keypair from a memory buffer
        keypair(felspar::buffer<const felspar::byte>);

        /// Return the secret and private parts
        secret priv() const {
            secret s;
            std::copy(privkey.data(), privkey.data() + 32, s.begin());
            return s;
        }
        secret pub() const {
            secret s;
            std::copy(privkey.data() + 32, privkey.data() + 64, s.begin());
            return s;
        }

        /// Return a signature for the presented data
        std::array<felspar::byte, 64>
                sign(felspar::buffer<const felspar::byte> data) const;

        /// Allow conversion of the keypair to a memory buffer
        operator felspar::buffer<const felspar::byte>() const {
            return privkey;
        }
    };


    /// Verify a signed message has been signed with the provided public key
    bool
            verify(felspar::buffer<const felspar::byte> pub,
                   felspar::buffer<const felspar::byte> msg,
                   felspar::buffer<const felspar::byte> sig);

    inline bool
            verify(felspar::buffer<const felspar::byte> pub,
                   const std::string &msg,
                   felspar::buffer<const felspar::byte> sig) {
        return verify(
                pub,
                felspar::buffer<const felspar::byte>{
                        reinterpret_cast<unsigned char const *>(msg.data()),
                        msg.size()},
                sig);
    }


}


namespace fostlib {


    /// Convert secrets to JSON and back
    template<>
    struct coercer<ed25519::secret, base64_string> {
        ed25519::secret coerce(base64_string const &s) {
            auto vec = fostlib::coerce<std::vector<unsigned char>>(s);
            if (vec.size() != 32) {
                throw fostlib::exceptions::out_of_range<std::size_t>{
                        "Secret base64 must produce exactly 32 bytes", 32, 32,
                        vec.size()};
            }
            ed25519::secret key;
            std::copy(vec.begin(), vec.end(), key.begin());
            return key;
        }
    };
    template<>
    struct coercer<ed25519::secret, json> {
        ed25519::secret coerce(json const &j) {
            return fostlib::coerce<ed25519::secret>(
                    fostlib::coerce<base64_string>(j));
        }
    };


}
