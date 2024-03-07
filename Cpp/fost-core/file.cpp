#include "fost-core.hpp"
#include <fost/filesystem.hpp>
#include <fost/unicode.hpp>
#include <fost/detail/utility.hpp>

#include <fost/exception/file_error.hpp>
#include <fost/exception/unexpected_eof.hpp>
#include <fost/exception/unicode_encoding.hpp>

#include <fstream>


using namespace fostlib;


namespace {
    string loadfile(std::ifstream &file) {
        string text;

        utf32 u32 = 0;
        size_t len = 0;
        while (!file.eof() && file.good()) {
            int u8 = file.get();
            if (u8 < 0x80 && len > 0)
                throw fostlib::exceptions::unicode_encoding(
                        "Not enough continuation characters found");
            else if (u8 < 0x80 && u8 > 0)
                text += char(u8);
            else if (u8 >= 0x80 && u8 < 0xC0) {
                if (len-- == 0)
                    throw fostlib::exceptions::unicode_encoding(
                            "Continuation character found in wrong place");
                u32 = (utf32(u32) << 6) | (utf32(u8) & 0x3F);
                if (len == 0 && u32 == utf::c_bom && !text.empty())
                    throw fostlib::exceptions::unicode_encoding(
                            "BOM may not appear anywhere other than at the "
                            "beginning of the file");
                else if (len == 0 && u32 != utf::c_bom)
                    text += u32;
            } else if (u8 >= 0xC0 && u8 < 0xE0) {
                len = 1;
                u32 = u8 & 0x1F;
            } else if (u8 >= 0xE0 && u8 < 0xF0) {
                len = 2;
                u32 = u8 & 0x0F;
            } else if (u8 >= 0xF0 && u8 < 0xF8) {
                len = 3;
                u32 = u8 & 0x07;
            }
        }

        return text;
    }
}


void fostlib::utf::save_file(
        std::filesystem::path const &filename, const string &content) {
    std::ofstream file{filename.string()};
    if (not file.is_open())
        throw exceptions::file_error(
                "File open unsuccesful", filename.string().c_str());
    file << coerce<utf8_string>(content).underlying();
    file.close();
    if (file.bad())
        throw exceptions::file_error(
                "File close unsuccesful", filename.string().c_str());
    /// Failed writes seem pretty hard to detect. The following is the safest
    /// detection, but it comes at quite some cost.
    //     if ( load_file(filename) != content )
    //         throw exceptions::file_error("File read following write failed",
    //         filename.string().c_str());
}


string fostlib::utf::load_file(std::filesystem::path const &filename) {
    std::ifstream file(filename);
    string text = loadfile(file);
    if ((!file.eof() && file.bad()) || text.empty())
        throw exceptions::unexpected_eof(
                "Could not load the requested file (or file empty)",
                coerce<string>(filename));
    return text;
}


string fostlib::utf::load_file(
        std::filesystem::path const &filename, const string &default_content) {
    std::ifstream file(filename);
    string text = loadfile(file);
    if ((!file.eof() && file.bad()) || text.empty())
        return default_content;
    else
        return text;
}


std::filesystem::path fostlib::unique_filename() {
    return std::filesystem::temp_directory_path()
            / coerce<std::filesystem::path>(guid());
}


std::filesystem::path fostlib::join_paths(
        std::filesystem::path const &root, std::filesystem::path const &path) {
    if (path.empty()) {
        return root;
    } else if (path.has_root_directory() || coerce<string>(path)[0] == '/') {
        return path;
    } else {
        return root / path;
    }
}
