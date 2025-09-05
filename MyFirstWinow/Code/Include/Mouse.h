#pragma once
#include "Windows.h"

enum class MouseStatus {
    CLICKED,
    RELEASED,
    DOUBLE_CLICKED,
    MOVE,
};

class Mouse {
public:
    Mouse() = default;
    ~Mouse() = default;

public:
    Mouse(const Mouse& other) = delete;
    Mouse(Mouse&& other) noexcept = delete;
    Mouse& operator=(const Mouse& other) = delete;
    Mouse& operator=(Mouse&& other) noexcept = delete;

public:
    void Deal(MouseStatus status, HWND hwnd, WPARAM wParam, LPARAM lParam);

private:
    void onClick(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onRelease(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onDoubleClick(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam);

private:
    void drawMousePath(HWND hwnd, WPARAM wParam, LPARAM lParam);
    int generateRandom(int min, int max);
    bool isInCircle(int x, int y, int centerX, int centerY, int radius);
    void drawBoxLine(HWND hwnd, POINT start, POINT end);
    void drawBox(HWND hwnd, WPARAM wParam, LPARAM lParam);

private:
    POINT m_start = {0, 0};
    POINT m_end = {0, 0};
    bool m_isDrawing = false;
    RECT m_rect;
};

