/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_POINTERS_HPP
#define FOST_POINTERS_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/exception/null.hpp>


namespace fostlib {


    /*
        Auto-initialising pointer
    */
    template< typename P >
    class init_ptr {
    public:
        init_ptr() : m_p( NULL ) {}
        init_ptr( P *p ) : m_p( p ) {}

        P *&get() { return m_p; }
        const P *get() const { return m_p; }

        P *reset( P *n ) { m_p = n; return n; }

        P &operator *() { return *get(); }
        const P &operator *() const { return *get(); }
        P *operator->() { return get(); }
        const P *operator->() const { return get(); }


        template< typename R >
        bool operator <( const init_ptr< R > r ) const {
            return get() < r.get();
        }
        template< typename R >
        bool operator >( const init_ptr< R > r ) const {
            return get() > r.get();
        }

    private:
        P *m_p;
    };

    template< typename P > bool operator ==( const P *l, const init_ptr< P > &r ) {
        return l == r.get();
    }
    template< typename P > bool operator ==( const init_ptr< P > &l, const init_ptr< P > &r ) {
        return l.get() == r.get();
    }
    template< typename P > bool operator ==( const init_ptr< P > &l, const P *r ) {
        return l.get() == r;
    }


    /*
        Pointer that acts like a reference
    */

    template< typename P >
    class reference_ptr {
    public:
        reference_ptr() : m_p() {}
        reference_ptr( P * p ) : m_p( p ) {}

        P &operator *() { return *m_p; }
        P *operator ->() { return m_p; }

        const P &operator *() const { return *m_p; }
        const P *operator ->() const { return m_p; }

        template< typename R >
        reference_ptr< P > &operator =( const reference_ptr< R > r ) {
            m_p = r.m_p;
            return *this;
        }
        template< typename R >
        reference_ptr< P > &operator =( R *r ) {
            if ( r == NULL ) throw fostlib::exceptions::null( L"Cannot assign null to a reference_ptr" );
            m_p = r;
            return *this;
        }

        template< typename R >
        bool operator <( const reference_ptr< R > r ) const {
            return m_p < r.m_p;
        }
        template< typename R >
        bool operator >( const reference_ptr< R > r ) const {
            return m_p > r.m_p;
        }

    private:
        P* m_p;
    };


}


#endif // FOST_POINTERS_HPP

