#pragma once
#include "ConstantBuffers.h"
#include "../Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
    TransformCbuf( Graphics& gfx, const Drawable& parent );
    void Bind( Graphics& gfx ) noexcept override;
private:
    //常用于图形学的 变换矩阵，例如：
    //世界矩阵（Model）
    //视图矩阵（View）
    //投影矩阵（Projection）
    //或者它们的组合（MVP）
    VertexConstantBuffer<DirectX::XMMATRIX> vcbuf; //DirectX::XMMATRIX 是 4×4 矩阵类型
    const Drawable& parent;
};