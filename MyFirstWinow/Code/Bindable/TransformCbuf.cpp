#include "TransformCbuf.h"

TransformCbuf::TransformCbuf( Graphics& gfx, const Drawable& parent )
    : parent(parent)
{
    //make_unique是创建unique_ptr的辅助函数，避免内存泄露风险
    if (!pVcbuf)
    {
        pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>( gfx );
    }
}

void TransformCbuf::Bind( Graphics& gfx ) noexcept
{
    const auto model = parent.GetTransformXM();
    const Transforms tf =
    {
        //DirectXMath 用的是 Row-Major（行主序） - HLSL 默认用的是：Column-Major（列主序）
        DirectX::XMMatrixTranspose( model ),  //对一个 4×4 矩阵做“转置（Transpose）”
        DirectX::XMMatrixTranspose(
            model *
            gfx.GetCamera() *
            gfx.GetProjection()
        )
    };
    pVcbuf->Update( gfx,tf );
    pVcbuf->Bind( gfx );
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
