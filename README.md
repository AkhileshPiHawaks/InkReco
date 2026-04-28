# InkReco — Windows Ink + WinRT + Qt Wrapper

InkReco is a lightweight C++ wrapper that brings **Windows Ink (WinRT)** handwriting, stroke collection, and recognition APIs into **Qt applications**.  
It provides a clean, Qt‑friendly interface for capturing ink strokes, recognizing handwriting, and integrating Windows Ink features without dealing directly with COM, WinRT projections, or platform complexity.

---

## ✨ Features

### ✔️ Windows Ink Integration
- Wraps the native **Windows.UI.Input.Inking** APIs  
- Supports `InkStroke`, `InkStrokeBuilder`, and `InkRecognizerContainer`  
- Converts WinRT ink data into Qt‑friendly structures

### ✔️ Handwriting Recognition
- Uses the built‑in Windows handwriting recognizer  
- Async recognition with Qt signals  
- Returns recognized text and shape classifications

### ✔️ Qt‑API
- Pure C++/Qt wrapper classes  
- No need to interact with WinRT directly  
- Signals/slots for:
  - `textReady(QString)` — recognized text result
  - `shapesReady(QString)` — recognized shape result

### ✔️ Cross‑Component Integration
- Designed to plug into:
  - `QGraphicsScene` drawing tools  
  - Custom brush engines  
  - Stroke smoothing pipelines  
  - Ink analysis modules

---

## 📋 Prerequisites

- **Windows 10/11** (WinRT is Windows-only)
- **Qt 5.12 or later** (Qt 6.x supported)
- **Visual Studio 2017 or later** with C++17 support
- **Windows SDK** (included with Visual Studio)

---

## 📦 Architecture Overview

```
┌─────────────────────────────────────┐
│         Qt UI Layer                 │
├─────────────────────────────────────┤
│   UnifiedInkRecognizer (Qt wrapper) │
│   ├─ Manages WinRT recognizer       │
│   ├─ Converts strokes (Qt ↔ WinRT)  │
│   └─ Emits Qt signals with results  │
├─────────────────────────────────────┤
│   WinRT Layer                       │
│   (Windows.UI.Input.Inking)         │
│   ├─ InkStrokeBuilder               │
│   ├─ InkRecognizerContainer         │
│   └─ InkStroke                      │
└─────────────────────────────────────┘
```

The wrapper isolates all WinRT complexity so your Qt code stays clean and portable.

---

## 🚀 Getting Started

### 1. Prerequisites & Setup

Ensure your project links against the WinRT runtime library.

**Using qmake:**
```pro
CONFIG += c++17
LIBS += -lruntimeobject
```

**Using CMake:**
```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE runtimeobject)
```

### 2. Include the Wrapper

```cpp
#include "UnifiedInkRecognizer.h"
```

### 3. Create an Instance

```cpp
UnifiedInkRecognizer recognizer;
```

### 4. Connect Signals

```cpp
connect(&recognizer, &UnifiedInkRecognizer::textReady,
        this, &YourClass::onTextRecognized);

connect(&recognizer, &UnifiedInkRecognizer::shapesReady,
        this, &YourClass::onShapeRecognized);
```

### 5. Add Strokes & Recognize

```cpp
// Collect stroke points (e.g., from mouse/pen events)
QVector<QPointF> strokePoints;
strokePoints << QPointF(0, 0) << QPointF(10, 10) << QPointF(20, 5);

// Add the stroke
recognizer.addStroke(strokePoints);

// Trigger async recognition
recognizer.recognizeAsync();
```

---

## 💻 Complete Example

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include "UnifiedInkRecognizer.h"

class InkWindow : public QMainWindow {
    Q_OBJECT
public:
    InkWindow() : recognizer(nullptr) {
        auto centralWidget = new QWidget(this);
        auto layout = new QVBoxLayout(centralWidget);
        
        outputLabel = new QLabel("Draw something...", this);
        layout->addWidget(outputLabel);
        
        setCentralWidget(centralWidget);
        setWindowTitle("InkReco Demo");
        resize(400, 300);
        
        // Initialize recognizer
        recognizer = new UnifiedInkRecognizer(this);
        
        connect(recognizer, &UnifiedInkRecognizer::textReady,
                this, &InkWindow::onTextReady);
        connect(recognizer, &UnifiedInkRecognizer::shapesReady,
                this, &InkWindow::onShapeReady);
    }
    
private slots:
    void onTextReady(const QString &text) {
        outputLabel->setText("Text: " + text);
    }
    
    void onShapeReady(const QString &shape) {
        outputLabel->setText("Shape: " + shape);
    }
    
private:
    QLabel *outputLabel;
    UnifiedInkRecognizer *recognizer;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    InkWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"
```

---

## 📚 API Reference

### `UnifiedInkRecognizer`

#### Methods

| Method | Description |
|--------|-------------|
| `addStroke(QVector<QPointF>)` | Add a stroke (line) to the recognizer |
| `recognizeAsync()` | Start async handwriting recognition |
| `clear()` | Clear all collected strokes |

#### Signals

| Signal | Parameters | Description |
|--------|------------|-------------|
| `textReady` | `QString text` | Emitted when text recognition completes |
| `shapesReady` | `QString shape` | Emitted when shape recognition completes |
| `recognitionError` | `QString error` | Emitted if recognition fails |

---

## ⚙️ Thread Safety

- Recognition runs on a background thread
- All signals are thread‑safe (use `Qt::AutoConnection`)
- Do not call `addStroke()` while `recognizeAsync()` is running

---

## 🐛 Troubleshooting

### "runtimeobject not found" error
- Ensure Windows SDK is installed with Visual Studio
- Verify `LIBS += -lruntimeobject` is in your `.pro` file
- Rebuild your project

### Recognition returns empty string
- Ensure strokes are added before calling `recognizeAsync()`
- Verify Windows Handwriting Recognition is enabled in Settings
- Check that the recognizer language matches your input

### Application crashes on exit
- Ensure `UnifiedInkRecognizer` is properly cleaned up (use `QObject` parent)
- Avoid calling methods after recognizer destruction

---

## 📄 License

MIT License — free for commercial and open‑source use.

See `LICENSE` file for details.

---

## 🤝 Contributing

Pull requests are welcome! Areas for contribution:

- **Stroke smoothing algorithms** — improve ink quality
- **Pressure curve support** — stylus pressure sensitivity
- **Shape analysis** — advanced geometric recognition
- **Performance optimization** — reduce recognition latency
- **Documentation** — examples and guides

To contribute:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Commit changes (`git commit -m 'Add my feature'`)
4. Push to branch (`git push origin feature/my-feature`)
5. Open a Pull Request

---

## 📞 Support

For issues, questions, or feature requests, please open a [GitHub Issue](../../issues).

---

**Qt developers**
