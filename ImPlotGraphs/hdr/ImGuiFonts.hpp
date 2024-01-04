#pragma once
#include <filesystem>
#include <vector>
#include <array>
#include <map>

#include "imgui.h"

namespace Code::ImGuiImPlot
{
    
class ImGuiView;

class ImGuiFonts
{
public:
    void RetrieveAndStoreFontFiles();
    void StoreUsableFontsIntoTwoIndexMap();
    ImFont* GetUsableFonts(int selectedFontNameIndex, int selectedFontSizeIndex);
    void FillFontWidgetInfo();
    void PushSelectedFont();

private:
    std::vector<std::filesystem::path> availableFontFiles;
    std::vector<std::string> availableFontNames;
    std::array<float, 4> fontSizes {16.0F, 20.0F, 24.0F, 28.0F};
    std::array<std::string, 4> fontSizesStr;
    int selectedFontNameIndex {3};
    int selectedFontSizeIndex {2};
    std::unique_ptr<const char*[]> pFontNameItems {nullptr};
    std::unique_ptr<const char*[]> pFontSizeItems {nullptr};
    
    // Nested map - get usableFont via two indices: std::string fontName, float fontSize:
    // usableFont["fontName"][fontSize]
    std::map<std::string, std::map<float, ImFont*>> usableFont;

    friend class ImGuiView;
};

} // ~namespace Code::ImGuiImPlot
