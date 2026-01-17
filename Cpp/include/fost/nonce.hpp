#pragma once


#include <fost/string>


namespace fostlib {


    /// Produce base64url nonces of various byte sizes
    base64_string nonce8b64u();
    base64_string nonce24b64u();
    base64_string nonce32b64u();


    /// Produce base64url nonces with the current time as a prefix
    base64_string timestamp_nonce8b64u();
    base64_string timestamp_nonce24b64u();
    base64_string timestamp_nonce32b64u();


}
