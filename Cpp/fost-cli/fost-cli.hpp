#ifdef FOST_OS_WINDOWS
#define FOST_CLI_DECLSPEC __declspec(dllexport)
#else
#define FOST_CLI_DECLSPEC
#endif


#include <fost/core>
