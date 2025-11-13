#pragma once


#include <fost/string.hpp>
#include <fost/json-core.hpp>
#include <fost/accessors.hpp>

#include <exception>


namespace fostlib {


    namespace exceptions {


        /// Base class for all Fost exceptions
        class FOST_CORE_DECLSPEC exception : public std::exception {
          public:
            /// Allow this class to be safely used as a base class
            ~exception() noexcept;

            /// Return the overall message for the exception
            virtual felspar::u8view message() const = 0;

            /// Print the exception on to the specified stream
            virtual ostream &printOn(ostream &) const;

            /// Fetch the structured information about the exception
            const json &data() const;

            /// Used to store structured data in the exception
            json &data();

            /// Allow us to retrieve a text description of the exception as used
            /// by std::exception
            char const *what() const noexcept;

            /// Return a JSON representation of this exception
            virtual json as_json() const;

          protected:
            json m_data;

            exception(std::source_location) noexcept;
            exception(felspar::u8view, std::source_location) noexcept;

          private:
            std::source_location m_source_location;
            mutable felspar::u8string m_what_string;
        };


        /// Output the exception to a stream
        inline ostream &operator<<(ostream &o, const exception &e) {
            return e.printOn(o);
        }


    }


    /// Call this whenever an exception is thrown away so that it break points
    /// can be set easily
    FOST_CORE_DECLSPEC void absorb_exception() noexcept;


}
