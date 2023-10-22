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

    // double* GetSetTimeViaPtr() { return &time; }
    float* GetSetTimeViaPtr() { return &fTime; }
    float GetTime() const { return fTime; }
    float* GetSetWaveNumberViaPtr() { return waveNumber.data(); }
    float* GetSetPhaseShiftViaPtr() { return phaseShift.data(); }
    float* GetSetGhostPositionCorrectionViaPtr() { return ghostPositionCorrection.data(); }

    int* GetSetSelectedSolitonComboItemViaPtr() { return &selectedSolitonComboItem; }
    const char * const * GetSolitonStartOfComboList() { return solitonComboDescription.data(); }
    int GetSolitonComboDescriptionSize() { return solitonComboDescription.size(); }

    unsigned long GetSolitonCount() const { return selectedSolitonComboItem + 2ul; }
    float SolitonApproxTravelTime();

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;

    auto GenerateXTicksAndXLabels();
    auto GenerateYTicksAndYLabels();

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
    // std::vector<double> xTicks;
    // std::vector<double> yTicks;
    // std::vector<const char*> xLabels;
    // std::vector<const char*> yLabels;
    
    std::vector<float> waveNumber {2, 1.7, 1.4, 1};
    std::vector<float> phaseShift {-35, -25, -15, -7};
    std::vector<float> ghostPositionCorrection {3.05f, 5.91f, 8.46f};
    // const double integrationConst = {-2.0/3.0};
    // const float integrationConst = {-2.0f/3.0f};
    const float integrationConst = {0.0f};

    float fTime {0.0f};
    double time {0.0};
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
