#pragma once

#ifdef INKRECOBRIDGE_EXPORTS
#define INK_API __declspec(dllexport)
#else
#define INK_API __declspec(dllimport)
#endif

extern "C" {

struct BridgePoint {
    float x;
    float y;
    float t;
};

// Initialize WinRT + InkAnalyzer
INK_API void InkReco_Init();

// Clear all strokes
INK_API void InkReco_Clear();

// Set handwriting language (BCP‑47 tag, e.g. L"hi-IN")
INK_API bool InkReco_SetLanguage(const wchar_t* bcp47);

// Stroke input
INK_API void InkReco_BeginStroke();
INK_API void InkReco_AddPoint(const BridgePoint& p);
INK_API void InkReco_EndStroke();

// Recognition
INK_API const wchar_t* InkReco_RecognizeText();
INK_API const wchar_t* InkReco_RecognizeShapes();
}
