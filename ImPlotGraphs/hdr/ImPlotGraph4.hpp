#pragma once
#include <vector>
#include <string>
#include "ImPlotGeneric.hpp"

namespace Code::ImGuiImPlot
{
    
class Plot4 : public PlotGeneric
{
public:
    Plot4();
    void Graph() override;

    float* SetTimeViaPtr() { return &time; }
    float GetTime() const { return time; }
    float GetTimeIncrement() { return timeIncrement; }
    float* SetWaveNumberViaPtr() { return waveNumber.data(); }
    std::pair<float, float> GetWaveNumberMinMax() const { return std::make_pair(0.4f, 2.8f); }
    float* SetPhaseShiftViaPtr() { return phaseShift.data(); }
    std::pair<float, float> GetPhaseShiftMinMax() const { return std::make_pair(xMin + 5.0f, xMax - 5.0f); }
    float* SetGhostEmpiricalPositionCorrectionViaPtr() { return ghostEmpiricalPositionCorrection.data(); }
    float* SetIntegrationConstViaPtr() { return &integrationConst; }

    int* SetSelectedSolitonComboItemViaPtr() { return &selectedSolitonComboItem; }
    const char * const * GetSolitonStartOfComboList() { return solitonComboDescription.data(); }
    int GetSolitonComboDescriptionSize() { return solitonComboDescription.size(); }

    unsigned long GetSolitonCount() const;
    std::vector<float> SolitonVelocity();
    std::string SolitonVelocityStr();
    float SolitonMaxTravelTime();

    bool GetIsAnimationPaused() const { return isAnimationPaused; }
    void SetIsAnimationPaused(bool isAnimationPaused) { this->isAnimationPaused = isAnimationPaused; }
    void RewindOneFrame() { time -= timeIncrement; }
    void ForwardOneFrame() { time += timeIncrement; }
    void SpeedUpAnimation() { if (timeIncrement < 0.4) timeIncrement += 0.01; }
    void SlowDownAnimation() { if (timeIncrement > 0.01) timeIncrement -= 0.01; }

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;

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
    double MultiSoliton(double x, double t);
    double SingleSolitonGhost(double x, double t, double waveNumber_, double phaseShift_);
    double GhostSuperposition(double x, double t);

    void AdvanceOrRepeatAnimation();

private:
    std::vector<double> x;
    std::vector<double> y1; // multi-soliton solution
    std::vector<double> y2; // one-soliton solution: non-interacting ghost of peak one
    std::vector<double> y3; // one-soliton solution: non-interacting ghost of peak two
    std::vector<double> y4; // one-soliton solution: non-interacting ghost of peak three
    std::vector<double> y5; // one-soliton solution: non-interacting ghost of peak four
    std::vector<double> y6; // linear supperposition of ghosts (to show contrasting behaviour of linear waves)
    
    std::vector<float> waveNumber {2.2f, 2.1f, 1.9f, 1.8f};
    std::vector<float> phaseShift {-20.0f, -15.0f, 2.0f, 10.0f};
    float integrationConst = {-2.0f/3.0f};

    // TRIAL AND ERROR CORRECTIONS TO INITIAL PEAK LOCATION OF GHOST SOLITONS:
    std::vector<float> ghostEmpiricalPositionCorrection {0.0f, 3.6f, 5.9f, 9.425f};

    float time {0.0f};
    float timeIncrement {0.1};
    bool isAnimationPaused {true};
    double xMin {-30.0};
    double xMax {-xMin};
    double yMin {integrationConst};
    double yMax {MultiSoliton(phaseShift[0], 0)};
    int selectedSolitonComboItem {4};
    std::vector<const char*> solitonComboDescription {
        "two solitons",
        "two solitons: merge-split collision",
        "two solitons: bounce-exchange collision",
        "three solitons",
        "four solitons"
    };
};

} // ~namespace Code::ImGuiImPlot
