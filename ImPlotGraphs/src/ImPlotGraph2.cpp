#include <numeric>
#include <cmath>
#include <array>
#include <algorithm>
#include <iostream>
#include "implot.h"
#include "ImPlotGraph2.hpp"

namespace Code::ImGuiImPlot
{

extern const float PI;

Plot2::Plot2()
{
    plotPointsPerUnitLength = 50;
    plotPoints1 = std::round(plotPointsPerUnitLength * (x1Max - x1Min));
    plotPoints2 = std::round(plotPointsPerUnitLength * (x2Max - x2Min));
    x1.reserve(plotPoints1);
    x2.reserve(plotPoints2);
    y1.reserve(plotPoints1);
    y2.reserve(plotPoints1);
    y3.reserve(plotPoints2);
    y4.reserve(plotPoints2);
}

void Plot2::CalculatePlotCoordinates()
{
    x1.clear();
    y1.clear();
    y2.clear();

    plotPoints1 = std::round(plotPointsPerUnitLength * (x1Max - x1Min));
    x1Increment = (x1Max - x1Min) / (plotPoints1 - 1);

    for (int i = 0; i < plotPoints1; ++i) 
    {
        x1.push_back(x1Min + i * x1Increment);

        if (x1.at(i) < -width / 2 || x1.at(i) > width / 2)
        {
            y1.push_back(0.0F);    
            y2.push_back(0.0F);    
        }
        else
        {
            // square pulse
            y1.push_back(amplitude);
            // triangle pulse
            y2.push_back(amplitude - 2 * amplitude  / width * std::abs(x1.at(i)));
        }
    }

    x2.clear();
    y3.clear();
    y4.clear();

    plotPoints2 = std::round(plotPointsPerUnitLength * (x2Max - x2Min));
    x2Increment = (x2Max - x2Min) / (plotPoints2 - 1);

    for (int i = 0; i < plotPoints2; ++i) 
    {
        x2.push_back(x2Min + i * x2Increment);

        if (x2.at(i) != 0)
        {
            // Fourier transform of square pulse
            y3.push_back(amplitude / PI / x2.at(i) * std::sin(x2.at(i) * width / 2));

            // Fourier transform of triangle pulse
            double sine = std::sin(x2.at(i) * width / 4);
            y4.push_back(4 * amplitude / PI / width / x2.at(i) / x2.at(i) * sine * sine);
        }
        else
        {
            y3.push_back(amplitude * width / 2 / PI);
            y4.push_back(amplitude * width / 4 / PI);
        }
    }
}

bool Plot2::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};
    static float amplitudeOld {amplitude};
    static float widthOld {width};

    if (plotPointsPerUnitLengthOld != plotPointsPerUnitLength)
    {
        plotPointsPerUnitLengthOld = plotPointsPerUnitLength;
        isNeedReplot = true;
    }
    else if (amplitudeOld != amplitude)
    {
        amplitudeOld = amplitude;
        isNeedReplot = true;
    }
    else if (widthOld != width)
    {
        widthOld = width;
        isNeedReplot = true;
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
}

