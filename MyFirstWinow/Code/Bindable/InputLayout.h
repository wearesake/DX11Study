#pragma once
#include "Bindable.h"

class InputLayout : public Bindable
{
public:
    InputLayout( Graphics &gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC> & layout,
        ID3DBlob * pVertexShaderBytecode );

    voif Bind(Graphics &gfx ) noexcept override;
protected:
    Microsoft::WRL::ComPtr<ID3DBlob> m_pInputLayout;
};