#include <fost/test>
#include <fost/unicode>

#define TRIM(X) (fostlib::trim(fostlib::nullable<felspar::u8view>{X}))
#define TRIM_WITH(X, Y) (fostlib::trim(felspar::u8view{X}, felspar::u8view{Y}))

FSL_TEST_SUITE(trim);

FSL_TEST_FUNCTION(basic_trim) {
    FSL_CHECK_EQ(TRIM(), fostlib::null);
    FSL_CHECK_EQ(TRIM(""), fostlib::null);
    FSL_CHECK_EQ(TRIM("\x20\xE2\x80\x8A"), fostlib::null);
    FSL_CHECK_EQ(
            TRIM("\xEF\xBB\xBFstr\xE2\x80\x87ing\xE2\x81\x9F\xE2\x80\xAF"),
            "str\xE2\x80\x87ing");
    FSL_CHECK_EQ(TRIM("string\xE2\x80\x82\xE2\x80\x83\xE2\x80\x84"), "string");
    FSL_CHECK_EQ(TRIM("\xE2\x80\x81\xC2\xA0string"), "string");
    FSL_CHECK_EQ(TRIM("\r\t\x20string\n\x20\f"), "string");
    FSL_CHECK_EQ(TRIM("\xE2\x80\x80string\xE3\x80\x80\xE2\x80\x88"), "string");
    FSL_CHECK_EQ(
            fostlib::trim(fostlib::nullable<fostlib::ascii_string>{" string "}),
            "string");
    FSL_CHECK_EQ(
            fostlib::trim(
                    fostlib::nullable<fostlib::utf8_string>{
                            "\xE2\x80\x82\xE2\x80\x83\xE2\x80\x84string\x20"}),
            "string");
    FSL_CHECK_EQ(
            fostlib::trim(
                    fostlib::nullable<fostlib::string>{
                            "\xE2\x80\x82\xE2\x80\x83\xE2\x80\x84string\x20"}),
            "string");
}

FSL_TEST_FUNCTION(specific_trim) {
    FSL_CHECK_EQ(TRIM_WITH("_string_", "_"), "string");
    FSL_CHECK_EQ(TRIM_WITH(, ""), fostlib::null);
    FSL_CHECK_EQ(
            TRIM_WITH("\x20\xE2\x80\x8Astring", "\x20"), "\xE2\x80\x8Astring");
    FSL_CHECK_EQ(
            fostlib::trim(fostlib::string{"xystringyx"}, fostlib::string{"xy"}),
            "string");
}
