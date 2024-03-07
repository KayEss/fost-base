#include "fost-crypto.hpp"
#include <fost/crypto.hpp>
#include <fost/ed25519.hpp>

#include "naclite.h"


namespace {


    auto signing_pubkey(felspar::buffer<const felspar::byte> sk) {
        // Derived from crypto_sign_keypair in Crypto++ tweetnacl.cpp
        uint8_t d[64];
        fostlib::nacl::gf p[4];

        fostlib::nacl::crypto_hash(
                d, reinterpret_cast<const uint8_t *>(sk.data()), sk.size());
        d[0] &= 248;
        d[31] &= 127;
        d[31] |= 64;
        fostlib::nacl::scalarbase(p, d);

        fostlib::ed25519::secret pk;
        fostlib::nacl::pack(reinterpret_cast<uint8_t *>(pk.data()), p);
        return pk;
    }


}


fostlib::ed25519::keypair::keypair() : keypair(crypto_bytes<32>()) {}


fostlib::ed25519::keypair::keypair(secret const &sk) {
    std::copy(sk.begin(), sk.end(), privkey.begin());
    auto pk = signing_pubkey(priv());
    std::copy(pk.begin(), pk.end(), privkey.begin() + 32);
}


fostlib::ed25519::keypair::keypair(felspar::buffer<const felspar::byte> sk) {
    if (sk.size() == secret{}.size()) {
        std::copy(sk.begin(), sk.end(), privkey.begin());
        auto pk = signing_pubkey(priv());
        std::copy(pk.begin(), pk.end(), privkey.begin() + 32);
    } else if (sk.size() != privkey.size()) {
        throw exceptions::out_of_range(
                "Buffer passed for Ed26619 private/public key pair must be 64 "
                "or 32 bytes",
                privkey.size(), privkey.size(), sk.size());
    } else {
        std::copy(sk.begin(), sk.end(), privkey.begin());
    }
}


std::array<felspar::byte, 64> fostlib::ed25519::keypair::sign(
        felspar::buffer<const felspar::byte> message) const {
    std::vector<felspar::byte> signature(message.size() + 64);
    uint64_t siglen{signature.size()};
    fostlib::nacl::crypto_sign(
            signature.data(), &siglen, message.data(), message.size(),
            privkey.data());
    if (siglen < 64)
        throw exceptions::not_implemented{
                "Unexpected ed25519 signature length - too short", siglen};
    std::array<felspar::byte, 64> ret;
    std::copy(signature.begin(), signature.begin() + 64, ret.begin());
    return ret;
}


bool fostlib::ed25519::verify(
        felspar::buffer<const felspar::byte> pub,
        felspar::buffer<const felspar::byte> msg,
        felspar::buffer<const felspar::byte> sig) {
    /// Because NaCl has a stupid API we have to build it's message format out
    /// of the parts we have before we can do anything else
    std::vector<felspar::byte> msgsig(msg.size() + sig.size());
    std::copy(sig.begin(), sig.end(), msgsig.begin());
    std::copy(msg.begin(), msg.end(), msgsig.begin() + sig.size());
    /// We also need a buffer for NaCl to give us the message back in (!) so
    /// that we can ignore it because we actually got handed the message as a
    /// parameter. Note that the outmsg allocation is larger than might be
    /// expected as the NaCl documentation tells us we must allocate enough for
    /// the message and the secret (for some reason).
    std::vector<felspar::byte> outmsg(msgsig.size());
    uint64_t outmsg_len{};
    if (fostlib::nacl::crypto_sign_open(
                outmsg.data(), &outmsg_len, msgsig.data(), msgsig.size(),
                pub.data())
        == 0) {
        if (outmsg_len != msg.size()) {
            /// If this check is ever false then probably we're in  a lot of
            /// trouble, especially if the output buffer is  now longer than the
            /// one we allocated
            throw exceptions::not_implemented{
                    "Something truly awful happened and the message somehow "
                    "grew"};
        }
        return true;
    } else {
        return false;
    }
}
