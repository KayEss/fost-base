/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/string/utility.hpp>


FSL_TEST_SUITE( string );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::test::default_copy_constructable< fostlib::string >();
    FSL_CHECK_EQ( fostlib::string( L"aaa" ), fostlib::string( "aaa" ) );
    FSL_CHECK_EQ( fostlib::string( 3, 'a' ), fostlib::string( L"aaa" ) );
}


FSL_TEST_FUNCTION( access ) {
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).at( 1 ), 'e' );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).at( 4 ), 'o' );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).at( 5 ), 0x2014 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).at( 6 ), 'w' );

    FSL_CHECK_EQ( fostlib::string( L"\x5b6b" ).at( 0 ), 0x5b6b );
    FSL_CHECK_EQ( fostlib::string( L"\xd834\xdd1e" ).at( 0 ), 0x1d11e );
}


FSL_TEST_FUNCTION( lengths ) {
    FSL_CHECK_EQ( fostlib::string( L"aaa" ).length(), 3 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).length(), 12 );
    if ( sizeof( wchar_t ) == 2 ) {
        FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).native_length(), 12 );
    } else {
        FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).native_length(), 14 );
    }
    FSL_CHECK_EQ( fostlib::string( L"\xd834\xdd1e" ).length(), 1 );
}


FSL_TEST_FUNCTION( find ) {
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).find( 'w' ), 6 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).find( L"world" ), 6 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).find( 0x2014 ), 5 );

    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).find_first_of( 0x2014 ), 5 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).find_first_of( L"xe" ), 1 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\x2014world!" ).find_first_not_of( L"He" ), 2 );

    FSL_CHECK_EQ( fostlib::string( L"Hello\xd834\xdd1eworld!" ).find_first_of( 0x1d11e ), 5 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\xd834\xdd1eworld!" ).find_first_of( L"xe" ), 1 );
    FSL_CHECK_EQ( fostlib::string( L"Hello\xd834\xdd1eworld!" ).find_first_not_of( L"He" ), 2 );

    FSL_CHECK_EQ( fostlib::string( L"abc" ).find_last_not_of( L" \n\t" ), 2 );

    FSL_CHECK_EQ( fostlib::string( L"abc" ).find( L"." ), fostlib::string::npos );
}


FSL_TEST_FUNCTION( trim ) {
    FSL_CHECK_EQ( fostlib::trim( L"abc" ), L"abc" );
    FSL_CHECK_EQ( fostlib::trim( L"  abc" ), L"abc" );
    FSL_CHECK_EQ( fostlib::trim( L"  abc " ), L"abc" );

    FSL_CHECK_EQ( fostlib::trim( L"Hello\x2014world!" ), L"Hello\x2014world!" );
}


FSL_TEST_FUNCTION( partition ) {
    FSL_CHECK_NULL( fostlib::partition( L"abc", L"." ).second );
    FSL_CHECK_EQ( fostlib::partition( L"abc", L"." ).first, L"abc" );
}


FSL_TEST_FUNCTION( split ) {
    FSL_CHECK_EQ( fostlib::split( L"abc", L"." )[ 0 ], L"abc" );
    FSL_CHECK_EQ( fostlib::split( L"abc", L"a" )[ 0 ], L"bc" );
    FSL_CHECK_EQ( fostlib::split( L"abc", L"b" )[ 0 ], L"a" );
    FSL_CHECK_EQ( fostlib::split( L"abc", L"b" )[ 1 ], L"c" );
    FSL_CHECK_EQ( fostlib::split( L"abc", L"c" )[ 0 ], L"ab" );

    FSL_CHECK_EQ( fostlib::split( L"abc", L"ab" )[ 0 ], L"c" );
    FSL_CHECK_EQ( fostlib::split( L"abc", L"bc" )[ 0 ], L"a" );

    FSL_CHECK( fostlib::split( L"abc", L"abc" ).empty() );
}
