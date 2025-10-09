#include "Drawable.h"
#include "../../Macros/GraphicsThrowMacros.h"
#include "../Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics &gfx) const noexcept(!_DEBUG)
{
    for (auto &b : m_binds)
    {
        b->Bind(gfx);
    }
    gfx.DrawIndexed( m_pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!_DEBUG)
{
    assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
    m_binds.push_back( std::move( bind ) );
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
    assert("Attempting to add index buffer to bind a second time" && m_pIndexBuffer == nullptr);
    m_pIndexBuffer = ibuf.get();
    m_binds.push_back(std::move(ibuf));
}