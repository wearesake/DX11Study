#include "../Include/MyWindow.h"
#include "../Include/Tools.h"
#include "../../Macros/WindowsThrowMacros.h"
#include "../Source/DrawType.cpp"
#include "../Include/KeyBoardEvent.h"
#include "../../resource.h"
#include <sstream>
#include "../Imgui/imgui_impl_win32.h"
//#include "Mouse.h"

// Window Class Stuff
MyWindow::WindowClass MyWindow::WindowClass::wndClass;

MyWindow::WindowClass::WindowClass() noexcept
    :
    hInst( GetModuleHandle( nullptr ) )
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof( wc );
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage( 
        GetInstance(),MAKEINTRESOURCE( IDD_MYFIRSTWINOW_DIALOG ),
        IMAGE_ICON,32,32,0
    ));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(
        GetInstance(),MAKEINTRESOURCE( IDD_MYFIRSTWINOW_DIALOG ),
        IMAGE_ICON,16,16,0
    ));
    RegisterClassEx( &wc );
}

MyWindow::WindowClass::~WindowClass()
{
    UnregisterClass( wndClassName,GetInstance() );
}

const char* MyWindow::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE MyWindow::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}


MyWindow::MyWindow( int width,int height,const char* name )
    :
    width( width ),
    height( height )
{
    // calculate window size based on desired client region size
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if( AdjustWindowRect( &wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE ) == 0 )
    {
        throw CHWND_LAST_EXCEPT();
    }
    // create window & get hWnd
    m_hwnd = CreateWindow(
        WindowClass::GetName(),name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT,CW_USEDEFAULT,wr.right - wr.left,wr.bottom - wr.top,
        nullptr,nullptr,WindowClass::GetInstance(),this
    );
    // check for error
    if( m_hwnd == nullptr )
    {
        throw CHWND_LAST_EXCEPT();
    }
    // newly created windows start off as hidden
    ShowWindow( m_hwnd,SW_SHOWDEFAULT );
    // Init ImGui Win32 Impl
    ImGui_ImplWin32_Init( m_hwnd );
    // create graphics object
    m_graphics = std::make_unique<Graphics>( m_hwnd );
}

MyWindow::MyWindow(std::string className) : m_className(std::move(className))
{ 
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpszClassName = m_className.c_str();
    wc.lpfnWndProc = MessageProc;
    wc.hInstance = m_hinstance;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    if (ATOM ret = RegisterClassEx(&wc); !ret)
    {
        Tools::LogErrorMsg(GetLastError());
    }
}

MyWindow::~MyWindow()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(m_hwnd);
    UnregisterClass(m_className.c_str(), m_hinstance);
}

std::optional<bool> MyWindow::Begin(const std::string& name)
{
    auto wnd = CreateWindowExA(NULL, m_className.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW, m_x, m_y, m_width, m_height, nullptr, nullptr, m_hinstance, this);

    if (!wnd)
    {
        Tools::LogErrorMsg(GetLastError());
        return false;
    }
    if (m_hwnd == nullptr)
    {
        m_hwnd = wnd;
    }
    ShowWindow(wnd, SW_SHOW);
    m_graphics = std::make_unique<Graphics>(wnd);

    return {};
    //等价于return std::nullopt;
}

std::optional<bool> MyWindow::Begin(const std::string& name, int w, int h)
{ 
    m_width = w;
    m_height = h;
    return Begin(name);
}

std::optional<bool> MyWindow::Begin(const std::string& name, int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    return Begin(name);
}

std::optional<bool> MyWindow::BeginAt(const std::string& name, int x, int y)
{
    m_x = x;
    m_y = y;
    return Begin(name);
}

LRESULT CALLBACK MyWindow::MessageProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{ 
    static MyWindow* pWnd = nullptr;
    if (msg == WM_NCCREATE && pWnd == nullptr)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
        pWnd = static_cast<MyWindow*>(pCreate->lpCreateParams);
    }
    if (pWnd != nullptr)
    {
        return pWnd->HandleMessage(hwnd, msg, wparam, lparam);
    }
    return DefWindowProcA(hwnd, msg, wparam, lparam);
}



