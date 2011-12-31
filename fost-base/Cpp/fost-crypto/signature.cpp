/*
    Copyright 1999-2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/detail/crypto.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>

#include <crypto++/hmac.h>
#include <crypto++/sha.h>


using namespace fostlib;


string fostlib::sha1_hmac( const string &key, const string &data ) {
    BOOST_STATIC_ASSERT(sizeof(std::size_t)>=sizeof(int));
    utf8_string key_utf8( coerce< utf8_string >( key ) ), data_utf8( coerce< utf8_string >( data ) );

    boost::array< unsigned char, CryptoPP::SHA1::DIGESTSIZE > signature;
    CryptoPP::HMAC<CryptoPP::SHA1> hmac;
    hmac.SetKey(
        reinterpret_cast<const unsigned char *>(key_utf8.underlying().c_str()),
        key_utf8.underlying().length());
    hmac.Update(
        reinterpret_cast<const unsigned char *>(data_utf8.underlying().c_str()),
        data_utf8.underlying().length());
    hmac.Final(signature.data());

     return coerce< string >(
         coerce< base64_string >(
             std::vector< unsigned char >(
                 signature.data(), signature.data() + CryptoPP::SHA1::DIGESTSIZE ) ) );
}


struct fostlib::hmac::impl : boost::noncopyable {
    virtual ~impl() {
    }

    static void check(fostlib::hmac::impl *i) {
        if ( !i ) throw fostlib::exceptions::null("This hmac has not been properly initialised");
    }
};
template< typename H >
struct hash_impl : public fostlib::hmac::impl {
    CryptoPP::HMAC< H > hmac;
};


fostlib::hmac::hmac( string (*hash)( const string & ), const string &key )
: m_implementation( NULL ) {
//     if ( hash == fostlib::sha1 )
//         m_implementation = new impl(EVP_sha1(), key);
//     else if ( hash == fostlib::md5 )
//         m_implementation = new impl(EVP_md5(), key);
//     else
//         throw fostlib::exceptions::not_implemented( "fostlib::hmac::hmac( string (*digest_function)( const string & ), const string &key ) -- Only sha1 and md5 are supported" );
}
fostlib::hmac::~hmac() {
    if ( m_implementation) delete m_implementation;
}


std::vector< unsigned char > fostlib::hmac::digest() const {
    impl::check(m_implementation);
//     unsigned char signature[EVP_MAX_MD_SIZE] = {0};
//     unsigned int signature_length = 0;
//     HMAC_Final(&m_implementation->ctx, signature, &signature_length);
//     return std::vector< unsigned char >( signature, signature + signature_length );
    throw fostlib::exceptions::not_implemented(
        "std::vector< unsigned char > fostlib::hmac::digest() const");
}


fostlib::hmac &fostlib::hmac::operator << ( const const_memory_block &p ) {
    const unsigned char
        *begin = reinterpret_cast< const unsigned char * >( p.first ),
        *end =  reinterpret_cast< const unsigned char * >( p.second )
    ;
    std::size_t length = end - begin;
    if ( length > std::size_t(std::numeric_limits< int >::max()) )
        throw exceptions::out_of_range< uint64_t >( L"Message data is too long", 0, std::numeric_limits< int >::max(), length );
//     else if ( length )
//         HMAC_Update(&m_implementation->ctx, begin, static_cast< int >( length ) );
    return *this;
}
fostlib::hmac &fostlib::hmac::operator << ( const fostlib::utf8_string &data_utf8 ) {
    return (*this) << const_memory_block( data_utf8.underlying().c_str(), data_utf8.underlying().c_str() + data_utf8.underlying().length() );
}

fostlib::hmac &fostlib::hmac::operator << ( const fostlib::string &data ) {
    return *this << coerce< utf8_string >( data );
}

fostlib::hmac &fostlib::hmac::operator << ( const boost::filesystem::wpath &filename ) {
    throw fostlib::exceptions::not_implemented( "fostlib::hmac::operator << ( const boost::filesystem::wpath &filename )" );
}

