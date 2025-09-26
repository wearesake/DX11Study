#pragma once
#include "../include/Graphics.h"

class Bindable
{
public:
    virtual void Bind( Graphics& gfx ) noexcept = 0;
    virtual ~Bindable() = default;

protected:
    static ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept;
    static ID3D11Device* GetDevice( Graphics& gfx ) noexcept;
    static DxgiInfoManager& GetInfoManager( Graphics& gfx ) noexcept(!IS_DEBUG);    //!IS_DEBUG → 如果不是 Debug 模式，就标记为 noexcept(true)；如果是 Debug 模式，就允许抛异常。

};