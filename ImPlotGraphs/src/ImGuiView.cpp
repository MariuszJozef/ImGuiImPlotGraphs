#include <iostream>
#include "imgui.h"
#include "ImGuiView.hpp"
#include "ImGuiFonts.hpp"
#include "ImPlotFrame.hpp"
#include "ImPlotGraph1.hpp"
#include "ImPlotGraph2.hpp"
#include "ImPlotGraph3.hpp"
#include "ImPlotGraph4.hpp"
#include "ImPlotGraph5.hpp"

namespace Code::ImGuiImPlot
{

ImGuiView::ImGuiView(ImGuiFonts& imGuiFonts)
    : imGuiFonts(imGuiFonts)
{}

void ImGuiView::MaximiseView1()
{
    static const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    // ImGui::SetNextWindowSize(viewport->Size, ImGuiCond_Once);
    // ImGui::SetNextWindowSize(viewport->Size, ImGuiCond_Always);
    ImGui::SetNextWindowSize(viewport->Size, ImGuiCond_Appearing);
}

void ImGuiView::SetInitialLayoutView1()
{
    static const ImGuiViewport* viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Once);
    // ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Always);
    ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Appearing);
    // ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y), ImGuiCond_Once);
    // ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y), ImGuiCond_Appearing);
}

void ImGuiView::MenuBarForView1()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::MenuItem("Adjust Style and Fonts", nullptr, &isShowSettingsView);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ImGuiView::DrawView1(ImPlotFrame* imPlotFrame)
{
    // MaximiseView1();
    SetInitialLayoutView1();

    imGuiFonts.PushSelectedFont();

    static bool isShowView1 {false};
    ImGuiWindowFlags windowFlags {0};
    // windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_MenuBar;
    windowFlags |= ImGuiWindowFlags_NoMove;
    // windowFlags |= ImGuiWindowFlags_NoResize;
    // windowFlags |= ImGuiWindowFlags_NoCollapse;

    if (!ImGui::Begin("ImGui View 1", &isShowView1, windowFlags))
    {
        ImGui::End();
        ImGui::PopFont();
        return;
    }

    MenuBarForView1();

    if (ImGui::BeginTabBar("TabBar"))
    {
        if (ImGui::BeginTabItem("Adjustible Sinusoid"))
        {
            DrawSubView1ForPlot1(imPlotFrame);
            imPlotFrame->plot1.Graph();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fourier Transform Pairs"))
        {
            DrawSubView1ForPlot2(imPlotFrame);
            imPlotFrame->plot2.Graph();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Multi-Slit Diffraction and Interference"))
        {
            DrawSubView1ForPlot3(imPlotFrame);
            imPlotFrame->plot3.Graph();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("KdV Soliton Collisions"))
        {
            DrawSubView1ForPlot4(imPlotFrame);
            imPlotFrame->plot4.Graph();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("KdV Soliton Collision Animations"))
        {
            DrawSubView1ForPlot5(imPlotFrame);
            imPlotFrame->plot5.Graph();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
    HandleActionsOfView1();
    ImGui::PopFont();
}

void ImGuiView::DrawSubView1ForPlot1(ImPlotFrame* imPlotFrame)
{
    ImGui::SetNextItemWidth(350);
    ImGui::SliderInt("##label1: PLOT POINTS", imPlotFrame->plot1.SetPlotPointsPerUnitLength(), 1, imPlotFrame->plot1.GetMaxPlotPointsPerUnitLength(), "plot points per unit length: %d");
    
    std::stringstream formatText;
    formatText << "x-mesh size: " 
        << std::setprecision(2) << std::scientific << imPlotFrame->plot1.GetXIncrement()
        << "\t(Slide to increase/decrease or ctrl + click to type in a value)";

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", formatText.str().c_str());

    ImGui::SameLine();
    ImGui::Checkbox("plot points", imPlotFrame->plot1.SetIsMarkersViaPtr());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    ImGui::Checkbox("zoom", imPlotFrame->plot1.SetIsZoomViaPtr());

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(280);
    ImGui::Combo("##label2: X DOMAIN", imPlotFrame->plot1.SetSelectedXAxisDomainViaPtr(), imPlotFrame->plot1.GetXAxisDomainStartOfComboList(), imPlotFrame->plot1.GetXAxisDomainSize());

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(200);
    ImGui::SliderFloat("##label3: AMPLITUDE", imPlotFrame->plot1.SetAmplitudeViaPtr(), -imPlotFrame->plot1.GetAmplitudeMax(), imPlotFrame->plot1.GetAmplitudeMax(), "amplitude: A = %.1F");

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(225);
    ImGui::SliderFloat("##label4: WAVELENGTH", imPlotFrame->plot1.SetWavelengthViaPtr(), 0.0F, 5 * std::numbers::pi, "wavelength: B = %.2F");

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(160);
    ImGui::SliderFloat("##label5: X-SHIFT", imPlotFrame->plot1.SetXShiftViaPtr(), -5.0F, 5.0F, "x-shift: C = %.1F");

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(160);
    ImGui::SliderFloat("##label6: Y-SHIFT", imPlotFrame->plot1.SetYShiftViaPtr(), -5.0F, 5.0F, "y-shift: D = %.1F");
    
    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(150);
    ImGui::SliderInt("##label7: COLOUR", imPlotFrame->plot1.SetSelectedPlotColourViaPtr(), 0, imPlotFrame->plot1.GetPlotColourSize() - 1, imPlotFrame->plot1.GetPlotColourName().c_str());
}

void ImGuiView::DrawSubView1ForPlot2(ImPlotFrame* imPlotFrame)
{
    ImGui::SetNextItemWidth(350);
    ImGui::SliderInt("##label1: PLOT POINTS", imPlotFrame->plot2.SetPlotPointsPerUnitLength(), 1, imPlotFrame->plot2.GetMaxPlotPointsPerUnitLength(), "plot points per unit length: %d");
    
    std::stringstream formatText;
    formatText << "x-mesh size: " 
        << std::setprecision(2) << std::scientific << imPlotFrame->plot2.GetXIncrement()
        << "\t(Slide to increase/decrease or ctrl + click to type in a value)";

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", formatText.str().c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    ImGui::Checkbox("zoom", imPlotFrame->plot2.SetIsZoomViaPtr());

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(250);
    ImGui::SliderFloat("##label2: AMPLITUDE", imPlotFrame->plot2.SetAmplitudeViaPtr(), 0.0F, 5.0F, "amplitude: A = %.1F");

    ImGui::SameLine(0.0f, 20.0f);
    ImGui::SetNextItemWidth(250);
    ImGui::SliderFloat("##label3: WIDTH", imPlotFrame->plot2.SetWidthViaPtr(), 0.0F, 5.0F, "width: a = %.1F");
}

void ImGuiView::DrawSubView1ForPlot3(ImPlotFrame* imPlotFrame)
{
    ImGui::SetNextItemWidth(350);
    ImGui::SliderInt("##label1: PLOT POINTS", imPlotFrame->plot3.SetPlotPointsPerUnitLength(), 1, imPlotFrame->plot3.GetMaxPlotPointsPerUnitLength(), "plot points per unit length: %d");
    
    std::stringstream formatText;
    formatText << "x-mesh size: " 
        << std::setprecision(2) << std::scientific << imPlotFrame->plot3.GetXIncrement()
        << "\t(Slide to increase/decrease or ctrl + click to type in a value)";

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", formatText.str().c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    ImGui::Checkbox("zoom", imPlotFrame->plot3.SetIsZoomViaPtr());

        ImGui::SameLine(0.0f, 20.0f);
        ImGui::SetNextItemWidth(300);
        ImGui::SliderInt("##label2: LIGHT COLOUR AND WAVELENGTH", imPlotFrame->plot3.SetSelectedLightViaPtr(), 0, imPlotFrame->plot3.GetLighColourAndWavelengthSize() - 1, imPlotFrame->plot3.GetLighColourName().c_str());

        ImGui::SameLine(0.0f, 20.0f);
        ImGui::SetNextItemWidth(250);
        ImGui::SliderFloat("##label3: SLIT WIDTH", imPlotFrame->plot3.SetSlitWidthViaPtr(), 1.0E-2, 2.0E0, "slit width:  %.1E (mm)");
        
        ImGui::SameLine(0.0f, 20.0f);
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##label4: SLIT NUMBER", imPlotFrame->plot3.SetSlitNumberViaPtr(), 0, 9, "number of slits: %d");

        if (*imPlotFrame->plot3.SetSlitNumberViaPtr() > 1)
        {
            ImGui::SameLine(0.0f, 20.0f);
            ImGui::SetNextItemWidth(300);
            ImGui::SliderFloat("##label5: SLIT SEPARATION", imPlotFrame->plot3.SetSlitSeparationViaPtr(), 1.0E-2, 6.0E0, "slit separation: %.1E (mm)");
        }

        ImGui::SameLine(0.0f, 20.0f);
        ImGui::SetNextItemWidth(300);
        ImGui::SliderFloat("##label6: SCREEN DISTANCE", imPlotFrame->plot3.SetScreenPerpDistanceViaPtr(), 1.0E2, 2.0E3, "screen distance: %.1E (mm)");
}

void ImGuiView::DrawSubView1ForPlot4(ImPlotFrame* imPlotFrame)
{
    ImGui::SetNextItemWidth(300);
    ImGui::SliderInt("##label1: PLOT POINTS", imPlotFrame->plot4.SetPlotPointsPerUnitLength(), 1, imPlotFrame->plot4.GetMaxPlotPointsPerUnitLength(), "plot points per unit length: %d");
    
    std::stringstream formatText;
    formatText << "x-mesh size: " 
        << std::setprecision(2) << std::scientific << imPlotFrame->plot4.GetXIncrement()
        << "\t(Slide to increase/decrease or ctrl + click to type in a value)";

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", formatText.str().c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    ImGui::Checkbox("zoom", imPlotFrame->plot4.SetIsZoomViaPtr());

    {
        static int i = 0;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(150);
            ImGui::SliderFloat("##label2: TIME", 
                imPlotFrame->plot4.SetTimeViaPtr(), 
                0.0F, 
                imPlotFrame->plot4.SolitonMaxTravelTime(), 
                "time = %.1f");

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("time (dimensionless)\n\nmax time: %.1f\t(proportional to distance to edge of slowest peak)", imPlotFrame->plot4.SolitonMaxTravelTime());

            ImGui::PopStyleColor(4);
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(170);
    ImGui::Combo("##label3: SOLITON COMBO", imPlotFrame->plot4.SetSelectedSolitonComboItemViaPtr(), imPlotFrame->plot4.GetSolitonStartOfComboList(), imPlotFrame->plot4.GetSolitonComboDescriptionSize());

    {
        static int i = 1;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(300);

            if (imPlotFrame->plot4.GetSolitonCount() == 2)
            {
                ImGui::SliderFloat2("##label4: WAVE NUMBER", 
                    imPlotFrame->plot4.SetWaveNumberViaPtr(), 
                    imPlotFrame->plot4.GetWaveNumberMinMax().first,
                    imPlotFrame->plot4.GetWaveNumberMinMax().second,
                    "%.2f");

                std::stringstream formatText;
                formatText << "wave number components: 0, 1\n\n"
                    << "solitonVelocity = waveNumber^2 + 6 * integrationConstant"
                    << std::setprecision(2) << std::fixed 
                    << "\n\ntherefore soliton velocities: " << imPlotFrame->plot4.SolitonVelocityStr()
                    << std::setprecision(1) << std::fixed 
                    << "\nand therefore  max time: " << imPlotFrame->plot4.SolitonMaxTravelTime();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }
            else if (imPlotFrame->plot4.GetSolitonCount() == 3)
            {
                ImGui::SliderFloat3("##label4: WAVE NUMBER", 
                    imPlotFrame->plot4.SetWaveNumberViaPtr(), 
                    imPlotFrame->plot4.GetWaveNumberMinMax().first,
                    imPlotFrame->plot4.GetWaveNumberMinMax().second,
                    "%.2f");

                std::stringstream formatText;
                formatText << "wave number components: 0, 1, 2\n\n"
                    << "solitonVelocity = waveNumber^2 + 6 * integrationConstant"
                    << std::setprecision(2) << std::fixed 
                    << "\n\ntherefore soliton velocities: " << imPlotFrame->plot4.SolitonVelocityStr()
                    << std::setprecision(1) << std::fixed 
                    << "\nand therefore  max time: " << imPlotFrame->plot4.SolitonMaxTravelTime();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }
            else if (imPlotFrame->plot4.GetSolitonCount() == 4)
            {
                ImGui::SliderFloat4("##label4: WAVE NUMBER", 
                    imPlotFrame->plot4.SetWaveNumberViaPtr(), 
                    imPlotFrame->plot4.GetWaveNumberMinMax().first,
                    imPlotFrame->plot4.GetWaveNumberMinMax().second,
                    "%.2f");

                std::stringstream formatText;
                formatText << "wave number components: 0, 1, 2, 3\n\n"
                    << "solitonVelocity = waveNumber^2 + 6 * integrationConstant"
                    << std::setprecision(2) << std::fixed 
                    << "\n\ntherefore soliton velocities: " << imPlotFrame->plot4.SolitonVelocityStr()
                    << std::setprecision(1) << std::fixed 
                    << "\nand therefore  max time: " << imPlotFrame->plot4.SolitonMaxTravelTime();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }

        ImGui::PopStyleColor(4);
    }

    {
        static int i = 2;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(300);

            if (imPlotFrame->plot4.GetSolitonCount() == 2)
            {
                ImGui::SliderFloat2("##label5: PHASE SHIFT", 
                    imPlotFrame->plot4.SetPhaseShiftViaPtr(), 
                    imPlotFrame->plot4.GetPhaseShiftMinMax().first, 
                    imPlotFrame->plot4.GetPhaseShiftMinMax().second, 
                    "%.1f");

                std::stringstream formatText1;
                formatText1 << std::setprecision(1) << std::fixed 
                    << "phase shift components: 0, 1\t(imply positions of peaks)\n\n"
                    << "therefore max time: "
                    << imPlotFrame->plot4.SolitonMaxTravelTime()
                    << "\t(proportional to distance to edge of slowest peak)";

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText1.str().c_str());
            }
            else if (imPlotFrame->plot4.GetSolitonCount() == 3)
            {
                ImGui::SliderFloat3("##label5: PHASE SHIFT", 
                    imPlotFrame->plot4.SetPhaseShiftViaPtr(), 
                    imPlotFrame->plot4.GetPhaseShiftMinMax().first, 
                    imPlotFrame->plot4.GetPhaseShiftMinMax().second, 
                    "%.1f");

                std::stringstream formatText2;
                formatText2 << std::setprecision(1) << std::fixed 
                    << "phase shift components: 0, 1, 2\t(imply positions of peaks)\n\n"
                    << "therefore max time: "
                    << imPlotFrame->plot4.SolitonMaxTravelTime()
                    << "\t(proportional to distance to edge of slowest peak)";

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText2.str().c_str());
            }
            else if (imPlotFrame->plot4.GetSolitonCount() == 4)
            {
                ImGui::SliderFloat4("##label5: PHASE SHIFT", 
                    imPlotFrame->plot4.SetPhaseShiftViaPtr(), 
                    imPlotFrame->plot4.GetPhaseShiftMinMax().first, 
                    imPlotFrame->plot4.GetPhaseShiftMinMax().second, 
                    "%.1f");

                std::stringstream formatText;
                formatText << std::setprecision(1) << std::fixed 
                    << "phase shift components: 0, 1, 2, 3\t(imply positions of peaks)\n\n"
                    << "therefore max time: "
                    << imPlotFrame->plot4.SolitonMaxTravelTime()
                    << "\t(proportional to distance to edge of slowest peak)";

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }

        ImGui::PopStyleColor(4);
    }

    {
        static int i = 6;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(250);

            if (imPlotFrame->plot4.GetTime() > 0.0f)
            {
                if (imPlotFrame->plot4.GetSolitonCount() == 2)
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::BeginDisabled();
                            ImGui::SliderFloat2("##label6: GHOST POSITION", imPlotFrame->plot4.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot4.GetSolitonCount() == 3)
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::BeginDisabled();
                            ImGui::SliderFloat3("##label6: GHOST POSITION", imPlotFrame->plot4.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot4.GetSolitonCount() == 4)
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::BeginDisabled();
                            ImGui::SliderFloat4("##label6: GHOST POSITION", imPlotFrame->plot4.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2, 3\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
            }
            else
            {
                if (imPlotFrame->plot4.GetSolitonCount() == 2)
                {
                    ImGui::SliderFloat2("##label6: GHOST POSITION", imPlotFrame->plot4.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot4.GetSolitonCount() == 3)
                {
                    ImGui::SliderFloat3("##label6: GHOST POSITION", imPlotFrame->plot4.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot4.GetSolitonCount() == 4)
                {
                    ImGui::SliderFloat4("##label6: GHOST POSITION", imPlotFrame->plot4.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2, 3\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
            }

        ImGui::PopStyleColor(4);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(240);
            ImGui::SliderFloat("##label7: INTEGRATION CONSTANT", imPlotFrame->plot4.SetIntegrationConstViaPtr(), -1.5f, 0.5f, "integration const. = %.2f");

            std::stringstream formatText;
            formatText << "solitonVelocity = waveNumber^2 + 6 * integrationConstant\n\n"
                << std::setprecision(2) << std::fixed 
                << "therefore soliton velocities: " << imPlotFrame->plot4.SolitonVelocityStr()
                << std::setprecision(1) << std::fixed 
                << "\nand therefore max time: " << imPlotFrame->plot4.SolitonMaxTravelTime();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("%s", formatText.str().c_str());

        ImGui::PopStyleColor(4);
    }
}

void ImGuiView::DrawSubView1ForPlot5(ImPlotFrame* imPlotFrame)
{
    ImGui::SetNextItemWidth(300);
    ImGui::SliderInt("##label1: PLOT POINTS", imPlotFrame->plot5.SetPlotPointsPerUnitLength(), 1, imPlotFrame->plot5.GetMaxPlotPointsPerUnitLength(), "plot points per unit length: %d");
    
    std::stringstream formatText;
    formatText << "x-mesh size: " 
        << std::setprecision(2) << std::scientific << imPlotFrame->plot5.GetXIncrement()
        << "\t(Slide to increase/decrease or ctrl + click to type in a value)";

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
        ImGui::SetTooltip("%s", formatText.str().c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    ImGui::Checkbox("zoom", imPlotFrame->plot5.SetIsZoomViaPtr());

/*
            ImGui::SameLine();
            ImGui::SetNextItemWidth(150);
            ImGui::SliderFloat("##label2: TIME", 
                imPlotFrame->plot5.SetTimeViaPtr(), 
                0.0F, 
                imPlotFrame->plot5.SolitonMaxTravelTime(), 
                "time = %.1f");

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("time (dimensionless)\n\nmax time: %.1f\t(proportional to distance to edge of slowest peak)", imPlotFrame->plot5.SolitonMaxTravelTime());
*/

    ImGui::SameLine();
    static bool isAnimationPaused {true};
    if (isAnimationPaused)
    {
        static int i = 0;
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));

        if (ImGui::Button("start"))
        {
            isAnimationPaused = !isAnimationPaused;
            imPlotFrame->plot5.SetIsAnimationPaused(isAnimationPaused);
            // std::cout << "START\n";
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("time (dimensionless)\n\nmax time: %.1f\t(proportional to distance to edge of slowest peak)", imPlotFrame->plot5.SolitonMaxTravelTime());

        std::stringstream formatText;
        formatText << "time = "
            << std::setprecision(2) << std::fixed 
            << imPlotFrame->plot5.GetTime()
            << " (dimensionless), max time: "
            << std::setprecision(1) << std::fixed 
            << imPlotFrame->plot5.SolitonMaxTravelTime();

        // ImGui::SameLine();
        ImGui::SameLine(0.0f, 20.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", formatText.str().c_str());

        ImGui::SameLine();
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##label3: REWIND", ImGuiDir_Left)) 
        {
            imPlotFrame->plot5.RewindOneFrame();
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("Rewind 1 frame (click-hold to repeatedly rewind)");

        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##label4: FAST FORWARD", ImGuiDir_Right))
        {
            imPlotFrame->plot5.ForwardOneFrame();
        }

        ImGui::PopButtonRepeat();
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("Forward 1 frame (click-hold to repeatedly forward)");

        ImGui::PopStyleColor(3);
    }
    else 
    {
        static int i = 0;
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));

        if (ImGui::Button("stop"))
        {
            isAnimationPaused = !isAnimationPaused;
            imPlotFrame->plot5.SetIsAnimationPaused(isAnimationPaused);
            // std::cout << "STOP\n";
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("time (dimensionless)\n\nmax time: %.1f\t(proportional to distance to edge of slowest peak)", imPlotFrame->plot5.SolitonMaxTravelTime());

        std::stringstream formatText;
        formatText << "time = "
            << std::setprecision(2) << std::fixed 
            << imPlotFrame->plot5.GetTime()
            << " (dimensionless), max time: "
            << std::setprecision(1) << std::fixed 
            << imPlotFrame->plot5.SolitonMaxTravelTime();

        // ImGui::SameLine();
        ImGui::SameLine(0.0f, 20.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", formatText.str().c_str());

        ImGui::SameLine();
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("##label5: SLOW DOWN", ImGuiDir_Down))
        {
            imPlotFrame->plot5.SlowDownAnimation();
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("Slow down overall animation speedometer = %.2f (arbitrary units; click-hold to repeatedly slow down)", imPlotFrame->plot5.GetTimeIncrement());

        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##label6: SPEED UP", ImGuiDir_Up)) 
        {
            imPlotFrame->plot5.SpeedUpAnimation();
        }

        ImGui::PopButtonRepeat();
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("Speed up overall animation speedometer = %.2f (arbitrary units; click-hold to repeatedly speed up)", imPlotFrame->plot5.GetTimeIncrement());

        ImGui::PopStyleColor(3);
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(170);
    ImGui::Combo("##label3: SOLITON COMBO", imPlotFrame->plot5.SetSelectedSolitonComboItemViaPtr(), imPlotFrame->plot5.GetSolitonStartOfComboList(), imPlotFrame->plot5.GetSolitonComboDescriptionSize());

    {
        static int i = 1;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(300);

            if (imPlotFrame->plot5.GetSolitonCount() == 2)
            {
                ImGui::SliderFloat2("##label4: WAVE NUMBER", 
                    imPlotFrame->plot5.SetWaveNumberViaPtr(), 
                    imPlotFrame->plot5.GetWaveNumberMinMax().first,
                    imPlotFrame->plot5.GetWaveNumberMinMax().second,
                    "%.2f");

                std::stringstream formatText;
                formatText << "wave number components: 0, 1\n\n"
                    << "solitonVelocity = waveNumber^2 + 6 * integrationConstant"
                    << std::setprecision(2) << std::fixed 
                    << "\n\ntherefore soliton velocities: " << imPlotFrame->plot5.SolitonVelocityStr()
                    << std::setprecision(1) << std::fixed 
                    << "\nand therefore  max time: " << imPlotFrame->plot5.SolitonMaxTravelTime();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }
            else if (imPlotFrame->plot5.GetSolitonCount() == 3)
            {
                ImGui::SliderFloat3("##label4: WAVE NUMBER", 
                    imPlotFrame->plot5.SetWaveNumberViaPtr(), 
                    imPlotFrame->plot5.GetWaveNumberMinMax().first,
                    imPlotFrame->plot5.GetWaveNumberMinMax().second,
                    "%.2f");

                std::stringstream formatText;
                formatText << "wave number components: 0, 1, 2\n\n"
                    << "solitonVelocity = waveNumber^2 + 6 * integrationConstant"
                    << std::setprecision(2) << std::fixed 
                    << "\n\ntherefore soliton velocities: " << imPlotFrame->plot5.SolitonVelocityStr()
                    << std::setprecision(1) << std::fixed 
                    << "\nand therefore  max time: " << imPlotFrame->plot5.SolitonMaxTravelTime();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }
            else if (imPlotFrame->plot5.GetSolitonCount() == 4)
            {
                ImGui::SliderFloat4("##label4: WAVE NUMBER", 
                    imPlotFrame->plot5.SetWaveNumberViaPtr(), 
                    imPlotFrame->plot5.GetWaveNumberMinMax().first,
                    imPlotFrame->plot5.GetWaveNumberMinMax().second,
                    "%.2f");

                std::stringstream formatText;
                formatText << "wave number components: 0, 1, 2, 3\n\n"
                    << "solitonVelocity = waveNumber^2 + 6 * integrationConstant"
                    << std::setprecision(2) << std::fixed 
                    << "\n\ntherefore soliton velocities: " << imPlotFrame->plot5.SolitonVelocityStr()
                    << std::setprecision(1) << std::fixed 
                    << "\nand therefore  max time: " << imPlotFrame->plot5.SolitonMaxTravelTime();

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }

        ImGui::PopStyleColor(4);
    }

    {
        static int i = 2;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(300);

            if (imPlotFrame->plot5.GetSolitonCount() == 2)
            {
                ImGui::SliderFloat2("##label5: PHASE SHIFT", 
                    imPlotFrame->plot5.SetPhaseShiftViaPtr(), 
                    imPlotFrame->plot5.GetPhaseShiftMinMax().first, 
                    imPlotFrame->plot5.GetPhaseShiftMinMax().second, 
                    "%.1f");

                std::stringstream formatText1;
                formatText1 << std::setprecision(1) << std::fixed 
                    << "phase shift components: 0, 1\t(imply positions of peaks)\n\n"
                    << "therefore max time: "
                    << imPlotFrame->plot5.SolitonMaxTravelTime()
                    << "\t(proportional to distance to edge of slowest peak)";

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText1.str().c_str());
            }
            else if (imPlotFrame->plot5.GetSolitonCount() == 3)
            {
                ImGui::SliderFloat3("##label5: PHASE SHIFT", 
                    imPlotFrame->plot5.SetPhaseShiftViaPtr(), 
                    imPlotFrame->plot5.GetPhaseShiftMinMax().first, 
                    imPlotFrame->plot5.GetPhaseShiftMinMax().second, 
                    "%.1f");

                std::stringstream formatText2;
                formatText2 << std::setprecision(1) << std::fixed 
                    << "phase shift components: 0, 1, 2\t(imply positions of peaks)\n\n"
                    << "therefore max time: "
                    << imPlotFrame->plot5.SolitonMaxTravelTime()
                    << "\t(proportional to distance to edge of slowest peak)";

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText2.str().c_str());
            }
            else if (imPlotFrame->plot5.GetSolitonCount() == 4)
            {
                ImGui::SliderFloat4("##label5: PHASE SHIFT", 
                    imPlotFrame->plot5.SetPhaseShiftViaPtr(), 
                    imPlotFrame->plot5.GetPhaseShiftMinMax().first, 
                    imPlotFrame->plot5.GetPhaseShiftMinMax().second, 
                    "%.1f");

                std::stringstream formatText;
                formatText << std::setprecision(1) << std::fixed 
                    << "phase shift components: 0, 1, 2, 3\t(imply positions of peaks)\n\n"
                    << "therefore max time: "
                    << imPlotFrame->plot5.SolitonMaxTravelTime()
                    << "\t(proportional to distance to edge of slowest peak)";

                if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                    ImGui::SetTooltip("%s", formatText.str().c_str());
            }

        ImGui::PopStyleColor(4);
    }

    {
        static int i = 6;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(250);

            if (imPlotFrame->plot5.GetTime() > 0.0f)
            {
                if (imPlotFrame->plot5.GetSolitonCount() == 2)
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::BeginDisabled();
                            ImGui::SliderFloat2("##label6: GHOST POSITION", imPlotFrame->plot5.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot5.GetSolitonCount() == 3)
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::BeginDisabled();
                            ImGui::SliderFloat3("##label6: GHOST POSITION", imPlotFrame->plot5.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot5.GetSolitonCount() == 4)
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::BeginDisabled();
                            ImGui::SliderFloat4("##label6: GHOST POSITION", imPlotFrame->plot5.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");
                        ImGui::EndDisabled();
                    }
                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2, 3\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
            }
            else
            {
                if (imPlotFrame->plot5.GetSolitonCount() == 2)
                {
                    ImGui::SliderFloat2("##label6: GHOST POSITION", imPlotFrame->plot5.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot5.GetSolitonCount() == 3)
                {
                    ImGui::SliderFloat3("##label6: GHOST POSITION", imPlotFrame->plot5.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
                else if (imPlotFrame->plot5.GetSolitonCount() == 4)
                {
                    ImGui::SliderFloat4("##label6: GHOST POSITION", imPlotFrame->plot5.SetGhostEmpiricalPositionCorrectionViaPtr(), 0.0f, 10.0f, "%.2f");

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Empirical correction of \"ghost\"s' initial position, components: 0, 1, 2, 3\n\nshift to align with multi-soliton solution\n\nonly adjustible at time = 0");
                }
            }

        ImGui::PopStyleColor(4);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.5f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.6f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.7f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(0.5f / 7.0f, 0.9f, 0.9f));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(240);
            ImGui::SliderFloat("##label7: INTEGRATION CONSTANT", imPlotFrame->plot5.SetIntegrationConstViaPtr(), -1.5f, 0.5f, "integration const. = %.2f");

            std::stringstream formatText;
            formatText << "solitonVelocity = waveNumber^2 + 6 * integrationConstant\n\n"
                << std::setprecision(2) << std::fixed 
                << "therefore soliton velocities: " << imPlotFrame->plot5.SolitonVelocityStr()
                << std::setprecision(1) << std::fixed 
                << "\nand therefore max time: " << imPlotFrame->plot5.SolitonMaxTravelTime();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                ImGui::SetTooltip("%s", formatText.str().c_str());

        ImGui::PopStyleColor(4);
    }
}

void ImGuiView::HandleActionsOfView1()
{
    if (isShowSettingsView)
        DrawSettingsView2();
}

void ImGuiView::DrawSettingsView2()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 550, viewport->WorkPos.y + 50), ImGuiCond_Always);
    // ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x / 2.1, viewport->WorkSize.y / 3.5), ImGuiCond_Appearing);
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 550, viewport->WorkPos.y + 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x / 2.1, viewport->WorkSize.y / 3.5), ImGuiCond_FirstUseEver);

    imGuiFonts.PushSelectedFont();

    if (!ImGui::Begin("Style and Fonts", &isShowSettingsView))
    {
        ImGui::End();
        ImGui::PopFont();
        return;
    }

    static int themeStyleIndex {0};
    ImGui::RadioButton("Dark", &themeStyleIndex, 0); ImGui::SameLine();
    ImGui::RadioButton("Light", &themeStyleIndex, 1); ImGui::SameLine();
    ImGui::RadioButton("Classic", &themeStyleIndex, 2);

    switch (themeStyleIndex)
    {
    case 0: ImGui::StyleColorsDark(); break;
    case 1: ImGui::StyleColorsLight(); break;
    case 2: ImGui::StyleColorsClassic(); break;
    }

    ImGui::SameLine();

    ImGui::Checkbox("ImGui Demo Window", &isShowImGuiDemo);
    ImGui::SameLine();
    ImGui::Checkbox("ImPlot Demo Window", &isShowImPlotDemo);

    ImGui::SetNextItemWidth(250);
    ImGui::Combo("Font  ", &imGuiFonts.selectedFontNameIndex, imGuiFonts.pFontNameItems.get(), imGuiFonts.availableFontNames.size());

    ImGui::SameLine();

    ImGui::SetNextItemWidth(100);
    ImGui::Combo("Font Size", &imGuiFonts.selectedFontSizeIndex, imGuiFonts.pFontSizeItems.get(), imGuiFonts.fontSizesStr.size());

    ImGui::End();
    HandleActionsOfSettingsView2();
    ImGui::PopFont();
}

void ImGuiView::HandleActionsOfSettingsView2()
{
    if (isShowImGuiDemo)
        ImGui::ShowDemoWindow(&isShowImGuiDemo);
            
    if (isShowImPlotDemo)
        ImPlot::ShowDemoWindow(&isShowImPlotDemo);
}
    
} // ~namespace Code::ImGuiImPlot
