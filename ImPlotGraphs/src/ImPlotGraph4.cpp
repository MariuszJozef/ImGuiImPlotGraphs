#include <cmath>
#include <string>
#include <algorithm>
#include <utility>
#include "implot.h"
#include "ImPlotGraph4.hpp"

namespace Code::ImGuiImPlot
{

Plot4::Plot4()
{
    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    x.reserve(plotPoints);
    y1.reserve(plotPoints);
    y2.reserve(plotPoints);
    y3.reserve(plotPoints);
    y4.reserve(plotPoints);
    y5.reserve(plotPoints);
}

void Plot4::CalculatePlotCoordinates()
{
    x.clear();
    y1.clear();
    y2.clear();
    y3.clear();
    y4.clear();
    y5.clear();

    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    xIncrement = (xMax - xMin) / (plotPoints - 1);

    // TRIAL AND ERROR CORRECTIONS TO INITIAL POSITION OF GHOST SOLITON
    double adjust2ndGhostInitialPosition = phaseShift[1] + ghostPositionCorrection.at(0);
    double adjust3rdGhostInitialPosition = phaseShift[2] + ghostPositionCorrection.at(1);
    double adjust4thGhostInitialPosition = phaseShift[3] + ghostPositionCorrection.at(2);

    time = fTime;

    for (int i = 0; i < plotPoints; ++i) 
    {
        x.push_back(xMin + i * xIncrement);
        y1.push_back(Soliton(x.at(i), time));
        y2.push_back(SolitonGhost1(x.at(i), time, waveNumber[0], phaseShift[0]));
        y3.push_back(SolitonGhost2(x.at(i), time, waveNumber[1], adjust2ndGhostInitialPosition));
        y4.push_back(SolitonGhost2(x.at(i), time, waveNumber[2], adjust3rdGhostInitialPosition));
        y5.push_back(SolitonGhost1(x.at(i), time, waveNumber[3], adjust4thGhostInitialPosition));
    }

    if (time == 0.0)
    {
        auto max = std::max_element(begin(y1), end(y1));
        yMax = std::ceil(*max);
    }
}

bool Plot4::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};
    static float fTimeOld {fTime};
    static std::vector<float> waveNumberOld {waveNumber};
    static std::vector<float> phaseShiftOld {phaseShift};
    static std::vector<float> ghostPositionCorrectionOld {ghostPositionCorrection};
    static int selectedSolitonComboItemOld {selectedSolitonComboItem};

    if (plotPointsPerUnitLengthOld != plotPointsPerUnitLength)
    {
        plotPointsPerUnitLengthOld = plotPointsPerUnitLength;
        isNeedReplot = true;
    }
    else if (fTimeOld != fTime)
    {
        fTimeOld = fTime;
        isNeedReplot = true;
    }
    else if (waveNumberOld != waveNumber)
    {
        waveNumberOld = waveNumber;
        isNeedReplot = true;
    }
    else if (phaseShiftOld != phaseShift)
    {
        phaseShiftOld = phaseShift;
        isNeedReplot = true;
    }
    else if (ghostPositionCorrectionOld != ghostPositionCorrection)
    {
        ghostPositionCorrectionOld = ghostPositionCorrection;
        isNeedReplot = true;
    }
    else if (selectedSolitonComboItemOld != selectedSolitonComboItem)
    {
        selectedSolitonComboItemOld = selectedSolitonComboItem;
        isNeedReplot = true;
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
}

auto Plot4::GenerateXTicksAndXLabels()
{
    std::pair<std::vector<double>, std::vector<const char*>> ret {};

    auto& xTicks = ret.first;
    auto& xLabels = ret.second;

    xTicks = std::vector<double> {-40, -30, -20, -10, 0, 10, 20, 30, 40};
    xLabels = std::vector<const char*> {"-40", "-30", "-20", "-10", "0", "10", "20", "30", "40"};

    return ret;
}

auto Plot4::GenerateYTicksAndYLabels()
{
    std::pair<std::vector<double>, std::vector<const char*>> ret {};

    auto& yTicks = ret.first;
    auto& yLabels = ret.second;

    if (yMax <= 0.5)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5"};
    }
    else if (yMax <= 1)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1"};
    }
    else if (yMax <= 1.5)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1, 1.5};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1", "1.5"};
    }
    else if (yMax <= 2)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1, 1.5, 2};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1", "1.5", "2"};
    }
    else if (yMax <= 2.5)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1", "1.5", "2", "2.5"};
    }
    else if (yMax <= 3)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1", "1.5", "2", "2.5", "3"};
    }
    else if (yMax <= 3.5)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1", "1.5", "2", "2.5", "3", "3.5"};
    }
    else if (yMax <= 4)
    {
        yTicks = std::vector<double> {-1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4};
        yLabels = std::vector<const char*> {"-1", "-0.5", "0", "0.5", "1", "1.5", "2", "2.5", "3", "3.5", "4"};
    }

    return ret;
}

