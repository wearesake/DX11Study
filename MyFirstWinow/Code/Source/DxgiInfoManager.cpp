#include "../Include/DxgiInfoManager.h"
#include "../Include/MyWindow.h"
#include "../Include/Graphics.h"
#include "../../Macros/WindowsThrowMacros.h"
#include "../../Macros/GraphicsThrowMacros.h"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")


DxgiInfoManager::DxgiInfoManager()
{
    typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);        //函数指针的写法 (返回类型 函数名)(参数);
    
    const auto hModDxgiDebug = LoadLibraryEx( "dxgidebug.dll",nullptr,LOAD_LIBRARY_SEARCH_SYSTEM32 );
    if (hModDxgiDebug == nullptr)
    {
        throw CHWND_LAST_EXCEPT();
    }

    const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress( hModDxgiDebug,"DXGIGetDebugInterface" ) ); 
    if (DxgiGetDebugInterface == nullptr)
    {
        throw CHWND_LAST_EXCEPT();
    }

    HRESULT hr;
    GFX_THROW_NOINFO( DxgiGetDebugInterface( __uuidof(IDXGIInfoQueue),(void**)&pDxgiInfoQueue ) );
}


void DxgiInfoManager::Set() noexcept
{
    next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
    std::vector<std::string> messages;
    const auto end = pDxgiInfoQueue->GetNumStoredMessages( DXGI_DEBUG_ALL );
    for( auto i = next; i < end; i++ )
    {
        HRESULT hr;
        SIZE_T messageLength;
        // get the size of message i in bytes
        GFX_THROW_NOINFO( pDxgiInfoQueue->GetMessage( DXGI_DEBUG_ALL,i,nullptr,&messageLength ) );
        // allocate memory for message
        auto bytes = std::make_unique<byte[]>( messageLength );
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
        // get the message and push its description into the vector
        GFX_THROW_NOINFO( pDxgiInfoQueue->GetMessage( DXGI_DEBUG_ALL,i,pMessage,&messageLength ) );
        messages.emplace_back( pMessage->pDescription );
    }
    return messages;
}


