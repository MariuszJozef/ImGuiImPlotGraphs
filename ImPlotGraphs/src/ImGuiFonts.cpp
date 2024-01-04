#include <algorithm>
#include <sstream>
#include "SDL.h"
#include "ImGuiFonts.hpp"

namespace Code::ImGuiImPlot
{
    
void ImGuiFonts::RetrieveAndStoreFontFiles()
{
    std::filesystem::path fontPathRelToExecDir = SDL_GetBasePath();
    
    // PRECAUTION: ENSURE a trailing slash below!
    // Otherwise file name will concatenate to last directory name!
    fontPathRelToExecDir = fontPathRelToExecDir
                        .parent_path().parent_path().parent_path().parent_path() 
                        / "External/fonts/"; 
    
    // Remedy just in case a traling slash above was omitted!
    // The following doesn't cause problems if a trailing slash is already there
    {
        fontPathRelToExecDir /= "."; 
        // Now remove unneeded dots and slashes
        fontPathRelToExecDir = fontPathRelToExecDir.lexically_normal();
    }

    if (std::filesystem::exists(fontPathRelToExecDir) && 
        std::filesystem::is_directory(fontPathRelToExecDir))
    {
        for (const auto& file : std::filesystem::directory_iterator(fontPathRelToExecDir))
        {
            availableFontFiles.push_back(file);
            availableFontNames.push_back(file.path().stem().generic_string());
        }
    }
    else
    {
        throw std::runtime_error("Failure: RetrieveAndStoreFontFiles");
    }
}

void ImGuiFonts::StoreUsableFontsIntoTwoIndexMap()
{
    ImGuiIO& io = ImGui::GetIO();

    for (const auto& file : availableFontFiles)
    {
        for (const auto& fontSize : fontSizes)
        {
            std::string fontName {file.stem().generic_string()};
            std::string fontFile {file.generic_string()};

            usableFont[fontName][fontSize] = io.Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize);
        }
    }
}

ImFont* ImGuiFonts::GetUsableFonts(int selectedFontNameIndex, int selectedFontSizeIndex)
{
    std::string fontName {availableFontNames.at(selectedFontNameIndex)};
    float fontSize {fontSizes.at(selectedFontSizeIndex)};
    
    return usableFont[fontName][fontSize];
}

void ImGuiFonts::FillFontWidgetInfo()
{
    std::transform(fontSizes.begin(), fontSizes.end(), 
        fontSizesStr.begin(), [](float fontSize) 
            {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(1) << fontSize; 
                return ss.str();
            }
    );

    pFontNameItems = std::make_unique<const char*[]>(availableFontNames.size());
    pFontSizeItems = std::make_unique<const char*[]>(fontSizesStr.size());

    for (int index = 0; const auto& fontName : availableFontNames)
    {
        pFontNameItems[index++] = fontName.c_str();
    }

    for (int index = 0; const auto& fontSize : fontSizesStr)
    {
        pFontSizeItems[index++] = fontSize.c_str();
    }
}

void ImGuiFonts::PushSelectedFont()
{
    ImGui::PushFont(GetUsableFonts(selectedFontNameIndex, selectedFontSizeIndex));
}

} // ~namespace Code::ImGuiImPlot

