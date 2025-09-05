#include "../Include/Mouse.h"

#include <random>

void Mouse::Deal(MouseStatus status, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    switch (status)
    {
    case MouseStatus::CLICKED:
        onClick(hwnd, wParam, lParam);
        break;
    case MouseStatus::DOUBLE_CLICKED:
        onDoubleClick(hwnd, wParam, lParam);
        break;
    case MouseStatus::MOVE:
        onMouseMove(hwnd, wParam, lParam);
        break;
    case MouseStatus::RELEASED:
        onRelease(hwnd, wParam, lParam);
        break;
    default:
        break;
    }
}

void Mouse::onClick(HWND hwnd, WPARAM wParam, LPARAM lParam)
{ 
    m_isDrawing = true;
    m_start.x = LOWORD(lParam);
    m_start.y = HIWORD(lParam);
    m_end = m_start;
    SetCapture(hwnd);
}

void Mouse::onRelease(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    m_isDrawing = false;
    m_end.x = LOWORD(lParam);
    m_end.y = HIWORD(lParam);
    ReleaseCapture();
}

void Mouse::onDoubleClick(HWND hwnd, WPARAM wParam, LPARAM lParam){}
void Mouse::onMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam){
    drawMousePath(hwnd, wParam, lParam);
}
void Mouse::drawMousePath(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    static POINT prevPoint = { -1, -1 };
    if (wParam & MK_LBUTTON)
    {
        HDC hdc = GetDC(hwnd);
        SetROP2(hdc, R2_NOTXORPEN);
        POINT currentPoint;
        currentPoint.x = LOWORD(lParam);
        currentPoint.y = HIWORD(lParam);

        if (prevPoint.x != -1 && prevPoint.y != -1)
        {
            MoveToEx(hdc, prevPoint.x, prevPoint.y, NULL);
            LineTo(hdc, currentPoint.x, currentPoint.y);

        }
        prevPoint = currentPoint;
        ReleaseDC(hwnd, hdc);

    }
    else
    {
        prevPoint.x = -1;
        prevPoint.y = -1;
    }
}

int Mouse::generateRandom(int min, int max) { return 0; }
bool Mouse::isInCircle(int x, int y, int centerX, int centerY, int radius){ return FALSE; }
void Mouse::drawBoxLine(HWND hwnd, POINT start, POINT end){}
void Mouse::drawBox(HWND hwnd, WPARAM wParam, LPARAM lParam){}