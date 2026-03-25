#include "InkRecoBridge.h"

#include <vector>
#include <string>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Input.Inking.h>
#include <winrt/Windows.UI.Input.Inking.Analysis.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>



using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Input::Inking;
using namespace Windows::UI::Input::Inking::Analysis;

namespace
{
    InkStrokeBuilder        g_builder{ nullptr };
    InkStrokeContainer      g_container{ nullptr };
    InkAnalyzer             g_analyzer{ nullptr };
    InkRecognizerContainer  g_recoContainer{ nullptr };

    // Now uses BridgePoint, not InkPoint
    std::vector<BridgePoint> g_currentStroke;

    std::wstring            g_textResult;
    std::wstring            g_shapeResult;
}

extern "C" {

    INK_API void InkReco_Init()
    {
        init_apartment(apartment_type::single_threaded);

        g_builder       = InkStrokeBuilder();
        g_container     = InkStrokeContainer();
        g_analyzer      = InkAnalyzer();
        g_recoContainer = InkRecognizerContainer();
    }

    INK_API void InkReco_Clear()
    {
        g_container     = InkStrokeContainer();
        g_analyzer      = InkAnalyzer();
        g_recoContainer = InkRecognizerContainer();
    }

    INK_API bool InkReco_SetLanguage(const wchar_t* bcp47)
    {
        if (!bcp47 || !g_recoContainer)
            return false;

        auto recos = g_recoContainer.GetRecognizers();
        for (auto const& r : recos)
        {
            if (r.Name() == bcp47)
            {
                g_recoContainer.SetDefaultRecognizer(r);
                return true;
            }
        }
        return false;
    }

    INK_API void InkReco_BeginStroke()
    {
        g_currentStroke.clear();
    }

    INK_API void InkReco_AddPoint(const BridgePoint& p)
    {
        g_currentStroke.push_back(p);
    }

    INK_API void InkReco_EndStroke()
    {
        if (g_currentStroke.size() < 2)
            return;

        std::vector<Windows::UI::Input::Inking::InkPoint> inkPts;
        inkPts.reserve(g_currentStroke.size());
        auto inkPtsWinRT = winrt::single_threaded_vector<InkPoint>();

        for (auto& bp : g_currentStroke)
        {
            inkPtsWinRT.Append(
                InkPoint(Point{ bp.x, bp.y }, 0.5f)
            );
        }

        for (auto& bp : g_currentStroke)
        {
            inkPts.emplace_back(
                Point{ bp.x, bp.y },
                0.5f // pressure
            );
        }

        //auto stroke = g_builder.CreateStroke(inkPts);
        
        auto stroke = g_builder.CreateStrokeFromInkPoints(
            inkPtsWinRT,
            Windows::Foundation::Numerics::float3x2{ 1,0,0,1,0,0 }
        );


        g_container.AddStroke(stroke);
    }

    INK_API const wchar_t* InkReco_RecognizeText()
    {
        g_textResult.clear();

        if (!g_analyzer || !g_container)
            return g_textResult.c_str();

        g_analyzer.ClearDataForAllStrokes();
        g_analyzer.AddDataForStrokes(g_container.GetStrokes());
        


        g_analyzer.AnalyzeAsync().get();

        auto root  = g_analyzer.AnalysisRoot();
        auto words = root.FindNodes(InkAnalysisNodeKind::InkWord);

        for (auto const& node : words)
        {
            auto word = node.as<InkAnalysisInkWord>();
            g_textResult += word.RecognizedText();
            g_textResult += L" ";
        }

        return g_textResult.c_str();
    }

    INK_API const wchar_t* InkReco_RecognizeShapes()
    {
        g_shapeResult.clear();

        if (!g_analyzer || !g_container)
            return g_shapeResult.c_str();

        g_analyzer.ClearDataForAllStrokes();
        g_analyzer.AddDataForStrokes(g_container.GetStrokes());

        g_analyzer.AnalyzeAsync().get();

        auto root     = g_analyzer.AnalysisRoot();
        auto drawings = root.FindNodes(InkAnalysisNodeKind::InkDrawing);

        for (auto const& node : drawings)
        {
            auto d    = node.as<InkAnalysisInkDrawing>();
            auto kind = d.DrawingKind();
            auto box  = d.BoundingRect();

            switch (kind)
            {
            case InkAnalysisDrawingKind::Rectangle:
                g_shapeResult += L"RECT(";
                break;
            case InkAnalysisDrawingKind::Circle:
                g_shapeResult += L"CIRCLE(";
                break;
            case InkAnalysisDrawingKind::Ellipse:
                g_shapeResult += L"ELLIPSE(";
                break;
            case InkAnalysisDrawingKind::Triangle:
                g_shapeResult += L"TRIANGLE(";
                break;
            default:
                g_shapeResult += L"POLYGON(";
                break;
            }

            g_shapeResult += std::to_wstring(box.X)      + L"," +
                             std::to_wstring(box.Y)      + L"," +
                             std::to_wstring(box.Width)  + L"," +
                             std::to_wstring(box.Height) + L");";
        }

        return g_shapeResult.c_str();
    }

} // extern "C"