void Plot2::Graph()
{
    if (CheckIsNeedReplot())
    {
        CalculatePlotCoordinates();
    }

    if (ImPlot::BeginSubplots("##Fourier Transform Pair Subplots", 2, 2, ImVec2(-1, -1)))
    {
        if (ImPlot::BeginPlot("Square Pulse", ImVec2(), ImPlotFlags_Crosshairs))
        {
            ImPlot::SetupAxis(ImAxis_X1, "x axis");
            ImPlot::SetupAxis(ImAxis_Y1, "y axis");
            ImPlot::SetupAxis(ImAxis_X2, nullptr, ImPlotAxisFlags_Opposite);
            ImPlot::SetupAxis(ImAxis_Y2, nullptr, ImPlotAxisFlags_Opposite);

            const auto [min, max] = std::minmax_element(begin(y1), end(y1));
            y1Min = std::floor(*min);
            y1Max = std::ceil(*max);

            if (isZoom)
            {
                ImPlot::SetupAxesLimits(x1Min - gap, x1Max + gap, y1Min - gap, y1Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_X2, x1Min - gap, x1Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y1Min - gap, y1Max + gap, ImPlotCond_None);
            }
            else
            {
                ImPlot::SetupAxesLimits(x1Min - gap, x1Max + gap, y1Min - gap, y1Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_X2, x1Min - gap, x1Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y1Min - gap, y1Max + gap, ImPlotCond_Always);
            }

            double x2Ticks[] = {-width/2, width/2};
            const char*  x2Labels[] = {"-a/2", "a/2"};
            double y2Ticks[] = {amplitude};
            const char*  y2Labels[] = {"A"};

            ImPlot::SetupAxisTicks(ImAxis_X2, x2Ticks, 2, x2Labels, false);
            ImPlot::SetupAxisTicks(ImAxis_Y2, y2Ticks, 1, y2Labels, false);

            ImPlot::SetNextLineStyle(ImVec4(1, 1, 0, 1), 3.0F);
            ImPlot::PlotLine("##y = A for -a/2 <= x <= a/2", x1.data(), y1.data(), plotPoints1);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Fourier Transform of Square Pulse", ImVec2(), ImPlotFlags_Crosshairs))
        {
            ImPlot::SetupAxis(ImAxis_X1, "frequency axis");
            ImPlot::SetupAxis(ImAxis_X2, nullptr, ImPlotAxisFlags_Opposite);
            ImPlot::SetupAxis(ImAxis_Y2, nullptr, ImPlotAxisFlags_Opposite);
            
            const auto [min, max] = std::minmax_element(begin(y3), end(y3));
            y3Min = *min;
            y3Max = *max;

            if (isZoom)
            {
                ImPlot::SetupAxesLimits(x2Min - gap, x2Max + gap, y3Min - gap, y3Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_X2, x2Min - gap, x2Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y3Min - gap, y3Max + gap, ImPlotCond_None);
            }
            else
            {
                ImPlot::SetupAxesLimits(x2Min - gap, x2Max + gap, y3Min - gap, y3Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_X2, x2Min - gap, x2Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y3Min - gap, y3Max + gap, ImPlotCond_Always);
            }

            static const int x2TicksCount {4};
            static constexpr int y2TicksCount {1};
            std::array<double, x2TicksCount> x2Ticks {
                -8 * PI / width, 
                -4 * PI / width, 
                4 * PI / width,
                8 * PI / width
            };

            std::array<const char*, x2TicksCount> x2Labels {
                "-8PI/a", "-4PI/a", "4PI/a", "8PI/a"
            };

            std::array<double, y2TicksCount> y2Ticks {
                amplitude * width / 2 / PI
            };

            std::array<const char*, y2TicksCount> y2Labels {
                "A*a/(2PI)"
            };

            ImPlot::SetupAxisTicks(ImAxis_X2, x2Ticks.data(), x2TicksCount, x2Labels.data(), false);
            ImPlot::SetupAxisTicks(ImAxis_Y2, y2Ticks.data(), y2TicksCount, y2Labels.data(), false);

            ImPlot::SetNextLineStyle(ImVec4(0, 1, 0, 1), 3.0F);
            ImPlot::PlotLine("##y = ", x2.data(), y3.data(), plotPoints2);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Triangle Pulse", ImVec2(), ImPlotFlags_Crosshairs))
        {
            ImPlot::SetupAxis(ImAxis_X1, "x axis");
            ImPlot::SetupAxis(ImAxis_Y1, "y axis");
            ImPlot::SetupAxis(ImAxis_X2, nullptr, ImPlotAxisFlags_Opposite);
            ImPlot::SetupAxis(ImAxis_Y2, nullptr, ImPlotAxisFlags_Opposite);

            const auto [min, max] = std::minmax_element(begin(y2), end(y2));
            y2Min = std::floor(*min);
            y2Max = std::ceil(*max);

            if (isZoom)
            {
                ImPlot::SetupAxesLimits(x1Min - gap, x1Max + gap, y2Min - gap, y2Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_X2, x1Min - gap, x1Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y2Min - gap, y2Max + gap, ImPlotCond_None);
            }
            else
            {
                ImPlot::SetupAxesLimits(x1Min - gap, x1Max + gap, y2Min - gap, y2Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_X2, x1Min - gap, x1Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y2Min - gap, y2Max + gap, ImPlotCond_Always);
            }

            double x2Ticks[] = {-width/2, width / 2};
            const char*  x2Labels[] = {"-a/2", "a/2"};
            double y2Ticks[] = {amplitude};
            const char*  y2Labels[] = {"A"};

            ImPlot::SetupAxisTicks(ImAxis_X2, x2Ticks, 2, x2Labels, false);
            ImPlot::SetupAxisTicks(ImAxis_Y2, y2Ticks, 1, y2Labels, false);
            
            ImPlot::SetNextLineStyle(ImVec4(1, 0, 1, 1), 3.0F);
            ImPlot::PlotLine("##y = A^2 a/2 - 2 A^2 / a |x|", x1.data(), y2.data(), plotPoints1);
            ImPlot::EndPlot();
        }

        if (ImPlot::BeginPlot("Fourier Transform of Triangle Pulse", ImVec2(), ImPlotFlags_Crosshairs))
        {
            ImPlot::SetupAxis(ImAxis_X1, "frequency axis");
            ImPlot::SetupAxis(ImAxis_X2, nullptr, ImPlotAxisFlags_Opposite);
            ImPlot::SetupAxis(ImAxis_Y2, nullptr, ImPlotAxisFlags_Opposite);

            const auto [min, max] = std::minmax_element(begin(y4), end(y4));
            y4Min = *min;
            y4Max = *max;

            if (isZoom)
            {
                ImPlot::SetupAxesLimits(x2Min - gap, x2Max + gap, y4Min - gap, y4Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_X2, x2Min - gap, x2Max + gap, ImPlotCond_None);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y4Min - gap, y4Max + gap, ImPlotCond_None);
            }
            else
            {
                ImPlot::SetupAxesLimits(x2Min - gap, x2Max + gap, y4Min - gap, y4Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_X2, x2Min - gap, x2Max + gap, ImPlotCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y2, y4Min - gap, y4Max + gap, ImPlotCond_Always);
            }

            static const int x2TicksCount {4};
            static constexpr int y2TicksCount {1};
            std::array<double, x2TicksCount> x2Ticks {
                -8 * PI / width, 
                -4 * PI / width, 
                4 * PI / width,
                8 * PI / width
            };

            std::array<const char*, x2TicksCount> x2Labels {
                "-8PI/a", "-4PI/a", "4PI/a", "8PI/a"
            };

            std::array<double, y2TicksCount> y2Ticks {
                amplitude * width / 4 / PI
            };

            std::array<const char*, y2TicksCount> y2Labels {
                "A*a/(4PI)"
            };

            ImPlot::SetupAxisTicks(ImAxis_X2, x2Ticks.data(), x2TicksCount, x2Labels.data(), false);
            ImPlot::SetupAxisTicks(ImAxis_Y2, y2Ticks.data(), y2TicksCount, y2Labels.data(), false);

            ImPlot::SetNextLineStyle(ImVec4(0.5, 0.3, 0, 1), 3.0F);
            ImPlot::PlotLine("##y = ", x2.data(), y4.data(), plotPoints2);
            ImPlot::EndPlot();
        }
        ImPlot::EndSubplots();
    }
}

} // ~namespace Code::ImGuiImPlot
