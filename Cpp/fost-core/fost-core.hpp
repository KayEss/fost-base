#ifdef FOST_OS_WINDOWS
#define FOST_CORE_DECLSPEC __declspec(dllexport)
#else
#define FOST_CORE_DECLSPEC
#endif


#include <fost/exception.hpp>
#include <fost/coerce.hpp>
#include <fost/detail/coerce.hpp>
#include <fost/settings.hpp>
#include <fost/utility-nullable.hpp>
