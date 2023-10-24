#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include "implot.h"
#include "ImPlotGraph5.hpp"

namespace Code::ImGuiImPlot
{

Plot5::Plot5()
{
    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    x.reserve(plotPoints);
    y1.reserve(plotPoints);
    y2.reserve(plotPoints);
    y3.reserve(plotPoints);
    y4.reserve(plotPoints);
    y5.reserve(plotPoints);
    y6.reserve(plotPoints);
}

void Plot5::CalculatePlotCoordinates()
{
    x.clear();
    y1.clear();
    y2.clear();
    y3.clear();
    y4.clear();
    y5.clear();
    y6.clear();

    plotPoints = std::round(plotPointsPerUnitLength * (xMax - xMin));
    xIncrement = (xMax - xMin) / (plotPoints - 1);

    for (int i = 0; i < plotPoints; ++i) 
    {
        x.push_back(xMin + i * xIncrement);
        y1.push_back(MultiSoliton(x.at(i), time));

        y2.push_back(SingleSolitonGhost(x.at(i), time, waveNumber[0], 
            phaseShift[0] + ghostEmpiricalPositionCorrection.at(0))
        );

        y3.push_back(SingleSolitonGhost(x.at(i), time, waveNumber[1], 
            phaseShift[1] + ghostEmpiricalPositionCorrection.at(1))
        );

        if (GetSolitonCount() >= 3)
        {
            y4.push_back(SingleSolitonGhost(x.at(i), time, waveNumber[2], 
                phaseShift[2] + ghostEmpiricalPositionCorrection.at(2))
            );
        }

        if (GetSolitonCount() == 4)
        {
            y5.push_back(SingleSolitonGhost(x.at(i), time, waveNumber[3], 
                phaseShift[3] + ghostEmpiricalPositionCorrection.at(3))
            );
        }

        y6.push_back(GhostSuperposition(x.at(i), time));
    }
}

bool Plot5::CheckIsNeedReplot()
{
    static bool isNeedReplot {true};
    static int plotPointsPerUnitLengthOld {plotPointsPerUnitLength};
    static float timeOld {time};
    static std::vector<float> waveNumberOld {waveNumber};
    static std::vector<float> phaseShiftOld {phaseShift};
    static std::vector<float> ghostEmpiricalPositionCorrectionOld {ghostEmpiricalPositionCorrection};
    static int selectedSolitonComboItemOld {selectedSolitonComboItem};
    static float integrationConstOld {integrationConst};

    if (plotPointsPerUnitLengthOld != plotPointsPerUnitLength)
    {
        plotPointsPerUnitLengthOld = plotPointsPerUnitLength;
        isNeedReplot = true;
    }
    else if (timeOld != time)
    {
        timeOld = time;
        isNeedReplot = true;
    }
    else if (waveNumberOld != waveNumber)
    {
        waveNumberOld = waveNumber;
        isNeedReplot = true;
        auto max = std::max_element(begin(y1), end(y1));
        yMax = std::ceil(*max);
    }
    else if (phaseShiftOld != phaseShift)
    {
        phaseShiftOld = phaseShift;
        isNeedReplot = true;
    }
    else if (ghostEmpiricalPositionCorrectionOld != ghostEmpiricalPositionCorrection)
    {
        ghostEmpiricalPositionCorrectionOld = ghostEmpiricalPositionCorrection;
        isNeedReplot = true;
    }
    else if (selectedSolitonComboItemOld != selectedSolitonComboItem)
    {
        selectedSolitonComboItemOld = selectedSolitonComboItem;
        isNeedReplot = true;
    }
    else if (integrationConstOld != integrationConst)
    {
        integrationConstOld = integrationConst;
        isNeedReplot = true;
        yMin = integrationConst;
        auto max = std::max_element(begin(y1), end(y1));
        yMax = std::ceil(*max);
    }

    if (isNeedReplot)
    {
        isNeedReplot = false; // So that next time we don't replot unless parameter(s) change
        return true;
    }

    return isNeedReplot;
}

std::pair<std::vector<double>, std::vector<const char*>> Plot5::GenerateXTicksAndXLabels()
{
    std::pair<std::vector<double>, std::vector<const char*>> ret {};

    auto& xTicks = ret.first;
    auto& xLabels = ret.second;

    xTicks = std::vector<double> {-40, -30, -20, -10, 0, 10, 20, 30, 40};
    xLabels = std::vector<const char*> {"-40", "-30", "-20", "-10", "0", "10", "20", "30", "40"};

    if (xTicks.size() != xLabels.size())
    {
        std::stringstream message;
        message << "Error: GenerateXTicksAndXLabels: Size mismatch! xTicks.size(): " << xTicks.size() 
            << ", xLabels.size(): " << xLabels.size();

        throw std::runtime_error(message.str());
    }

    return ret;
}

std::pair<std::vector<double>, std::vector<const char*>> Plot5::GenerateYTicksAndYLabels()
{
    std::pair<std::vector<double>, std::vector<const char*>> ret {};

    auto& yTicks = ret.first;
    auto& yLabels = ret.second;

    yTicks = std::vector<double> {-2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4};
    yLabels = std::vector<const char*> {"-2", "-1,5", "-1", "-0.5", "0", "0.5", "1", "1.5", "2", "2.5", "3", "3.5", "4"};

    if (yTicks.size() != yLabels.size())
    {
        std::stringstream message;
        message << "Error: GenerateYTicksAndYLabels: Size mismatch! yTicks.size(): " << yTicks.size() 
            << ", yLabels.size(): " << yLabels.size();

        throw std::runtime_error(message.str());
    }

    return ret;
}

void Plot5::Graph()
{
    if (CheckIsNeedReplot())
    {
        CalculatePlotCoordinates();
    }

    if (ImPlot::BeginPlot("Collision of solitons: an (exact) N-soliton solution u(x, t) of nonlinear KdV equation: u_t + 6 u * u_x + u_{xxx} = 0", ImVec2(-1, -1), ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupAxis(ImAxis_X1, "x axis (dimensionless - no units)");
        ImPlot::SetupAxis(ImAxis_Y1, "y axis (dimensionless - no units)");

        double extraForLegend = yMax + 0.5;

        if (isZoom)
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, extraForLegend + gap, ImPlotCond_None);
        }
        else
        {
            ImPlot::SetupAxesLimits(xMin - gap, xMax + gap, yMin - gap, extraForLegend + gap, ImPlotCond_Always);
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

        // ImPlot::SetNextLineStyle(ImVec4(139.0/255, 69.0/255, 19.0/255, 1), 2.0F);
        // ImPlot::SetNextLineStyle(ImVec4(128.0/255, 128.0/255, 128.0/255, 1), 2.0F);
        // ImPlot::SetNextLineStyle(ImVec4(56.0/255, 83.0/255, 75.0/255, 1), 2.0F);
        ImPlot::SetNextLineStyle(ImVec4(25.0/255, 188.0/255, 55.0/255, 1), 2.0F);
        ImPlot::PlotLine("Fictitious linear superposition of \"ghost\" solitons", x.data(), y6.data(), plotPoints);

        ImPlot::EndPlot();
    }

    AdvanceOrRepeatAnimation();
}

void Plot5::AdvanceOrRepeatAnimation()
{
    if (!isAnimationPaused)
        ForwardOneFrame();

    if (time > SolitonMaxTravelTime())
    {
        time = 0.0f; // repeat animation
    }
}

double Plot5::CouplingA(int i, int j)
{
    return std::pow(waveNumber[i] - waveNumber[j], 2) / std::pow(waveNumber[i] + waveNumber[j], 2);
}

double Plot5::CouplingAA(int i, int j)
{
    return std::pow(waveNumber[i] - waveNumber[j], 2) / (waveNumber[i] + waveNumber[j]);
}

double Plot5::CouplingAAA(int i, int j)
{
    return std::pow(waveNumber[i] - waveNumber[j], 2);
}

double Plot5::CouplingB(int i, int j, int k)
{
    return CouplingA(i, j) * CouplingA(i, k) * CouplingA(j, k);
}

double Plot5::CouplingBB(int i, int j, int k)
{
    return CouplingB(i, j, k) * (waveNumber[i] + waveNumber[j] + waveNumber[k]);
}

double Plot5::CouplingBBB(int i, int j, int k)
{
    return CouplingB(i, j, k) * std::pow(waveNumber[i] + waveNumber[j] + waveNumber[k], 2);
}

double Plot5::CouplingC(int i, int j, int k, int l)
{
    return CouplingA(i, j) * CouplingA(i, k) * CouplingA(i, l)
        * CouplingA(j, k) * CouplingA(j, l) * CouplingA(k, l);
}

double Plot5::CouplingCC(int i, int j, int k, int l)
{
    return CouplingC(i, j, k, l) * (waveNumber[i] + waveNumber[j] + waveNumber[k] + waveNumber[l]);
}

double Plot5::CouplingCCC(int i, int j, int k, int l)
{
    return CouplingC(i, j, k, l) * std::pow(waveNumber[i] + waveNumber[j] + waveNumber[k] + waveNumber[l], 2);
}

std::vector<double> Plot5::PhaseArg(double x, double t)
{
    std::vector<double> phaseArg(GetSolitonCount());
    std::vector<float> velocity = SolitonVelocity();

    for (long unsigned i = 0; i < GetSolitonCount(); ++i)
    {
        phaseArg[i] = waveNumber[i] * (x - velocity.at(i) * t - phaseShift[i]);
    }

    return phaseArg;
}

double Plot5::PhaseArg(double x, double t, double waveNumber_, double phaseShift_)
{
    double velocity = waveNumber_ * waveNumber_ + 6.0 * integrationConst;
    return waveNumber_ * (x - velocity * t - phaseShift_);
}

double Plot5::Numerator1(double x, double t)
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

double Plot5::Numerator2(double x, double t)
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

double Plot5::Denominator(double x, double t)
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

double Plot5::MultiSoliton(double x, double t)
{
    return 2 * Numerator1(x, t) / Denominator(x, t)
        - 2 * std::pow(Numerator2(x, t) / Denominator(x, t), 2)
        + integrationConst;
}

double Plot5::SingleSolitonGhost(double x, double t, double waveNumber_, double phaseShift_)
{
    double sechArg = 1.0 / std::cosh(0.5 * PhaseArg(x, t, waveNumber_, phaseShift_));
    return 0.5 * waveNumber_ * waveNumber_ * sechArg * sechArg
        + integrationConst;
}

/*  EQUIVALENT EXPRESSION:
double Plot5::SingleSolitonGhost(double x, double t, double waveNumber_, double phaseShift_)
{
    double expArg = std::exp(PhaseArg(x, t, waveNumber_, phaseShift_));
    return 2 * waveNumber_ * waveNumber_ * expArg / std::pow(1 + expArg, 2)
        + integrationConst;
}
*/

double Plot5::GhostSuperposition(double x, double t)
{
    double superposition {integrationConst}; // add integrationConst just once!

    for (int i = 0; i < GetSolitonCount(); i++)
    {
        superposition += SingleSolitonGhost(x, t, 
            waveNumber[i], 
            phaseShift[i] + ghostEmpiricalPositionCorrection[i]) 
            - integrationConst; // do not add the integrationConst multiple times!
    }

    return superposition;
}

std::vector<float> Plot5::SolitonVelocity()
{
    std::vector<float> velocity(GetSolitonCount());

    std::transform(waveNumber.begin(), waveNumber.end(), velocity.begin(),
        [integrationConst = this->integrationConst]
        (float w)
        { 
            return w * w + 6.0f * integrationConst; 
        }
    );

    return velocity;
}

std::string Plot5::SolitonVelocityStr()
{
    std::stringstream ss;

    for (bool isFirst = true; const auto& v : SolitonVelocity())
    {
        if (isFirst)
        {
            isFirst = false;
        }
        else
        {
            ss << ", ";
        }

        ss << std::setprecision(2) << std::fixed << std::round(v * 100.0) / 100.0;
    }

    ss << "\n";

    return ss.str();
}

float Plot5::SolitonMaxTravelTime()
{
    std::vector<float> solitonVelocity = SolitonVelocity();
    std::vector<float> solitonTravelTimes(GetSolitonCount());

    for (int i = 0; i < solitonVelocity.size(); i++)
    {
        float travelDistance {0.0f};

        if (std::abs(solitonVelocity.at(i)) <= 1.0E-6) // i.e. velocity is zero
        {
            continue;
        }
        if (std::abs(solitonVelocity.at(i)) < 0.5)
        {
            // exclude snail-pace velocity, it would make the travel time exceedingly long
            continue;
        }
        else if (solitonVelocity.at(i) > 0.0f)
        {
            travelDistance = static_cast<float>(xMax) - phaseShift.at(i);
        }
        else if (solitonVelocity.at(i) < 0.0f)
        {
            travelDistance = phaseShift.at(i) - static_cast<float>(xMin);
        }

        solitonTravelTimes.push_back((travelDistance) / std::abs(solitonVelocity.at(i)));
    }

    auto maxTravelTime = std::max_element(begin(solitonTravelTimes), end(solitonTravelTimes));

    return std::roundf(static_cast<float>(*maxTravelTime));
}

} // ~namespace Code::ImGuiImPlot
