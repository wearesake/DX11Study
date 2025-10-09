//float4 main(float3 color : Color) : SV_Target
//{
//    return float4(color, 1.0f);
//}

cbuffer CBuf
{
    float4 face_colors[6];
};
//SV_PrimitiveID :当前正在绘制的三角形的索引（ID）。
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
    //因为每个立方体面由 2 个三角形 构成；
    //所以用整数除法 / 2 可以让相邻的两个三角形共享同一个颜色；
    return face_colors[tid / 2];
}