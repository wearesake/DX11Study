#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
    Topology(Graphics & gfx, D3D11_PRIMITIVE_TOPOLOGY primitiveType);
    void Bind(Graphics & gfx) noexcept override;
protected:
    D3D11_PRIMITIVE_TOPOLOGY m_primitiveType;
};