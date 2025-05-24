#include <fost/cli>
#include <fost/filesystem.hpp>
#include <fost/main>
#include <fost/unicode>

#include <fstream>


FSL_MAIN(
        "fost-core-test-file-io",
        "fost-core-test-file-io\n"
        "Test file I/O and its Unicode handling\n"
        "Copyright (C) 2009-2025 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    auto filename(fostlib::coerce<std::filesystem::path>(args[1].value()));
    // Check that we can do some basic reads
    { // Build a basic text stream that we want to check against
        std::ofstream outfile(filename);
        outfile.write("abcdef\n", 7);
        unsigned char tm[] = {0xe2, 0x84, 0xa2, 0x00};
        outfile.write(reinterpret_cast<char *>(tm), 3);
        outfile.write("\n", 1);
    }
    felspar::u8string loaded = fostlib::utf::load_file(filename);
    if (loaded != felspar::u8string{u"abcdef\n\u2122\n"}) {
        out << "File loaded did not match file saved\n"
               "97 98 99 100 101 102 10 8482 10"
            << std::endl;
        for (auto const c : loaded) { out << int(c) << " "; }
        out << std::endl;
        return 1;
    }
    std::filesystem::remove(filename);
    return 0;
}
