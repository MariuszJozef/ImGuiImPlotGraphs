#[=========================[
Either find preinstaled SDL2 package, or if it's absent, fetch it from github
#]=========================]
set(packageName SDL2)
set(packageVersion 2.28.5)

#[=========================[
Point the path to ${packageName}Config.cmake or ${packageName}-config.cmake
if the package is not on the standard search path nor in CMAKE_INSTALL_PREFIX
#]=========================]
if (WIN32)
    set(${packageName}_DIR C:/Packages/SDL2-2.28.3/cmake)
    # set(packageVersion 2.28.3)
elseif(LINUX)
    set(${packageName}_DIR /usr/lib64/cmake/SDL2)
    # set(packageVersion 2.26.3)
elseif(APPLE)
    set(${packageName}_DIR /opt/local/lib/cmake/SDL2)
    # set(packageVersion 2.28.0)
elseif(UNIX AND NOT APPLE)
    set(${packageName}_DIR )
endif()

# Omit REQUIRED keyword so as to be able to fetch the package if it is not installed
find_package(${packageName} ${packageVersion})

if(${packageName}_FOUND)
    message(STATUS "${packageName}_FOUND: ${${packageName}_FOUND}")
    message(STATUS "${packageName}_VERSION: ${${packageName}_VERSION}")
    message(STATUS "${packageName}_INCLUDE_DIRS: ${${packageName}_INCLUDE_DIRS}")
    message(STATUS "${packageName}_LIBRARIES: ${${packageName}_LIBRARIES}")

    set(gtest_force_shared_crt ON CACHE BOOL "On Windows don't override this project's linker settings; always use msvcrt.dll" FORCE)

else()
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    
    FetchContent_Declare(sdl2
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
        GIT_TAG         SDL2 # release2-28.3
        SOURCE_DIR      ${CMAKE_SOURCE_DIR}/External/SDL2
        GIT_PROGRESS    TRUE
        GIT_SHALLOW     TRUE
        USES_TERMINAL_DOWNLOAD TRUE   # <---- only used by Ninja generator
    )

    FetchContent_MakeAvailable(sdl2)
endif()
