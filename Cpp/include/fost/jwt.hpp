#pragma once


#include <fost/crypto.hpp>
#include <fost/datetime>
#include <fost/insert>


namespace fostlib {


    namespace jws {


        /// The digest algorithms that are supported
        enum class alg { HS256, EdDSA };


        /// Low level API for signing the header and payload BASE64 encoded
        /// parts of the JWT and returning the signed version
        [[nodiscard]] std::string sign_base64_string(
                felspar::u8view header_b64,
                felspar::u8view payload_b64,
                alg,
                felspar::buffer<const felspar::byte> key);


    }


    namespace jwt {


        /// Support the same algorithms as JWS does
        using alg = jws::alg;


        /// The encryption algorithms that are supported
        enum encryption {};


        /// Create a JWT
        class mint {
            alg algorithm;
            json header, m_payload;

          public:
            /// Set up the parameters used for creating the JWT
            mint(alg, json payload = json::object_t{});

            /// Make movable
            mint(mint &&);

            /// Set the subject claim
            mint &subject(const string &);

            /// Set the token to expire after this amount of time
            template<typename R, typename P>
            std::chrono::system_clock::time_point
                    expires(std::chrono::duration<R, P> const tp,
                            bool const add_issued_claim = true) {
                auto const now = std::chrono::system_clock::now();
                auto const exp = now + tp;
                if (add_issued_claim) {
                    insert(m_payload, "iss",
                           std::chrono::system_clock::to_time_t(now));
                }
                insert(m_payload, "exp",
                       std::chrono::system_clock::to_time_t(exp));
                return exp;
            }

            /// Set a claim. If the claim name is not listed at
            /// http://www.iana.org/assignments/jwt/jwt.xhtml then
            /// it should be a URL. See RFC7519
            mint &claim(felspar::u8view url, const json &value);

            /// Return the token
            std::string token(felspar::buffer<const felspar::byte> key);

            /// Return the current payload
            const json &payload() const { return m_payload; }
        };


        /// Check a JWT
        struct token {
            /// Load the token with a secret returned by the lambda
            static nullable<token> load(
                    felspar::u8view jwt,
                    const std::function<std::vector<felspar::byte>(json, json)>
                            &lambda);
            /// Load the token and return it if verified
            static nullable<token> load(string secret, felspar::u8view jwt) {
                return load(jwt, [secret = std::move(secret)](json, json) {
                    return std::vector<felspar::byte>(
                            secret.data().begin(), secret.data().end());
                });
            }
            /// The token header and payload
            const json header, payload;
        };


    }


}
