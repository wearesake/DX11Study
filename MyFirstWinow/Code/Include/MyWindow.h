#pragma once
#include <string>
#include <Windows.h>
#include <optional>
#include "Gdi.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
#include "MyException.h"

constexpr UINT WM_BUTTON_CREATE = WM_USER + 1;
constexpr UINT WM_CHECKBOX_CREATE = WM_USER + 2;
constexpr UINT IDB_BUTTON = 1001;
constexpr UINT IDB_CHECKBOX = 1002;
struct ButtonContent {
    std::string text;
    int x;
    int y;
    int width;
    int height;
};
struct CheckBoxContent {
    std::string text;
    int x;
    int y;
    int width;
    int height;
};

class MyWindow {
public:
    class Exception : public MyException {
        using MyException::MyException;
    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };
    class HrException : public Exception {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorDescription() const noexcept;
    private:
        HRESULT hr;
    };
    class NoGuardException : public Exception {
    public:
        using Exception::Exception;
        const  char* GetType() const noexcept override;
    };

public:
    explicit MyWindow(std::string className);
    ~MyWindow();
    MyWindow(const MyWindow& other) = delete;
    MyWindow(MyWindow& other) noexcept = delete;
    MyWindow& operator=(const MyWindow&) = delete;
    MyWindow& operator=(MyWindow&&) noexcept = delete;

public:
    void Button(const std::string& text, int x, int y, int width = 150, int height = 30);
    void CheckBox(const std::string& text, int x, int y, int width = 150, int height = 30);
    void onDealCheckBox(HWND hwnd, WPARAM wparam, LPARAM lparam);
private:
    bool button(HWND hwnd, const std::string& text, int x, int y, int width = 150, int height = 30);
    bool checkBox(HWND hwnd, const std::string& text, int x, int y, int width = 150, int height = 30);

public:
    //std::optional<T> 表示一个 可选值：要么存有一个类型为 T 的值，要么为空（std::nullopt）；
    std::optional<bool> Begin(const std::string& name);
    std::optional<bool> Begin(const std::string& name, int w, int h);
    std::optional<bool> Begin(const std::string& name, int x, int y, int width, int height);
    std::optional<bool> BeginAt(const std::string& name, int x, int y);
    static LRESULT CALLBACK MessageProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    std::string m_className;
    HINSTANCE m_hinstance = GetModuleHandle(nullptr);
    HWND m_hwnd = nullptr;
    int m_x = CW_USEDEFAULT;
    int m_y = CW_USEDEFAULT;
    int m_width = CW_USEDEFAULT;
    int m_height = CW_USEDEFAULT;
    Gdi m_gdi;
    KeyBoard m_kb;
    Mouse m_mouse;
    
private:
    std::unique_ptr<Graphics> m_graphics;


public:
    HWND GetHwnd()
    {
        return m_hwnd;
    }

    Graphics& GetGraphics();
  

    static std::optional<int> ProcessMessages();
};
