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
    for (auto &b : GetStaticBinds())
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
    m_pIndexBuffer = ibuf.get(); //.get() 返回 原始指针（不转移所有权）。 保存地址
    m_binds.push_back(std::move(ibuf));//🔹std::move(ibuf)：表示“转移所有权”。 std::unique_ptr 是独占指针，一个对象只能被一个 unique_ptr 拥有； 所以必须显式使用 std::move() 把 ibuf 移动进容器中。
}