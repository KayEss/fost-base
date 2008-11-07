/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_WIN_HPP
#define FOST_EXCEPTION_WIN_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/settings.hpp>


namespace fostlib {


    // Format a Windows error to an error string
    FOST_CORE_DECLSPEC string formatLastError();


	extern class FOST_CORE_DECLSPEC com_hr {
	public:
		com_hr( void ) throw () {}
		com_hr( HRESULT hr, size_t line, const char *file );
		template< typename T >
		com_hr( HRESULT hr, T error ) {
			try {
				check( hr );
            } catch ( fostlib::exceptions::exception &e ) {
				e.info() << error << std::endl << format( hr ) << std::endl;
				throw;
			}
		}
		template< typename T >
		com_hr( HRESULT hr, T error, IUnknown *iface ) {
			try {
				check( hr );
			} catch ( fostlib::exceptions::exception &e ) {
				e.info() << error << std::endl << format( iface ) << std::endl;
				throw;
			}
		}
		template< typename T, typename Y >
		com_hr( HRESULT hr, T error, ATL::CComPtr< Y > *iface ) {
			try {
				check( hr );
			} catch ( fostlib::exceptions::exception &e ) {
				e.info() << error << std::endl << format( iface ) << std::endl;
				throw;
			}
		}

		HRESULT operator =( HRESULT hr ) const { check( hr ); return hr; }
		HRESULT operator()( HRESULT hr ) const { check( hr ); return hr; }

		static fostlib::string format( HRESULT hr );
		static fostlib::string format( IUnknown *iface );
		static fostlib::string format( ATL::CComPtr<ISupportErrorInfo> iface );

	private:
		void check( HRESULT hr ) const { if ( FAILED( hr ) ) doThrow( hr ); }
		void doThrow( HRESULT hr ) const;
	} hresult;


    namespace exceptions {


        class FOST_CORE_DECLSPEC structured : public exception {
        public:
            structured( const structure_information &info ) throw ();

            static const setting< bool > c_translate;

        protected:
            const wchar_t * const message() const throw ();
        };


        class FOST_CORE_DECLSPEC com_error : public exception {
        public:
            com_error( const string &message );
            com_error( const string &message, const string &information );
            com_error( const _com_error &c );
            com_error( const _com_error &c, const string & );

        protected:
            const wchar_t * const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_WIN_HPP

