#pragma once
#include "../Include/Graphics.h"
#include <DirectXMath.h>
#include <memory>

class Bindable;

class Drawable
{
public:
    Drawable() = default;
    Drawable( const Drawable & ) = delete;
    virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
    void Draw(Graphics &gfx) const noexcept(!_DEBUG);
    virtual void Update(float dt) noexcept = 0;
    void AddBind( std::unique_ptr<Bindable> bind) noexcept(!_DEBUG);
    void AddIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf) noexcept;
    virtual ~Drawable() = default;
private:
    const IndexBuffer *m_pIndexBuffer = nullptr;
    std::vector<std::unique_ptr<Bindable>> m_binds;
};