#include <cmath>
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
    double adjust2ndGhostSolitonInitialPosition = phaseShift[1] + ghostPositionCorrection.at(0);
    double adjust3rdGhostSolitonInitialPosition = phaseShift[2] + ghostPositionCorrection.at(1);
    double adjust4thGhostSolitonInitialPosition = phaseShift[3] + ghostPositionCorrection.at(2);

    time = fTime;

    for (int i = 0; i < plotPoints; ++i) 
    {
        x.push_back(xMin + i * xIncrement);
        y1.push_back(Soliton(x.at(i), time));
        y2.push_back(SolitonGhost1(x.at(i), time, waveNumberK[0], phaseShift[0]));
        y3.push_back(SolitonGhost2(x.at(i), time, waveNumberK[1], adjust2ndGhostSolitonInitialPosition));
        y4.push_back(SolitonGhost2(x.at(i), time, waveNumberK[2], adjust3rdGhostSolitonInitialPosition));
        y5.push_back(SolitonGhost1(x.at(i), time, waveNumberK[3], adjust4thGhostSolitonInitialPosition));
    }
}

bool Plot4::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};
    static float fTimeOld {fTime};
    static std::vector<float> waveNumberKOld {waveNumberK};
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
    else if (waveNumberKOld != waveNumberK)
    {
        waveNumberKOld = waveNumberK;
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
        solitonInteraction = static_cast<SolitonInteraction>(selectedSolitonComboItem);
        isNeedReplot = true;
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
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

        static double yMax = 1.2 * Soliton(phaseShift[0], 0);

        if (isZoom)
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_None);
        }
        else
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, yMax + gap, ImPlotCond_Always);
        }

        static double xTicks[] = {-40, -30, -20, -10, 0, 10, 20, 30, 40};
        static const char*  xLabels[] = {"-40", "-30", "-20", "-10", "0", "10", "20", "30", "40"};
        static double yTicks[] = {0, 0.5, 1, 1.5, 2};
        static const char*  yLabels[] = {"0", "0.5", "1", "1.5", "2"};

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks, 9, xLabels, false);
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks, 5, yLabels, false);

        // ImPlot::SetNextLineStyle(ImVec4(193.0/255, 201.0/255, 83.0/255, 1), 4.0F);
        ImPlot::SetNextLineStyle(ImVec4(1, 1, 0, 1), 4.0F);
        ImPlot::PlotLine("Four interacting solitons: nonlinear, non-additive, phase shifted after collision", x.data(), y1.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(1, 192.0/255, 203.0/255, 1), 1.5F);
        // ImPlot::SetNextLineStyle(ImVec4(128.0/255, 128.0/255, 128.0/255, 1), 1.5F);
        ImPlot::PlotLine("Noninteracting soliton \"ghost\" 1", x.data(), y2.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(1, 105.0/255, 180.0/255, 1), 1.5F);
        // ImPlot::SetNextLineStyle(ImVec4(139.0/255, 69.0/255, 19.0/255, 1), 1.5F);
        ImPlot::PlotLine("Noninteracting soliton \"ghost\" 2", x.data(), y3.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(1, 20.0/255, 147.0/255, 1), 1.5F);
        // ImPlot::SetNextLineStyle(ImVec4(56.0/255, 83.0/255, 75.0/255, 1), 1.5F);
        ImPlot::PlotLine("Noninteracting soliton \"ghost\" 3", x.data(), y4.data(), plotPoints);

        ImPlot::SetNextLineStyle(ImVec4(1, 10.0/255, 105.0/255, 1), 1.5F);
        // ImPlot::SetNextLineStyle(ImVec4(25.0/255, 188.0/255, 55.0/255, 1), 1.5F);
        ImPlot::PlotLine("Noninteracting soliton \"ghost\" 4", x.data(), y5.data(), plotPoints);

        ImPlot::EndPlot();
    }
}

double Plot4::CouplingA(int i, int j)
{
    return std::pow(waveNumberK[i] - waveNumberK[j], 2) / std::pow(waveNumberK[i] + waveNumberK[j], 2);
}

double Plot4::CouplingAA(int i, int j)
{
    return std::pow(waveNumberK[i] - waveNumberK[j], 2) / (waveNumberK[i] + waveNumberK[j]);
}

double Plot4::CouplingAAA(int i, int j)
{
    return std::pow(waveNumberK[i] - waveNumberK[j], 2);
}

double Plot4::CouplingB(int i, int j, int k)
{
    return CouplingA(i, j) * CouplingA(i, k) * CouplingA(j, k);
}

double Plot4::CouplingBB(int i, int j, int k)
{
    return CouplingB(i, j, k) * (waveNumberK[i] + waveNumberK[j] + waveNumberK[k]);
}

double Plot4::CouplingBBB(int i, int j, int k)
{
    return CouplingB(i, j, k) * std::pow(waveNumberK[i] + waveNumberK[j] + waveNumberK[k], 2);
}

