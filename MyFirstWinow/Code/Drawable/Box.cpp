#include "Box.h"
#include "../../Macros/GraphicsThrowMacros.h"

Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
    : r( rdist(rng) ), droll( ddist(rng) ), dpitch( ddist(rng) ),
        dyaw( ddist(rng) ), dphi( odist(rng) ), dtheta( odist(rng) ),
        dchi(odist (rng) ), chi( adist(rng) ), theta( adist(rng) ), phi( adist(rng))
{
    struct Vertex
    {
        struct
        {
            float X, Y, Z;
        }pos;
    };

    const std::vector<Vertex> vertices = {
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
    };
}
