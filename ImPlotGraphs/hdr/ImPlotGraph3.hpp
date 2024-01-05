#pragma once
#include <numeric>
#include <cmath>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include "ImPlotGeneric.hpp"

namespace Code::ImGuiImPlot
{
    
extern const float PI;

class Plot3 : public PlotGeneric
{
public:
    Plot3();
    void Graph() override;

    float* SetSlitWidthViaPtr() { return &slitWidthMm; }
    int* SetSlitNumberViaPtr() { return &slitNumber; }
    float* SetSlitSeparationViaPtr() { return &slitSeparationMm; }
    float* SetScreenPerpDistanceViaPtr() { return &screenPerpDistanceMm; }

    int* SetSelectedLightViaPtr() { return &selectedLight; }
    std::string GetLighColourName() { return std::get<0>(lightColourAndWavelength.at(selectedLight)); }
    int GetLighColourAndWavelengthSize() { return lightColourAndWavelength.size(); }

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;

    double DiffractionPhase(double x);
    double InterferencePhaseWithoutSlitNumber(double x);
    double DiffractionIntensitySingleSlit(double x);
    double InterferenceIntensityMultiSlit(double x);

private:
    std::vector<double> x;
    std::vector<double> y1;
    std::vector<double> y2;
    std::vector<double> y3;
    std::vector<double> xTicks;
    std::vector<double> yTicks;
    std::vector<const char*> xLabels;
    std::vector<const char*> yLabels;

    float slitWidthMm {0.05F};  // float because its is adjusted in ImGui::SliderFloat
    int slitNumber {2};
    float slitSeparationMm {2 * slitWidthMm};
    float screenPerpDistanceMm {500.0F};
    const double maxAngleToObservationPointDeg {2.0};

    double xMax {screenPerpDistanceMm * tanf(PI / 180 * maxAngleToObservationPointDeg)};
    double xMin {-xMax};
    double yMin {0.0};
    double yMax {0.0};

    std::map<int, std::tuple<std::string, float, ImVec4>> lightColourAndWavelength
        {
            {0, std::make_tuple("wavelength: 700 nm (red)    ", 7.0E-4, ImVec4(1, 0, 0, 1))},
            // {1, std::make_tuple("wavelength: 600 nm (orange) ", 6.0E-4, ImVec4(1, 127.0/255, 80.0/255, 1))},
            {1, std::make_tuple("wavelength: 600 nm (orange) ", 6.0E-4, ImVec4(1, 140.0/255, 0, 1))},
            // {1, std::make_tuple("wavelength: 600 nm (orange) ", 6.0E-4, ImVec4(1, 165.0/255, 0, 1))},
            {2, std::make_tuple("wavelength: 580 nm (yellow) ", 5.8E-4, ImVec4(1, 1, 0, 1))},
            {3, std::make_tuple("wavelength: 530 nm (green)  ", 5.3E-4, ImVec4(0, 1, 0, 1))},
            {4, std::make_tuple("wavelength: 510 nm (blue)   ", 5.1E-4, ImVec4(0, 0, 1, 1))},
            {5, std::make_tuple("wavelength: 430 nm (indigo) ", 4.3E-4, ImVec4(148, 0, 211, 1))},
            {6, std::make_tuple("wavelength: 390 nm (violet) ", 3.9E-4, ImVec4(75.0/255.0, 0, 130/255.0, 1))}
        };

    int selectedLight {5};
    float wavelengthMm = std::get<1>(lightColourAndWavelength.at(selectedLight));
};

} // ~namespace Code::ImGuiImPlot
