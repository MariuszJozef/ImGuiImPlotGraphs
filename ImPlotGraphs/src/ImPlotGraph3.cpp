#include <cmath>
#include <algorithm>
#include "implot.h"
#include "ImPlotGraph3.hpp"

namespace Code::ImGuiImPlot
{

extern const float PI;

Plot3::Plot3()
{
    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    x.reserve(plotPoints);
    y1.reserve(plotPoints);
    y2.reserve(plotPoints);
    y3.reserve(plotPoints);
}

void Plot3::CalculatePlotCoordinates()
{
    x.clear();
    y1.clear();
    y2.clear();
    y3.clear();

    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    xIncrement = (xMax - xMin) / (plotPoints - 1);

    for (int i = 0; i < plotPoints; ++i) 
    {
        x.push_back(xMin + i * xIncrement);
/*
        y1.push_back(DiffractionIntensitySingleSlit(x.at(i)));
        y2.push_back(InterferenceIntensityMultiSlit(x.at(i)));
        y3.push_back(y1.at(i) * y2.at(i));
*/
        double diffractionIntensity = DiffractionIntensitySingleSlit(x.at(i));
        double interferenceIntensity = InterferenceIntensityMultiSlit(x.at(i));
        y1.push_back(diffractionIntensity);
        y2.push_back(interferenceIntensity);
        y3.push_back(diffractionIntensity * interferenceIntensity);
    }

    auto [min, max] = std::minmax_element(begin(y3), end(y3));
    yMin = std::floor(*min);
    yMax = std::ceil(*max);
}

bool Plot3::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};
    static int selectedLightOld {selectedLight};
    static float slitWidthMmOld {slitWidthMm};
    static int slitNumberOld {slitNumber};
    static float slitSeparationMmOld {slitSeparationMm};
    static float screenPerpDistanceMmOld {screenPerpDistanceMm};

    if (plotPointsPerUnitLengthOld != plotPointsPerUnitLength)
    {
        plotPointsPerUnitLengthOld = plotPointsPerUnitLength;
        isNeedReplot = true;
    }
    else if (selectedLightOld != selectedLight)
    {
        selectedLightOld = selectedLight;
        wavelengthMm = std::get<1>(lightColourAndWavelength.at(selectedLight));
        isNeedReplot = true;
    }
    else if (slitWidthMmOld != slitWidthMm)
    {
        slitWidthMmOld = slitWidthMm;
        isNeedReplot = true;
    }
    else if (slitNumberOld != slitNumber)
    {
        slitNumberOld = slitNumber;
        isNeedReplot = true;
    }
    else if (slitSeparationMmOld != slitSeparationMm)
    {
        slitSeparationMmOld = slitSeparationMm;
        isNeedReplot = true;
    }
    else if (screenPerpDistanceMmOld != screenPerpDistanceMm)
    {
        screenPerpDistanceMmOld = screenPerpDistanceMm;
        isNeedReplot = true;
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
}

void Plot3::Graph()
{
    if (CheckIsNeedReplot())
    {
        CalculatePlotCoordinates();
    }

    if (ImPlot::BeginPlot("Combined Diffraction and Interference of Light due to Multiple Slits", ImVec2(-1, -1), ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupAxis(ImAxis_X1, "Distance x (mm) along viewing screen subtended by angle 'theta'");
        ImPlot::SetupAxis(ImAxis_Y1, "Relative intensity of light: I('theta')/I(0)");

        if (isZoom)
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_None);
        }
        else
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_Always);
        }

        static double xTicks[] = {-15, -10, -5, 0, 5, 10, 15};
        static const char*  xLabels[] = {"-15", "-10", "-5", "0", "5", "10", "15"};
        static double yTicks[] = {0, 0.5, 1};
        static const char*  yLabels[] = {"0", "0.5", "1"};

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks, 7, xLabels, false);
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks, 3, yLabels, false);

        // ImPlot::SetNextLineStyle(ImVec4(188.0/255, 143.0/255, 143.0/255, 1), 3.0F);
        ImPlot::SetNextLineStyle(ImVec4(128.0/255, 128.0/255, 128.0/255, 1), 3.0F);
        ImPlot::PlotLine("Single Slit Diffraction", x.data(), y1.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(139.0/255, 69.0/255, 19.0/255, 1), 2.5F);
        ImPlot::PlotLine("Multi Slit Interference", x.data(), y2.data(), plotPoints);

        ImVec4 colour = std::get<2>(lightColourAndWavelength.at(selectedLight));
        ImPlot::SetNextLineStyle(colour, 3.5F);
        ImPlot::PlotLine("Diffraction with Interferance", x.data(), y3.data(), plotPoints);
        ImPlot::EndPlot();
    }
}

double Plot3::DiffractionPhase(double x)
{
    return PI * slitWidthMm * x / wavelengthMm 
        / std::sqrt(screenPerpDistanceMm * screenPerpDistanceMm + x * x);
}

double Plot3::InterferencePhaseWithoutSlitNumber(double x)
{
    return PI * slitSeparationMm * x / wavelengthMm 
        / std::sqrt(screenPerpDistanceMm * screenPerpDistanceMm + x * x);
}

double Plot3::DiffractionIntensitySingleSlit(double x)
{
    if (slitNumber == 0)
        return 0;

    if (x != 0)
    {
        double diffractionAmplitude = std::sin(DiffractionPhase(x)) / DiffractionPhase(x);
        return diffractionAmplitude * diffractionAmplitude;
    }
    else
    {
        return 1;
    }
}

double Plot3::InterferenceIntensityMultiSlit(double x)
{
    if (slitNumber == 0)
        return 0;

    if (x != 0)
    {
        double interferenceAmplitude = std::sin(slitNumber * InterferencePhaseWithoutSlitNumber(x)) 
            / slitNumber / std::sin(InterferencePhaseWithoutSlitNumber(x));

        return interferenceAmplitude * interferenceAmplitude;
    }
    else
    {
        return 1;
    }
}

} // ~namespace Code::ImGuiImPlot
