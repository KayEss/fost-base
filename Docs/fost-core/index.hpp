/** \defgroup fostcore fost-core

    fost-core is the bottom most layer of the Fost 4 libraries. It features a number of foundation sub-libraries.

    The sub-libraries included at this level

    - \ref fost_core_string -- A Unicode string class modelled on the `std::string` interface, but using the native character size internally (char on Linux, wchar_t on Windows), but presenting a consistent UTF-32 based exterior. This allows Unicode strings to be used efficiently with portable source code between Windows and Linux.
    - \ref fost_core_settings -- A settings database used for software configuration.
    - [[JSON]] -- A JSON parser and printer together with suitable data types to handle JSON objects, arrays and atoms.
    - \ref fost_core_coerce -- A type safe type coercion library which is extensible for user defined types.
    - [[thread]] -- A threading library providing futures and worker threads with thread affinity.

    There are also a number of smaller support libraries.
*/