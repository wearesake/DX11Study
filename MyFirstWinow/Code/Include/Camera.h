#pragma once
#include "Graphics.h"

class Camera
{
public:
    DirectX::XMMATRIX GetMatrix() const noexcept;
    void SpawnControlWindow() noexcept;
    void Reset() noexcept;

private:
    // 球坐标系
    float r = 20.0f;
    float theta = 0.0f;
    float phi = 0.0f;
    // 旋转（欧拉角）
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
};
