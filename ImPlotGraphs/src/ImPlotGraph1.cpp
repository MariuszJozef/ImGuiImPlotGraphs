#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <cmath>
#include "ImPlotGraph1.hpp"

namespace Code::ImGuiImPlot
{

extern const float PI;

Plot1::Plot1()
{
    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    x.reserve(plotPoints);
    y.reserve(plotPoints);
}

void Plot1::CalculatePlotCoordinates()
{
    x.clear();
    y.clear();

    xMin = -selectedXAxisDomain * PI;
    xMax = selectedXAxisDomain == 0 ? 2 * PI : selectedXAxisDomain * PI;

    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    xIncrement = (xMax - xMin) / (plotPoints - 1);
    double period = 2 * PI / wavelength;

    for (int i = 0; i < plotPoints; ++i) 
    {
        x.push_back(xMin + i * xIncrement);
        y.push_back(amplitude * std::sin(period * (x.at(i) - xShift)) + yShift);
    }

    const auto [min, max] = std::minmax_element(begin(y), end(y));
    yMin = std::floor(*min);
    yMax = std::ceil(*max);
    // yMin = yMin < 0 ? std::floor(*min - 0.5) + 0.5 : std::floor(*min + 0.5) - 0.5;
    // yMax = yMax < 0 ? std::ceil(*max - 0.5) + 0.5 : std::ceil(*max + 0.5) - 0.5;
}

bool Plot1::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};
    static int selectedXAxisDomainOld {selectedXAxisDomain};
    static float amplitudeOld {amplitude};
    static float wavelengthOld {wavelength};
    static float xShiftOld {xShift};
    static float yShiftOld {yShift};

    if (plotPointsPerUnitLengthOld != plotPointsPerUnitLength)
    {
        plotPointsPerUnitLengthOld = plotPointsPerUnitLength;
        isNeedReplot = true;
    }
    else if (selectedXAxisDomainOld != selectedXAxisDomain)
    {
        selectedXAxisDomainOld = selectedXAxisDomain;
        isNeedReplot = true;
    }
    else if (amplitudeOld != amplitude)
    {
        amplitudeOld = amplitude;
        isNeedReplot = true;
    }
    else if (wavelengthOld != wavelength)
    {
        wavelengthOld = wavelength;
        isNeedReplot = true;
    }
    else if (xShiftOld != xShift)
    {
        xShiftOld = xShift;
        isNeedReplot = true;
    }
    else if (yShiftOld != yShift)
    {
        yShiftOld = yShift;
        isNeedReplot = true;
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
}

void Plot1::GenerateXTicks()
{
    xTicks.clear();
    int xTicksCount {selectedXAxisDomain == 0 ? 5 : 4 * selectedXAxisDomain + 1};

    for (int i = 0; i < xTicksCount; i++)
    {
        double nextTick {xMin + i * PI / 2};
        xTicks.push_back(nextTick);
    }
}

std::vector<const char*> Plot1::GenerateXLabels()
{
    xLabels.clear();

    switch (selectedXAxisDomain)
    {
    case 0:
        return std::vector<const char*> {"0", "PI/2", "PI", "3PI/2", "2PI"};
    case 1:
        return std::vector<const char*> {"-PI", "-PI/2", "0", "PI/2", "PI"};
    case 2:
        return std::vector<const char*> {"-2PI", "-3PI/2", "-PI", "-PI/2", "0", "PI/2", "PI", "3PI/2", "2PI"};
    case 3:
        return std::vector<const char*> {"-3PI", "-5PI/2", "-2PI", "-3PI/2", "-PI", "-PI/2", "0", "PI/2", "PI", "3PI/2", "2PI", "5PI/2", "3PI"};
    case 4:
        return std::vector<const char*> {"-4PI", "-7PI/2", "-3PI", "-5PI/2", "-2PI", "-3PI/2", "-PI", "-PI/2", "0", "PI/2", "PI", "3PI/2", "2PI", "5PI/2", "3PI", "7PI/2", "4PI"};
    case 5:
        return std::vector<const char*> {"-5PI", "-9PI/2", "-4PI", "-7PI/2", "-3PI", "-5PI/2", "-2PI", "-3PI/2", "-PI", "-PI/2", "0", "PI/2", "PI", "3PI/2", "2PI", "5PI/2", "3PI", "7PI/2", "4PI", "9PI/2", "5PI"};
    default:
        throw std::runtime_error("Failure: GenerateXLabels: selectedXAxisDomain out of Domain");
    }
}

void Plot1::GenerateYTicksAndYLabels()
{
    yTicks.clear();
    yLabels.clear();

    yTicks.push_back(yMin);
    int i = 0;
    std::snprintf(&yLabelsThatWork[i][0], yLabelBufferSize, "%.2f", yMin);
    yLabels.push_back(&yLabelsThatWork[i][0]);

    for (double nextTick = yMin + 0.5; nextTick < yMax; nextTick += 0.5)
    {
        yTicks.push_back(nextTick);
        i++;
        std::snprintf(&yLabelsThatWork[i][0], yLabelBufferSize, "%.2f", nextTick);
        yLabels.push_back(&yLabelsThatWork[i][0]);
    }

    if (yMax != yMin) // i.e. amplitude != 0
    {
        yTicks.push_back(yMax);
        i++;
        std::snprintf(&yLabelsThatWork[i][0], yLabelBufferSize, "%.2f", yMax);
        yLabels.push_back(&yLabelsThatWork[i][0]);
    }
}

void Plot1::Graph()
{
    if (CheckIsNeedReplot())
    {
        CalculatePlotCoordinates();
        GenerateXTicks();
        xLabels = GenerateXLabels();
        GenerateYTicksAndYLabels();

        if (xTicks.size() != xLabels.size())
        {
            std::stringstream message;
            message << "Error: Graph: Size mismatch! xTicks.size(): " << xTicks.size() 
                << ", xLabels.size(): " << xLabels.size();

            throw std::runtime_error(message.str());
        }

        if (yTicks.size() != yLabels.size())
        {
            std::stringstream message;
            message << "Error: Graph: Size mismatch! yTicks.size(): " << yTicks.size() 
                << ", yLabels.size(): " << yLabels.size();

            throw std::runtime_error(message.str());
        }
    }

    if (ImPlot::BeginPlot("Sinusoid", ImVec2(-1, -1), ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupAxis(ImAxis_X1, "x axis");
        ImPlot::SetupAxis(ImAxis_Y1, "y axis");

        if (isZoom)
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_None);
        }
        else
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_Always);
        }

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), xTicks.size(), xLabels.data(), false);
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks.data(), yTicks.size(), yLabels.data(), false);

        static float thickness = 3;
        ImPlot::SetNextLineStyle(plotColour.at(selectedPlotColour).second, thickness);

        if (isMarkers)
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond);

        ImPlot::PlotLine("y = A sin(2PI/B (x - C)) + D", x.data(), y.data(), plotPoints);
        ImPlot::EndPlot();
    }
}

} // ~namespace Code::ImGuiImPlot