void Plot4::Graph()
{
    if (CheckIsNeedReplot())
    {
        CalculatePlotCoordinates();
    }

    if (ImPlot::BeginPlot("Collision of solitons: an (exact) N-soliton solution u(x, t) of nonlinear KdV equation: u_t + 6 u * u_x + u_{xxx} = 0", ImVec2(-1, -1), ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupAxis(ImAxis_X1, "x axis (arbitrary units)");
        ImPlot::SetupAxis(ImAxis_Y1, "y axis (arbitrary units)");

        double yMinExtra = yMin - 0.5;
        double yMaxExtra = yMax - integrationConst;

        if (yMax <= 1)
            yMaxExtra += 0.25;
        else if (yMax <= 2)
            yMaxExtra += 0.5;
        else if (yMax <= 3)
            yMaxExtra += 0.75;
        else if (yMax <= 4)
            yMaxExtra += 1;

        if (isZoom)
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMinExtra - gap, yMaxExtra + gap, ImPlotCond_None);
        }
        else
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMinExtra - gap, yMaxExtra + gap, ImPlotCond_Always);
        }

        auto [xTicks, xLabels] = GenerateXTicksAndXLabels();
        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), xTicks.size(), xLabels.data(), false);

        auto [yTicks, yLabels] = GenerateYTicksAndYLabels();
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks.data(), yTicks.size(), yLabels.data(), false);

        std::string title {};
        if (GetSolitonCount() == 2)
            title = "Two interacting solitons: nonlinear, non-additive, phase shifted after collision";
        else if (GetSolitonCount() == 3)
            title = "Three interacting solitons: nonlinear, non-additive, phase shifted after collision";
        else if (GetSolitonCount() == 4)
            title = "Four interacting solitons: nonlinear, non-additive, phase shifted after collision";

        ImPlot::SetNextLineStyle(ImVec4(1, 1, 0, 1), 4.0F);
        ImPlot::PlotLine(title.c_str(), x.data(), y1.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(1, 192.0/255, 203.0/255, 1), 1.5F);
        ImPlot::PlotLine("Noninteracting soliton \"ghost\" 1", x.data(), y2.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(1, 105.0/255, 180.0/255, 1), 1.5F);
        ImPlot::PlotLine("Noninteracting soliton \"ghost\" 2", x.data(), y3.data(), plotPoints);

        if (GetSolitonCount() >= 3)
        {
            ImPlot::SetNextLineStyle(ImVec4(1, 20.0/255, 147.0/255, 1), 1.5F);
            ImPlot::PlotLine("Noninteracting soliton \"ghost\" 3", x.data(), y4.data(), plotPoints);
        }

        if (GetSolitonCount() == 4)
        {
            ImPlot::SetNextLineStyle(ImVec4(1, 10.0/255, 105.0/255, 1), 1.5F);
            ImPlot::PlotLine("Noninteracting soliton \"ghost\" 4", x.data(), y5.data(), plotPoints);
        }

        ImPlot::EndPlot();
    }
}

double Plot4::CouplingA(int i, int j)
{
    return std::pow(waveNumber[i] - waveNumber[j], 2) / std::pow(waveNumber[i] + waveNumber[j], 2);
}

double Plot4::CouplingAA(int i, int j)
{
    return std::pow(waveNumber[i] - waveNumber[j], 2) / (waveNumber[i] + waveNumber[j]);
}

double Plot4::CouplingAAA(int i, int j)
{
    return std::pow(waveNumber[i] - waveNumber[j], 2);
}

double Plot4::CouplingB(int i, int j, int k)
{
    return CouplingA(i, j) * CouplingA(i, k) * CouplingA(j, k);
}

double Plot4::CouplingBB(int i, int j, int k)
{
    return CouplingB(i, j, k) * (waveNumber[i] + waveNumber[j] + waveNumber[k]);
}

double Plot4::CouplingBBB(int i, int j, int k)
{
    return CouplingB(i, j, k) * std::pow(waveNumber[i] + waveNumber[j] + waveNumber[k], 2);
}

double Plot4::CouplingC(int i, int j, int k, int l)
{
    return CouplingA(i, j) * CouplingA(i, k) * CouplingA(i, l)
        * CouplingA(j, k) * CouplingA(j, l) * CouplingA(k, l);
}

double Plot4::CouplingCC(int i, int j, int k, int l)
{
    return CouplingC(i, j, k, l) * (waveNumber[i] + waveNumber[j] + waveNumber[k] + waveNumber[l]);
}

double Plot4::CouplingCCC(int i, int j, int k, int l)
{
    return CouplingC(i, j, k, l) * std::pow(waveNumber[i] + waveNumber[j] + waveNumber[k] + waveNumber[l], 2);
}

