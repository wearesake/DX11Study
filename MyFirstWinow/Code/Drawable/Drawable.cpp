#include "Drawable.h"
#include "../../Macros/GraphicsThrowMacros.h"
#include "../Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics &gfx) const noexcept(!_DEBUG)
{
    for (auto &b : mBinds)
    {
        b->Bind(gfx);
    }
    gfx.DrawIndexed( m_pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
    assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
    assert("Attempting to add index buffer to bind a second time" && m_pIndexBuffer == nullptr);
    m_pIndexBuffer = ibuf.get();
    mBinds.push_back(std::move(ibuf));
}