double Plot4::CouplingC(int i, int j, int k, int l)
{
    return CouplingA(i, j) * CouplingA(i, k) * CouplingA(i, l)
        * CouplingA(j, k) * CouplingA(j, l) * CouplingA(k, l);
}

double Plot4::CouplingCC(int i, int j, int k, int l)
{
    return CouplingC(i, j, k, l) * (waveNumberK[i] + waveNumberK[j] + waveNumberK[k] + waveNumberK[l]);
}

double Plot4::CouplingCCC(int i, int j, int k, int l)
{
    return CouplingC(i, j, k, l) * std::pow(waveNumberK[i] + waveNumberK[j] + waveNumberK[k] + waveNumberK[l], 2);
}

std::array<double, 4> Plot4::PhaseArg(double x, double t)
{
    std::array<double, 4> phaseArg {};

    for (long unsigned i = 0; i < waveNumberK.size(); ++i)
    {
        phaseArg[i] = waveNumberK[i] * (x - waveNumberK[i] * waveNumberK[i] * t - phaseShift[i]);
    }

    return phaseArg;
}

double Plot4::PhaseArg(double x, double t, double waveNumberK_, double phaseShift_)
{
    return waveNumberK_ * (x - waveNumberK_ * waveNumberK_ * t - phaseShift_);
}

double Plot4::Numerator1(double x, double t)
{
    double numerator {0.0};

    for (long unsigned i = 0; i < waveNumberK.size(); i++)
    {
        numerator += waveNumberK[i] * waveNumberK[i] * std::exp(PhaseArg(x, t)[i]);
    }

    numerator += CouplingAAA(0, 1) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1]);
    numerator += CouplingAAA(0, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2]);
    numerator += CouplingAAA(0, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[3]);
    numerator += CouplingAAA(1, 2) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    numerator += CouplingAAA(1, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
    numerator += CouplingAAA(2, 3) * std::exp(PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);

    numerator += CouplingBBB(0, 1, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    numerator += CouplingBBB(0, 1, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
    numerator += CouplingBBB(0, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    numerator += CouplingBBB(1, 2, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    
    numerator += CouplingCCC(0, 1, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);

    return numerator;
}

double Plot4::Numerator2(double x, double t)
{
    double numerator {0.0};

    for (long unsigned i = 0; i < waveNumberK.size(); i++)
    {
        numerator += waveNumberK[i] * std::exp(PhaseArg(x, t)[i]);
    }

    numerator += CouplingAA(0, 1) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1]);
    numerator += CouplingAA(0, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2]);
    numerator += CouplingAA(0, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[3]);
    numerator += CouplingAA(1, 2) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    numerator += CouplingAA(1, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
    numerator += CouplingAA(2, 3) * std::exp(PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);

    numerator += CouplingBB(0, 1, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    numerator += CouplingBB(0, 1, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
    numerator += CouplingBB(0, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    numerator += CouplingBB(1, 2, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    
    numerator += CouplingCC(0, 1, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);

    return numerator;
}

double Plot4::Denominator(double x, double t)
{
    double denominator {1.0};

    for (long unsigned i = 0; i < waveNumberK.size(); i++)
    {
        denominator += std::exp(PhaseArg(x, t)[i]);
    }

    denominator += CouplingA(0, 1) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1]);
    denominator += CouplingA(0, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2]);
    denominator += CouplingA(0, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[3]);
    denominator += CouplingA(1, 2) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    denominator += CouplingA(1, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
    denominator += CouplingA(2, 3) * std::exp(PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);

    denominator += CouplingB(0, 1, 2) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2]);
    denominator += CouplingB(0, 1, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[3]);
    denominator += CouplingB(0, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    denominator += CouplingB(1, 2, 3) * std::exp(PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);
    
    denominator += CouplingC(0, 1, 2, 3) * std::exp(PhaseArg(x, t)[0] + PhaseArg(x, t)[1] + PhaseArg(x, t)[2] + PhaseArg(x, t)[3]);

    return denominator;
}

double Plot4::Soliton(double x, double t)
{
    return 2 * Numerator1(x, t) / Denominator(x, t)
        - 2 * std::pow(Numerator2(x, t) / Denominator(x, t), 2)
        + integrationConst;
}

double Plot4::SolitonGhost1(double x, double t, double waveNumberK_, double phaseShift_)
{
    double expArg = std::exp(PhaseArg(x, t, waveNumberK_, phaseShift_));
    return 2 * waveNumberK_ * waveNumberK_ * expArg / std::pow(1 + expArg, 2);
}

double Plot4::SolitonGhost2(double x, double t, double waveNumberK_, double phaseShift_)
{
    double sechArg = 1.0 / std::cosh(0.5 * PhaseArg(x, t, waveNumberK_, phaseShift_));
    return 0.5 * waveNumberK_ * waveNumberK_ * sechArg * sechArg;
}

} // ~namespace Code::ImGuiImPlot
