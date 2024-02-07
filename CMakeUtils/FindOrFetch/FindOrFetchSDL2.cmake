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
    # set(${packageName}_DIR C:/Packages/SDL2-2.28.3/cmake)
    # set(packageVersion 2.28.3)
    set(${packageName}_DIR C:/SDL2/SDL2-2.28.5/cmake)
elseif(LINUX)
    set(${packageName}_DIR /usr/lib/x86_64-linux-gnu/cmake/SDL2)
    set(packageVersion 2.0.20...2.28.5)
    # set(${packageName}_DIR /usr/lib64/cmake/SDL2)
    # set(packageVersion 2.26.3)
elseif(APPLE)
    # set(${packageName}_DIR /opt/local/lib/cmake/SDL2)
    set(${packageName}_DIR /usr/local/Cellar/sdl2/2.28.5/lib/cmake/SDL2)
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
else()
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    set(externalProjectDir ${CMAKE_SOURCE_DIR}/External/SDL2)
    set(externalProjectUrl https://github.com/libsdl-org/SDL.git)

    # This 'if' assumes that externalProjectDir is not empty and contains all sources (not just CMakeLists.txt) downloaded in the 'else' clause
    if(EXISTS ${externalProjectDir}/CMakeLists.txt)
        message(STATUS "Not fetching ${packageName} again from ${externalProjectUrl} since it's already downloaded locally into ${externalProjectDir}")

        FetchContent_Declare(sdl2
            SOURCE_DIR      ${externalProjectDir}
        )
    else()
        message(STATUS "Fetching ${packageName} from remote repo: ${externalProjectUrl}")

        FetchContent_Declare(sdl2
            GIT_REPOSITORY  ${externalProjectUrl}
            GIT_TAG         SDL2 # release2-28.3
            SOURCE_DIR      ${externalProjectDir}
            GIT_PROGRESS    TRUE
            GIT_SHALLOW     TRUE
            USES_TERMINAL_DOWNLOAD TRUE   # <---- only used by Ninja generator
        )
    endif()

    FetchContent_MakeAvailable(sdl2)
endif()
