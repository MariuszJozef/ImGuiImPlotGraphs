#pragma once
#include <numeric>
#include <map>
#include <vector>
#include <string>
#include "implot.h"
#include "ImPlotGeneric.hpp"

namespace Code::ImGuiImPlot
{

extern const float PI;

class Plot1 : public PlotGeneric
{
public:
    Plot1();
    void Graph() override;

    bool* SetIsMarkersViaPtr() { return &isMarkers; }
    float* SetAmplitudeViaPtr() { return &amplitude; }
    float GetAmplitudeMax() { return amplitudeMax; }
    float* SetWavelengthViaPtr() { return &wavelength; }
    float* SetXShiftViaPtr() { return &xShift; }
    float* SetYShiftViaPtr() { return &yShift; }
    int* SetSelectedPlotColourViaPtr() { return &selectedPlotColour; }
    int* SetSelectedXAxisDomainViaPtr() { return &selectedXAxisDomain; }
    const char * const * GetXAxisDomainStartOfComboList() { return &xAxisDomain.at(0); }
    std::string GetPlotColourName() { return plotColour.at(selectedPlotColour).first; }
    int GetPlotColourSize() { return plotColour.size(); }
    int GetXAxisDomainSize() { return xAxisDomain.size(); }

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;
    void GenerateXTicks();
    std::vector<const char*> GenerateXLabels();
    void GenerateYTicksAndYLabels();

private:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> xTicks;
    std::vector<double> yTicks;
    std::vector<const char*> xLabels;
    std::vector<const char*> yLabels;
    static const int distinctBufferCount {21}; // 4 * amplitudeMax + 1
    static const int yLabelBufferSize {16};
    char yLabelsThatWork[distinctBufferCount][yLabelBufferSize];
    std::vector<const char*> xAxisDomain 
        {
            "[xMin, xMax] = [0, 2PI]",
            "[xMin, xMax] = [-PI, PI]",
            "[xMin, xMax] = [-2PI, 2PI]",
            "[xMin, xMax] = [-3PI, 3PI]",
            "[xMin, xMax] = [-4PI, 4PI]",
            "[xMin, xMax] = [-5PI, 5PI]"
        };
    int selectedXAxisDomain {3};
    double xMin {0.0};
    double xMax {2 * PI};
    double yMin {0.0};
    double yMax {0.0};
    
    float amplitude {1.0F}; // float because its is adjusted in ImGui::SliderFloat
    const float amplitudeMax {5.0F};
    float wavelength {2.0F * PI};
    float xShift {0.0F};
    float yShift {0.0F};
    bool isMarkers {false};
    int selectedPlotColour {3};

    std::map<int, std::pair<std::string, ImVec4>> plotColour 
        {
            {0, std::make_pair("red    ", ImVec4(1, 0, 0, 1))},
            // {1, std::make_pair("orange ", ImVec4(1, 140.0/255.0, 0, 1))},
            {1, std::make_pair("orange ", ImVec4(1, 127.0/255, 80.0/255, 1))},
            // {1, std::make_pair("orange ", ImVec4(200.0/255, 165.0/255, 0, 1))},
            {2, std::make_pair("yellow ", ImVec4(1, 1, 0, 1))},
            {3, std::make_pair("green  ", ImVec4(0, 1, 0, 1))},
            {4, std::make_pair("blue   ", ImVec4(0, 0, 1, 1))},
            {5, std::make_pair("violet ", ImVec4(148, 0, 211, 1))},
            {6, std::make_pair("indigo ", ImVec4(75.0/255.0, 0, 130/255.0, 1))}
        };
};

} // ~namespace Code::ImGuiImPlot
