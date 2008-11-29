/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/http.server.hpp>
#include <fost/thread.hpp>


using namespace fostlib;


fostlib::http::request::request( std::auto_ptr< boost::asio::ip::tcp::socket > sock )
: m_sock( sock ) {
}
namespace {
    void first_line( boost::asio::ip::tcp::socket &sock, nullable< std::pair< string, string > > &r ) {
        if ( r.isnull() ) {
            boost::asio::basic_streambuf<> data;
            std::size_t length( boost::asio::read_until( sock, data, std::string( "\r\n" ) ) );
            data.commit( length );
            std::istream is( &data );
            std::string line;
            std::getline( is, line );
            std::pair< string, nullable< string > > parsed( partition( string( line ), L" " ) );
            r = std::make_pair( parsed.first, partition( parsed.second.value(), L" " ).first );
        }
    }
}
const string &fostlib::http::request::method() {
    first_line( *m_sock, m_first_line );
    return m_first_line.value().first;
}
const string &fostlib::http::request::file_spec() {
    first_line( *m_sock, m_first_line );
    return m_first_line.value().second;
}

void fostlib::http::request::operator() ( const mime &response ) {
    boost::asio::streambuf b;
    std::ostream os( &b );
    os << L"200 OK\r\n";
    response.print_on( os );
    std::size_t length( m_sock->send( b.data() ) );
    b.consume( length );
}


fostlib::http::server::server( const host &h, uint16_t p )
: binding( h ), port( p ), m_server( m_service, boost::asio::ip::tcp::endpoint( binding().address(), port() ) ) {
}

std::auto_ptr< http::request > fostlib::http::server::operator() () {
    std::auto_ptr< boost::asio::ip::tcp::socket > sock( new boost::asio::ip::tcp::socket( m_service ) );
    m_server.accept( *sock );
    return std::auto_ptr< http::request >( new http::request( sock ) );
}
