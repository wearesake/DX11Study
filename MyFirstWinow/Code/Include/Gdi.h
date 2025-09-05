#pragma once
#include <Windows.h>
#include <string>

class Gdi {
public:
    Gdi() = default;
    ~Gdi() = default;
    Gdi(const Gdi& other) = delete;
    Gdi(Gdi&& other) noexcept = delete;
    Gdi& operator=(const Gdi& other) = delete;
    Gdi& operator=(Gdi&& other) noexcept = delete;

public:
    void OnPaint(HWND hwnd, int drawType);
    void OnCreate();

    static void DrawTextToWindow(HWND hwnd, const std::string& text);

private:
    void drawText(HWND hwnd);
    void drawTextWithFont(HWND hwnd);
    void drawPoint(HWND hwnd);
    void drawLine(HWND hwnd);
    void drawRectangle(HWND hwnd);
    void drawRectangleWithPen(HWND hwnd);
    void drawRectangleWithText(HWND hwnd);
    void drawRectangleWithRgn(HWND hwnd);
    void drawImage(HWND hwnd);

    

private:
    std::string m_text = "hello world";
};

