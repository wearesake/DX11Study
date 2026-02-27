#include "TransformCbuf.h"

TransformCbuf::TransformCbuf( Graphics& gfx, const Drawable& parent )
    : parent(parent)
{
    //make_unique是创建unique_ptr的辅助函数，避免内存泄露风险
    if (!pVcbuf)
    {
        pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>( gfx );
    }
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
    pVcbuf->Update( gfx, DirectX::XMMatrixTranspose(
            parent.GetTransformXM() *
            gfx.GetCamera() *
            gfx.GetProjection()
        ) );
    //gfx.GetProjection() 返回当前的 投影矩阵。

    pVcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pVcbuf;