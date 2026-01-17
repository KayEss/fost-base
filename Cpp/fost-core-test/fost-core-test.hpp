#ifdef FOST_OS_WINDOWS
#define FOST_TEST_DECLSPEC __declspec(dllimport)
#else
#define FOST_TEST_DECLSPEC
#endif


#include <fost/test>
