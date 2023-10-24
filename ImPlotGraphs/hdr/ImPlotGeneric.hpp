#pragma once

namespace Code::ImGuiImPlot
{

class PlotGeneric
{
public:
    PlotGeneric() = default;
    virtual ~PlotGeneric() = default;

    double GetXIncrement() { return xIncrement; }
    int GetMaxPlotPointsPerUnitLength() { return maxPlotPointsPerUnitLength; }
    int* SetPlotPointsPerUnitLength() { return &plotPointsPerUnitLength; }
    bool* SetIsZoomViaPtr() { return &isZoom; }

protected:
    virtual void CalculatePlotCoordinates() = 0;
    virtual bool CheckIsNeedReplot() = 0;
    virtual void Graph() = 0;

protected:
    double xIncrement {0.0F};    // should be set in derived class
    const int maxPlotPointsPerUnitLength {100};
    int plotPointsPerUnitLength {9};
    int plotPoints {0};         // It is set in derived classes, includes both endpoints xMin and xMax
    const double gap {0.1};
    bool isZoom {false};
};

} // ~namespace Code::ImGuiImPlot
