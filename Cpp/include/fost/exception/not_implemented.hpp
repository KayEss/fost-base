#pragma once


#include <fost/coerce.hpp>
#include <fost/exception.hpp>
#include <fost/filesystem.hpp>


namespace fostlib {


    namespace exceptions {


        /// Not implemented exceptions show where code is still incomplete
        class FOST_CORE_DECLSPEC not_implemented : public exception {
          public:
            /// Throw a not implemented exception
            not_implemented(
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            /// Throw a not implemented exception providing a message
            not_implemented(
                    const string &message,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            /// Throw providing a message and extra information
            not_implemented(
                    const string &message,
                    const string &extra,
                    felspar::source_location =
                            felspar::source_location::current()) noexcept;
            /// Throw providing a message and extra JSON information
            not_implemented(
                    const string &message,
                    const json &extra,
                    felspar::source_location =
                            felspar::source_location::current());
            /// Extra information can be anything coercible to JSON
            template<typename E>
            not_implemented(
                    const string &message,
                    const E &extra,
                    felspar::source_location loc =
                            felspar::source_location::current())
            : not_implemented(message, coerce<json>(extra), std::move(loc)) {}

          protected:
            felspar::u8view message() const noexcept;
        };


    }


}
