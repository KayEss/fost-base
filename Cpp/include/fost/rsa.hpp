#pragma once


#include <string>


namespace fostlib::rsa {


    using base64url_encoded = std::string;


    namespace PKCS1v15_SHA256 {


        /// Return signature for message signed by private key
        /// n (modulus), e (public exponent) and d (private exponent) are rsa
        /// private key component
        base64url_encoded
                sign(base64url_encoded message,
                     base64url_encoded n,
                     base64url_encoded e,
                     base64url_encoded d);

        /// Verify signature of message by public key
        /// n (modulus) and e (public exponent) are rsa private key component
        bool validate(
                base64url_encoded message,
                base64url_encoded signature,
                base64url_encoded n,
                base64url_encoded e);


    }


}
