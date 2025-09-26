#include "Bindable.h"


ID3D11DeviceContext* Bindable::GetContext( Graphics& gfx ) noexcept
{
    return gfx.pDeviceContext.Get();
}
ID3D11Device* Bindable::GetDevice( Graphics& gfx ) noexcept
{
    return gfx.pDevice.Get();
}
DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx ) noexcept(!_DEBUG)
{
    return gfx.infoManager;
}