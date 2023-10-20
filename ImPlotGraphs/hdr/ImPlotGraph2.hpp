#pragma once
#include <vector>
#include "ImPlotGeneric.hpp"

namespace Code::ImGuiImPlot
{
    
class Plot2 : public PlotGeneric
{
public:
    Plot2();
    void Graph() override;

    float* GetSetAmplitudeViaPtr() { return &amplitude; }
    float* GetSetWidthViaPtr() { return &width; }

private:
    void CalculatePlotCoordinates() override;
    bool CheckIsNeedReplot() override;

private:
    std::vector<double> x1;
    std::vector<double> x2;
    std::vector<double> y1;
    std::vector<double> y2;
    std::vector<double> y3;
    std::vector<double> y4;

    double x1Min {-5.0};
    double x1Max {-x1Min};
    double x2Min {-20.0};
    double x2Max {-x2Min};
    double y1Min {0.0};
    double y1Max {0.0};
    double y2Min {0.0};
    double y2Max {0.0};
    double y3Min {0.0};
    double y3Max {0.0};
    double y4Min {0.0};
    double y4Max {0.0};

    int plotPoints1 {0};
    int plotPoints2 {0};
    double x1Increment {0.0};
    double& x2Increment = xIncrement;
    
    float amplitude {2.0F}; // float because its is adjusted in ImGui::SliderFloat
    float width {3.0F};     // float because its is adjusted in ImGui::SliderFloat
};

} // ~namespace Code::ImGuiImPlot
