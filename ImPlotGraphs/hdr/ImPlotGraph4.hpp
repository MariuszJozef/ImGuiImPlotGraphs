#pragma once
#include <vector>
#include "ImPlotGeneric.hpp"

namespace Code::ImGuiImPlot
{
    
class Plot4 : public PlotGeneric
{
public:
    Plot4();
    void Graph() override;

    float* GetSetTimeViaPtr() { return &time; }
    float GetTime() const { return time; }
    float* GetSetWaveNumberViaPtr() { return waveNumber.data(); }
    std::pair<float, float> GetWaveNumberMinMax() const { return std::make_pair(0.4f, 2.8f); }
    float* GetSetPhaseShiftViaPtr() { return phaseShift.data(); }
    std::pair<float, float> GetPhaseShiftMinMax() const { return std::make_pair(xMin + 5.0f, xMax - 5.0f); }
    float* GetSetGhostEmpiricalPositionCorrectionViaPtr() { return ghostEmpiricalPositionCorrection.data(); }
    float* GetSetIntegrationConstViaPtr() { return &integrationConst; }

    int* GetSetSelectedSolitonComboItemViaPtr() { return &selectedSolitonComboItem; }
    const char * const * GetSolitonStartOfComboList() { return solitonComboDescription.data(); }
    int GetSolitonComboDescriptionSize() { return solitonComboDescription.size(); }

    unsigned long GetSolitonCount() const { return selectedSolitonComboItem + 2ul; }
    std::vector<float> SolitonVelocity();
    std::string SolitonVelocityStr();
    float SolitonMaxTravelTime();

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;

    // auto GenerateXTicksAndXLabels();
    // auto GenerateYTicksAndYLabels();
    std::pair<std::vector<double>, std::vector<const char*>> GenerateXTicksAndXLabels();
    std::pair<std::vector<double>, std::vector<const char*>> GenerateYTicksAndYLabels();

    double CouplingA(int i, int j);
    double CouplingAA(int i, int j);
    double CouplingAAA(int i, int j);
    double CouplingB(int i, int j, int k);
    double CouplingBB(int i, int j, int k);
    double CouplingBBB(int i, int j, int k);
    double CouplingC(int i, int j, int k, int l);
    double CouplingCC(int i, int j, int k, int l);
    double CouplingCCC(int i, int j, int k, int l);
    std::vector<double> PhaseArg(double x, double t);
    double PhaseArg(double x, double t, double waveNumber_, double phaseShift_);
    double Numerator1(double x, double t);
    double Numerator2(double x, double t);
    double Denominator(double x, double t);
    double SolitonGhost1(double x, double t, double waveNumber_, double phaseShift_);
    double SolitonGhost2(double x, double t, double waveNumber_, double phaseShift_);
    double Soliton(double x, double t);

private:
    std::vector<double> x;
    std::vector<double> y1;
    std::vector<double> y2;
    std::vector<double> y3;
    std::vector<double> y4;
    std::vector<double> y5;
    
    std::vector<float> waveNumber {2.0f, 1.7f, 1.4f, 1.0f};
    std::vector<float> phaseShift {-35.0f, -25.0f, -15.0f, -7.0f};
    float integrationConst = {-2.0f/3.0f};
/*
    TRIAL AND ERROR CORRECTIONS TO INITIAL PEAK LOCATION OF GHOST SOLITONS:
    the zeroth ghost sits exactly at phaseShift[0] and is not represented in ghostEmpiricalPositionCorrection
*/
    std::vector<float> ghostEmpiricalPositionCorrection {3.05f, 5.91f, 8.46f};

    float time {0.0f};
    double xMin {-40.0};
    double xMax {-xMin};
    double yMin {integrationConst};
    double yMax {Soliton(phaseShift[0], 0)};
    int selectedSolitonComboItem {2};
    std::vector<const char*> solitonComboDescription {
        "two solitons",
        "three solitons",
        "four solitons"
    };

    // enum class SolitonInteraction
    // {
    //     twoSolitons,
    //     threeSolitons,
    //     fourSolitons
    // };
    
    // SolitonInteraction solitonInteraction {static_cast<SolitonInteraction>(selectedSolitonComboItem)};
};

} // ~namespace Code::ImGuiImPlot
