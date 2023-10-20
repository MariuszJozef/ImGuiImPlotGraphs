#[==================================================[
ImPlotGraphs (executable program)
#]==================================================]
add_executable(targetExe) 

target_include_directories(targetExe
    PRIVATE
        hdr
        ../External/glad4.1/include
        ../External/imgui-docking
        ../External/implot-master
)

target_sources(targetExe
    PRIVATE
        src/MainImGuiImPlot.cpp
        hdr/SDLWindow.hpp
        src/SDLWindow.cpp
        hdr/OpenGLContext.hpp
        src/OpenGLContext.cpp
        hdr/ImGuiFrame.hpp
        src/ImGuiFrame.cpp
        hdr/ImGuiFonts.hpp
        src/ImGuiFonts.cpp
        hdr/ImGuiView.hpp
        src/ImGuiView.cpp
        hdr/ImPlotFrame.hpp
        src/ImPlotFrame.cpp
        hdr/ImPlotGeneric.hpp
        src/ImPlotGeneric.cpp
        hdr/ImPlotGraph1.hpp
        src/ImPlotGraph1.cpp
        hdr/ImPlotGraph2.hpp
        src/ImPlotGraph2.cpp
        hdr/ImPlotGraph3.hpp
        src/ImPlotGraph3.cpp
        hdr/ImPlotGraph4.hpp
        src/ImPlotGraph4.cpp
        hdr/ImPlotGraph5.hpp
        src/ImPlotGraph5.cpp
# GLAD
        ../External/glad4.1/src/glad.c
# IMGUI
        ../External/imgui-docking/imconfig.h
        ../External/imgui-docking/imgui.cpp
        ../External/imgui-docking/imgui.h
        ../External/imgui-docking/imgui_demo.cpp
        ../External/imgui-docking/imgui_draw.cpp
        ../External/imgui-docking/imgui_internal.h
        ../External/imgui-docking/imgui_tables.cpp
        ../External/imgui-docking/imgui_widgets.cpp
        ../External/imgui-docking/imstb_rectpack.h
        ../External/imgui-docking/imstb_textedit.h
        ../External/imgui-docking/imstb_truetype.h
        ../External/imgui-docking/imgui_impl_opengl3.cpp
        ../External/imgui-docking/imgui_impl_opengl3.h
        ../External/imgui-docking/imgui_impl_opengl3_loader.h
        ../External/imgui-docking/imgui_impl_sdl2.cpp
        ../External/imgui-docking/imgui_impl_sdl2.h
# IMPLOT
        ../External/implot-master/implot.cpp
        ../External/implot-master/implot.h
        ../External/implot-master/implot_demo.cpp
        ../External/implot-master/implot_internal.h
        ../External/implot-master/implot_items.cpp
)

set_target_properties(targetExe
    PROPERTIES
        OUTPUT_NAME                         ImPlotGraphs
        EXPORT_NAME                         ImPlotGraphs
        DEBUG_POSTFIX                       "_d"
        RUNTIME_OUTPUT_DIRECTORY            ${CMAKE_BINARY_DIR}/buildOut/ImPlotGraphs/bin/$<CONFIG>
        PDB_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/buildOut/ImPlotGraphs/pdb/$<CONFIG>
        RUNTIME_OUTPUT_DIRECTORY_<Config>   ${RUNTIME_OUTPUT_DIRECTORY}
        PDB_OUTPUT_DIRECTORY_<Config>       ${PDB_OUTPUT_DIRECTORY}
)

target_compile_features(targetExe
    PRIVATE
        c_std_17
        cxx_std_20
)

if(MSVC)

    target_compile_options(targetExe 
        PRIVATE
            /EHsc
            /W2
            # /Wall
    )

else()

    target_compile_options(targetExe 
        PRIVATE
            -Wall
            # -Wextra
            # -Wpedantic
    )

endif()

# Uncomment the proprocessor define to enable OpenGL draw primitives: glBegin(); ... glEnd();
target_compile_definitions(targetExe
    PRIVATE
        # OPENGL_DRAW_PRIMITIVES
)

#[==================================================[
On Windows copy *.dll needed at runtime by the .exe into the same directory as the .exe
#]==================================================]
option(isThereDllToLink "Switch OFF when not linking any DLLs, otherwise copy <empty> will fail!" ON)

if(WIN32 AND isThereDllToLink)
    add_custom_command(TARGET targetExe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "copy: $<TARGET_RUNTIME_DLLS:targetExe>"
        COMMAND ${CMAKE_COMMAND} -E echo "to:   $<TARGET_FILE_DIR:targetExe>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                $<TARGET_RUNTIME_DLLS:targetExe> 
                $<TARGET_FILE_DIR:targetExe>
        COMMAND_EXPAND_LISTS
    )
endif()

#[==================================================[
# Copy fonts for ImGui into binary directory (relative to executable) so that they 
can be accessed by relative paths and IRRESPECTIVE of how the program is launched!
#]==================================================]
add_custom_command(TARGET targetExe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "copy: ${CMAKE_SOURCE_DIR}/External/fonts/*.ttf"
    COMMAND ${CMAKE_COMMAND} -E echo "to:   ${CMAKE_BINARY_DIR}/buildOut/External/fonts/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        ${CMAKE_SOURCE_DIR}/External/fonts
        ${CMAKE_BINARY_DIR}/buildOut/External/fonts
    COMMENT "Copy fonts for ImGui into binary directory (relative to executable) so that they can be accessed by relative paths and IRRESPECTIVE of how the program is launched!"
)

#[==================================================[
Run the executable 
#]==================================================]
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E echo "execute: $<TARGET_FILE:targetExe>"
    COMMAND $<TARGET_FILE:targetExe>
)
