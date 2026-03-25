# InkReco — Windows Ink + WinRT + Qt Wrapper

InkReco is a lightweight C++ wrapper that brings **Windows Ink (WinRT)** handwriting, stroke collection, and recognition APIs into **Qt applications**.  
It provides a clean, Qt‑friendly interface for capturing ink strokes, recognizing handwriting, and integrating Windows Ink features without dealing directly with COM, WinRT projections, or platform‑specific boilerplate.

---

## ✨ Features

### ✔️ Windows Ink Integration
- Wraps the native **Windows.UI.Input.Inking** APIs  
- Supports `InkStroke`, `InkStrokeBuilder`, and `InkRecognizerContainer`  
- Converts WinRT ink data into Qt‑friendly structures

### ✔️ Handwriting Recognition
- Uses the built‑in Windows handwriting recognizer  
- Async recognition with Qt signals  
- Returns recognized text and shapes

### ✔️ Qt‑Friendly API
- Pure C++/Qt wrapper classes  
- No need to interact with WinRT directly  
- Signals/slots for:
  - `textReady(QString)`
  - `shapesReady(QString)`

### ✔️ Cross‑Component Integration
- Designed to plug into:
  - `QGraphicsScene` drawing tools  
  - Custom brush engines  
  - Stroke smoothing pipelines  
  - Ink analysis modules

---

## 📦 Architecture Overview
Qt UI Layer
│
├── UnifiedInkRecognizer (Qt wrapper)
│       ├── Manages WinRT recognizer
│       ├── Converts strokes between Qt ↔ WinRT
│       └── Emits Qt signals with recognition results
│
WinRT Layer (Windows.UI.Input.Inking)
├── InkStrokeBuilder
├── InkRecognizerContainer
└── InkStroke

The wrapper isolates all WinRT complexity so your Qt code stays clean and portable.

---

## 🚀 Getting Started

### 1. Enable WinRT in your Qt project
Add this to your `.pro` or CMake file:
### 1. Include the wrapper
#include "UnifiedInkRecognizer.h"


### 2. Connect signals
connect(recognizer, &UnifiedInkRecognizer::textReady,
        this, &YourClass::onTextReady);

connect(recognizer, &UnifiedInkRecognizer::shapesReady,
        this, &YourClass::onShapesReady);


### 3. recognizer
recognizer->addStroke(points);
recognizer->recognizeAsync();

### 4. Text Recognition:
"hello world"
### 5. Shape Recognition:
"circle, rectangle, arrow"
### License
MIT License — free for commercial and open‑source use.

### Contributing
Pull requests are welcome!
If you want to extend the wrapper (e.g., stroke smoothing, shape analysis, pressure curves), feel free to open an issue.

**qmake:**
```pro
CONFIG += c++17
LIBS += -lruntimeobject

**CMake:**
target_link_libraries(${PROJECT_NAME} PRIVATE runtimeobject)

