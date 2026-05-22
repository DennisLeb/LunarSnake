#
# Dependencies
#

include(FetchContent)

# raylib
find_package(raylib 5.0 QUIET)

if (NOT raylib_FOUND)

    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
    )

    FetchContent_MakeAvailable(raylib)

endif()

set_target_properties(raylib PROPERTIES FOLDER "dependencies")