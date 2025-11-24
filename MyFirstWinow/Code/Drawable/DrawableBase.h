#pragma once
#include <random>

#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
public:
    bool IsStaticInitialized() const noexcept
    {
        return !staticBinds.empty();
    }
    void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept(!_DEBUG)
    {
        assert("Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
        staticBinds.push_back( std::move(bind) );
    }
    void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> ibuf ) noexcept(!_DEBUG)
    {
        assert( "Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr );
        m_pIndexBuffer = ibuf.get();
        staticBinds.push_back( std::move(ibuf) );
    }
    void SetIndexFromStatic() noexcept(!_DEBUG)
    {
        assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
        for (const auto &b : staticBinds)
        {
            if (const auto p = dynamic_cast<IndexBuffer*>(b.get()) )
            {
                m_pIndexBuffer = p;
                return;
            }
        }
        assert("Failed to find index buffer in static binds" && m_pIndexBuffer != nullptr);
    }
    
private:
    const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
    {
        return staticBinds;
    }
private:
    static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

//必须要在类外部提供 static 成员的定义，T 模版类的使用就是为了让不同模型共享一个静态成员
template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;