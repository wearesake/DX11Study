#include "TransformCbuf.h"

TransformCbuf::TransformCbuf( Graphics& gfx, const Drawable& parent )
    : vcbuf(gfx), parent(parent)
{
    
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
    vcbuf.Update( gfx, DirectX::XMMatrixTranspose(
            parent.GetTransformXM() * gfx.GetProjection()
        ) );
    //gfx.GetProjection() 返回当前的 投影矩阵。

    vcbuf.Bind(gfx);
}