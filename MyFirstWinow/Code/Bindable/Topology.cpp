#include "Topology.h"

Topology::Topology( Graphics & gfx, D3D11_PRIMITIVE_TOPOLOGY primitiveType) : m_primitiveType(primitiveType)
{
}

void Topology::Bind(Graphics & gfx)
{
    GetContext(gfx)->IASetPrimitiveTopology(m_primitiveType);
}