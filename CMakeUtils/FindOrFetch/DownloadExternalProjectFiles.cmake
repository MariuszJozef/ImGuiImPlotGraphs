#[==================================================[
DownloadImGui
#]==================================================]
set(downloadImGuiFiles
    https://raw.githubusercontent.com/ocornut/imgui/docking/imconfig.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/imconfig.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui_demo.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui_draw.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui_internal.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui_tables.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/imgui_widgets.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/imstb_rectpack.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/imstb_textedit.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/imstb_truetype.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/backends/imgui_impl_opengl3.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/backends/imgui_impl_opengl3.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/backends/imgui_impl_opengl3_loader.h
    https://raw.githubusercontent.com/ocornut/imgui/docking/backends/imgui_impl_sdl2.cpp
    https://raw.githubusercontent.com/ocornut/imgui/docking/backends/imgui_impl_sdl2.h
)

set(imGuiDir ${CMAKE_SOURCE_DIR}/External/imgui-docking)

foreach(filePath IN LISTS downloadImGuiFiles)

    cmake_path(GET filePath FILENAME fileName)

    if (NOT EXISTS ${imGuiDir}/${fileName})

        message(STATUS "Downloading: ${filePath}\n   into:        ${imGuiDir}")

        file(DOWNLOAD 
            ${filePath}
            ${imGuiDir}/${fileName}
            STATUS downloadStatus
        )

        list(GET downloadStatus 0 statusCode)
        list(GET downloadStatus 1 errorMessage)

        if(${statusCode} EQUAL 0)
            message("   success!")
        else()
            message(FATAL_ERROR "Failed to download: ${errorMessage}")
        endif()

    endif()

endforeach()

#[==================================================[
DownloadImPlot
#]==================================================]
set(downloadImPlotFiles
    https://raw.githubusercontent.com/epezent/implot/master/implot.cpp
    https://raw.githubusercontent.com/epezent/implot/master/implot.h
    https://raw.githubusercontent.com/epezent/implot/master/implot_demo.cpp
    https://raw.githubusercontent.com/epezent/implot/master/implot_internal.h
    https://raw.githubusercontent.com/epezent/implot/master/implot_items.cpp
)

set(ImPlotDir ${CMAKE_SOURCE_DIR}/External/implot-master)

foreach(filePath IN LISTS downloadImPlotFiles)

    cmake_path(GET filePath FILENAME fileName)

    if (NOT EXISTS ${ImPlotDir}/${fileName})

        set(isDownloaded CACHE BOOL TRUE FORCE)

        message(STATUS "Downloading: ${filePath}\n   into:        ${ImPlotDir}")

        file(DOWNLOAD 
            ${filePath}
            ${ImPlotDir}/${fileName}
            STATUS downloadStatus
        )

        list(GET downloadStatus 0 statusCode)
        list(GET downloadStatus 1 errorMessage)

        if(${statusCode} EQUAL 0)
            message("   success!")
        else()
            message(FATAL_ERROR "Failed to download: ${errorMessage}")
        endif()

    endif()

endforeach()