std::vector<double> Plot4::PhaseArg(double x, double t)
{
    std::vector<double> phaseArg(GetSolitonCount());

    for (long unsigned i = 0; i < GetSolitonCount(); ++i)
    {
        phaseArg[i] = waveNumber[i] * (x - waveNumber[i] * waveNumber[i] * t - phaseShift[i]);
    }

    return phaseArg;
}

double Plot4::PhaseArg(double x, double t, double waveNumber_, double phaseShift_)
{
    return waveNumber_ * (x - waveNumber_ * waveNumber_ * t - phaseShift_);
}

double Plot4::Numerator1(double x, double t)
{
    double numerator {0.0};

    for (long unsigned i = 0; i < GetSolitonCount(); i++)
    {
        numerator += waveNumber[i] * waveNumber[i] * std::exp(PhaseArg(x, t)[i]);
    }

    numerator += CouplingAAA(0, 1) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1]);

    if (GetSolitonCount() >= 3)
    {
        numerator += CouplingAAA(0, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2]);
        numerator += CouplingAAA(1, 2) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
        
        numerator += CouplingBBB(0, 1, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    }

    if (GetSolitonCount() == 4)
    {
        numerator += CouplingAAA(0, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[3]);
        numerator += CouplingAAA(1, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
        numerator += CouplingAAA(2, 3) * std::exp(PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        
        numerator += CouplingBBB(0, 1, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
        numerator += CouplingBBB(0, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        numerator += CouplingBBB(1, 2, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    
        numerator += CouplingCCC(0, 1, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    }

    return numerator;
}

double Plot4::Numerator2(double x, double t)
{
    double numerator {0.0};

    for (long unsigned i = 0; i < GetSolitonCount(); i++)
    {
        numerator += waveNumber[i] * std::exp(PhaseArg(x, t)[i]);
    }

    numerator += CouplingAA(0, 1) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1]);

    if (GetSolitonCount() >= 3)
    {
        numerator += CouplingAA(0, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2]);
        numerator += CouplingAA(1, 2) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);

        numerator += CouplingBB(0, 1, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    }

    if (GetSolitonCount() == 4)
    {
        numerator += CouplingAA(0, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[3]);
        numerator += CouplingAA(1, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
        numerator += CouplingAA(2, 3) * std::exp(PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        
        numerator += CouplingBB(0, 1, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
        numerator += CouplingBB(0, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        numerator += CouplingBB(1, 2, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        
        numerator += CouplingCC(0, 1, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    }

    return numerator;
}

double Plot4::Denominator(double x, double t)
{
    double denominator {1.0};

    for (long unsigned i = 0; i < GetSolitonCount(); i++)
    {
        denominator += std::exp(PhaseArg(x, t)[i]);
    }

    denominator += CouplingA(0, 1) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1]);

    if (GetSolitonCount() >= 3)
    {
        denominator += CouplingA(0, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2]);
        denominator += CouplingA(1, 2) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
        
        denominator += CouplingB(0, 1, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    }

    if (GetSolitonCount() == 4)
    {
        denominator += CouplingA(0, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[3]);
        denominator += CouplingA(1, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
        denominator += CouplingA(2, 3) * std::exp(PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        
        denominator += CouplingB(0, 1, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
        denominator += CouplingB(0, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        denominator += CouplingB(1, 2, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
        
        denominator += CouplingC(0, 1, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    }

    return denominator;
}

double Plot4::Soliton(double x, double t)
{
    return 2 * Numerator1(x, t) / Denominator(x, t)
        - 2 * std::pow(Numerator2(x, t) / Denominator(x, t), 2)
        + integrationConst;
}

double Plot4::SolitonGhost1(double x, double t, double waveNumber_, double phaseShift_)
{
    double expArg = std::exp(PhaseArg(x, t, waveNumber_, phaseShift_));
    return 2 * waveNumber_ * waveNumber_ * expArg / std::pow(1 + expArg, 2)
        + integrationConst;
}

double Plot4::SolitonGhost2(double x, double t, double waveNumber_, double phaseShift_)
{
    double sechArg = 1.0 / std::cosh(0.5 * PhaseArg(x, t, waveNumber_, phaseShift_));
    return 0.5 * waveNumber_ * waveNumber_ * sechArg * sechArg
        + integrationConst;
}

float Plot4::SolitonApproxTravelTime()
{
    std::vector<float> solitonSpeeds {};

    for (const auto& w : waveNumber)
    {
        solitonSpeeds.push_back(w * w + 6 * integrationConst);
    }

    std::vector<float> solitonTravelTimes {};
    for (int i = 0; i < solitonSpeeds.size(); ++i)
    {
        if (solitonSpeeds.at(i) == 0.0)
            continue;

        solitonTravelTimes.push_back((xMax - phaseShift.at(i)) / solitonSpeeds.at(i));
    }

    auto maxTravelTime = std::max_element(begin(solitonTravelTimes), end(solitonTravelTimes));

    return std::roundf(static_cast<float>(*maxTravelTime));
}

} // ~namespace Code::ImGuiImPlot
