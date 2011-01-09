/*
    Copyright 2000-2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TIMESTAMP_HPP
#define FOST_TIMESTAMP_HPP
#pragma once


#include <fost/date.hpp>
#include <fost/time.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <utility>
using namespace std::rel_ops;


namespace fostlib {


    /// Stores a time and date together with a time zone. UTC is preferred.
    class FOST_CORE_DECLSPEC timestamp {
        boost::posix_time::ptime m_ts;
        friend struct fostlib::coercer< boost::posix_time::ptime, timestamp >;
    public:
        /// Construct an inderminate timestamp
        timestamp();
        /// Construct a timestamp from a Boost POSIX ptime
        explicit timestamp(boost::posix_time::ptime pt);
        /// Construct a timestamp for midnight at the start of the specified day
        timestamp( int year, int month, int day );

        /// The zone info associated with this time stamp
        accessors< zoneinfo > timezone;

        /// The current time
        static timestamp now();

        /// Compare time stamps for equality
        bool operator == ( const timestamp &ts ) const {
            return m_ts == ts.m_ts;
        }
        /// Compare time stamps for inequality
        bool operator != ( const timestamp &ts ) const {
            return m_ts != ts.m_ts;
        }
    };


    /// This date format is used in emails and HTTP where it is the date format
    struct FOST_CORE_DECLSPEC rfc1123_timestamp_tag {
        static void do_encode( fostlib::nliteral from, ascii_string &into );
        static void do_encode( const ascii_string &from, ascii_string &into );
        static void check_encoded( const ascii_string &s );
    };
    typedef tagged_string< rfc1123_timestamp_tag, ascii_string > rfc1123_timestamp;


    /// Allow coercing of timestamps to and from common types
    template<>
    struct coercer< boost::posix_time::ptime, timestamp > {
        boost::posix_time::ptime coerce( timestamp t ) {
            return t.m_ts;
        }
    };
    template<>
    struct coercer< timestamp, boost::posix_time::ptime > {
        timestamp coerce(boost::posix_time::ptime p) {
            return timestamp(p);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, timestamp > {
        // The default string format is ISO with the 'T' separator exchanged for a space
        string coerce( timestamp );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< rfc1123_timestamp, timestamp > {
        rfc1123_timestamp coerce( timestamp );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, rfc1123_timestamp > {
        string coerce( const rfc1123_timestamp &t ) {
            return fostlib::coerce< string >( t.underlying() );
        }
    };

    /// We need JSON as well if we are to be able to use this for database fields
    template<>
    struct coercer< json, timestamp > {
        json coerce( timestamp t ) {
            return json( fostlib::coerce< string >( t ) );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< timestamp, json > {
        timestamp coerce( const json & );
    };


}


namespace std {


    inline fostlib::ostream &operator << ( fostlib::ostream &o, fostlib::timestamp t ) {
        return o << fostlib::coerce< fostlib::string >( t );
    }


}


#endif // FOST_TIMESTAMP_HPP