LRESULT CALLBACK MyWindow::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ) )
    {
        return true;
    }
    
    switch (msg)
    { 
        case WM_CLOSE:
            if (hwnd == m_hwnd)
            {
                PostQuitMessage(0);
            }
            else
            {
                DestroyWindow(hwnd);
            }
            return -1;
        
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK MyWindow::HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if( msg == WM_NCCREATE )
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        MyWindow* const pWnd = static_cast<MyWindow*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWnd) );
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&MyWindow::HandleMsgThunk) );
        // forward message to window instance handler
        return pWnd->HandleMessage( hWnd,msg,wParam,lParam );
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc( hWnd,msg,wParam,lParam );
}

LRESULT CALLBACK MyWindow::HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
    // retrieve ptr to window instance
    MyWindow* const pWnd = reinterpret_cast<MyWindow*>(GetWindowLongPtr( hWnd,GWLP_USERDATA ));
    // forward message to window instance handler
    return pWnd->HandleMessage( hWnd,msg,wParam,lParam );
}



Graphics& MyWindow::GetGraphics()
{
    if (!m_graphics)
    {
        throw CHWND_NOGFX_EXCEPT();
    }
    return *m_graphics;
}

std::optional<int> MyWindow::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    { 
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

void MyWindow::Button(const std::string& text, int x, int y, int width, int height)
{
    auto ctx = std::make_unique<ButtonContent>();
    ctx->text = text;
    ctx->x = x;
    ctx->y = y;
    ctx->width = width;
    ctx->height = height;
    PostMessage(m_hwnd, WM_BUTTON_CREATE, 0, reinterpret_cast<LPARAM>(ctx.release()));
}

void MyWindow::CheckBox(const std::string& text, int x, int y, int width, int height)
{
    auto ctx = std::make_unique<CheckBoxContent>();
    ctx->text = text;
    ctx->x = x;
    ctx->y = y;
    ctx->width = width;
    ctx->height = height;
    PostMessage(m_hwnd, WM_CHECKBOX_CREATE, 0, reinterpret_cast<LPARAM>(ctx.release()));
}

bool MyWindow::button(HWND hwnd, const std::string& text, int x, int y, int width, int height)
{
    HWND h_button = CreateWindow("Button", text.c_str(),
        WS_VISIBLE | WS_CHILD,
        x, y, width, height,
        hwnd,
        (HMENU)IDB_BUTTON,
        m_hinstance,
        NULL);
    if (h_button == nullptr) { return false; }
    return true;
}

bool MyWindow::checkBox(HWND hwnd, const std::string& text, int x, int y, int width, int height)
{
    HWND h_checkBox = CreateWindow("Button", text.c_str(),
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
        x, y, width, height,
        hwnd,
        (HMENU)IDB_CHECKBOX,
        m_hinstance,
        NULL);
    if (h_checkBox == nullptr) { return false; }
    return true;
}

void MyWindow::onDealCheckBox(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
    if (HIWORD(wparam) == BN_CLICKED)
    {
        HWND h_checkBox = (HWND)lparam;
        LRESULT check_state = SendMessage(h_checkBox, BM_GETCHECK, 0, 0);
        if (check_state == BST_CHECKED)
        {
            SendMessage(h_checkBox, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
        }
        else if (check_state == BST_UNCHECKED)
        {
            SendMessage(h_checkBox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
        }
    }
}

MyWindow::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
    : Exception(line, file), hr(hr)
{}

const char * MyWindow::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << " [Error code] " << GetErrorCode() << std::endl
        << " [Description] " << GetErrorDescription() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* MyWindow::HrException::GetType() const noexcept
{
    return "MyWindow Exception";
}

std::string MyWindow::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
    char * pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf),
        0,
        nullptr
    );
    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT MyWindow::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string MyWindow::HrException::GetErrorDescription() const noexcept
{
    return TranslateErrorCode(hr);
}

const char* MyWindow::NoGuardException::GetType() const noexcept
{
    return "MyWindow NoGuardException";
}
