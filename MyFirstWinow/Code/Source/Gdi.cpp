#include "../Include/Gdi.h"
#include <format>
#include "DrawType.cpp"
#include "../../resource.h"


void Gdi::OnPaint(HWND hwnd, int drawType)
{
    switch (drawType)
    {
    case DRAW_POINT:
        drawPoint(hwnd);
        break;
    case DRAW_LINE:
        drawLine(hwnd);
        break;
    case DRAW_RECT:
        drawRectangle(hwnd);
        break;
    case DRAW_RECT_TEXT:
        drawRectangleWithText(hwnd);
        break;
    case DRAW_RECT_RGN:
        drawRectangleWithRgn(hwnd);
        break;
    case DRAW_IMG:
        drawImage(hwnd);
        break;
    default:
        break;
    }
    drawPoint(hwnd);
    //drawLine(hwnd);
}

void Gdi::OnCreate()
{

}

void Gdi::drawText(HWND hwnd)
{
    OutputDebugStringA("Gdi::OnPaint\n");
    PAINTSTRUCT ps;
    InvalidateRect(hwnd, nullptr, true);
    HDC hdc = BeginPaint(hwnd, &ps);
    TEXTMETRICA tm;
    GetTextMetrics(hdc, &tm);
    auto fontInfo = std::format("this font height is {}, width is {}", tm.tmHeight, tm.tmWeight);
    OutputDebugStringA(fontInfo.c_str());
    SetTextColor(hdc, RGB(255, 0, 0));
    //TextOut(hdc, 400, 400, m_text.c_str(), m_text.length());
    DrawText(hdc, m_text.c_str(), m_text.length(), &ps.rcPaint, DT_LEFT | DT_TOP | DT_SINGLELINE);
    EndPaint(hwnd, &ps);
}

void Gdi::drawPoint(HWND hwnd)
{
    /*1.*/
    //PAINTSTRUCT ps;
    //HDC hdc = BeginPaint(hwnd, &ps);
    //SetPixel(hdc, 100, 100, RGB(255, 0, 0));
    ////SetPixel(hdc, 100, 100, RGB(255, 0, 0));
    //EndPaint(hwnd, &ps);

    /*2.*/
    auto hDc = GetDC(hwnd);
    SetPixel(hDc, 100, 100, RGB(255, 0, 0));
    COLORREF color = GetPixel(hDc, 100, 100);
    ReleaseDC(hwnd, hDc);

}

void Gdi::drawLine(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    auto hpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, hpen);
    /*1.����*/
    //MoveToEx(hdc, 100, 100, nullptr);
    //LineTo(hdc, 200, 200);
    /*2.�㻭*/
    POINT pt[4] = {100, 100, 200, 200, 100, 200, 200, 100};
    Polyline(hdc, pt, 4);

    EndPaint(hwnd, &ps);
    DeleteObject(hpen);

}

void Gdi::drawRectangle(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    //�û��ʻ�
    auto hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    //��ˢ�ӻ�
    //auto hbrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hpen);
    Rectangle(hdc, 100, 100, 200, 200);
    EndPaint(hwnd, &ps);
    DeleteObject(hpen);

}

void Gdi::drawRectangleWithText(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    auto hbrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, hbrush);
    Rectangle(hdc, 100, 100, 200, 200);
    RECT rc = {100, 100, 200, 200};
    DrawText(hdc, m_text.c_str(), m_text.length(), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    DeleteObject(hbrush);
    EndPaint(hwnd, &ps);
}

void Gdi::drawRectangleWithRgn(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    auto hbrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, hbrush);
    auto hrgn = CreateRectRgn(150, 150, 180, 180);
    SelectObject(hdc, hrgn);
    Rectangle(hdc, 100, 100, 200, 200);
    DeleteObject(hbrush);
    DeleteObject(hrgn);
    EndPaint(hwnd, &ps);
}

void Gdi::drawImage(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    auto image = LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    auto hMemDc = CreateCompatibleDC(hdc);
    SelectObject(hMemDc, image);
    BITMAP bitmap;
    GetObject(image, sizeof(BITMAP), &bitmap);
    //BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDc, 0, 0, SRCCOPY);
    RECT rc = {0, 0, 600, 600};
    StretchBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMemDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
    DeleteDC(hMemDc);
    DeleteObject(image);
    EndPaint(hwnd, &ps);

}

void Gdi::DrawTextToWindow(HWND hwnd, const std::string& text)
{
    HDC hdc = GetDC(hwnd);
    SetTextColor(hdc, RGB(255, 0, 0));
    RECT rc;
    GetClientRect(hwnd, &rc);
    DrawText(hdc, text.c_str(), text.length(), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    ReleaseDC(hwnd, hdc);
}