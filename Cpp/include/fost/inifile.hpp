#ifndef FOST_INIFILE_HPP
#define FOST_INIFILE_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/settings.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CLI_DECLSPEC invalid_ini_line : public exception {
          public:
            invalid_ini_line(
                    const string &section,
                    const string &read,
                    const string &processed,
                    std::source_location =
                            std::source_location::current()) noexcept;

          protected:
            felspar::u8view message() const noexcept;
        };


    }


    class FOST_CLI_DECLSPEC ini_file {
      public:
        ini_file(string const &a_fileName);

      private:
        void loadAll();

        string m_fileName;
        std::vector<std::shared_ptr<setting<json>>> m_settings;
    };


}


#endif // FOST_INIFILE_HPP
