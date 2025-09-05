#include <Windows.h>
#include "Code/Include/App.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR pCmdLine, _In_ int nCmdShow)
{
    try
    {
        return App{}.Go();
    }
    catch (const MyException& e)
    {
        MessageBox(nullptr, e.what(), "Error", MB_OK);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Error", MB_OK);
    }
    catch (...)
    {
        MessageBox(nullptr, "Unknown error", "Error", MB_OK);
    }
    return -1;
    
}
