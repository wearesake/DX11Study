#include "Box.h"
#include "../Bindable/BindableBase.h"
#include "../../Macros/GraphicsThrowMacros.h"

//随机引擎（engine）：负责产生随机比特流 这里是 std::mt19937（Mersenne Twister 算法）
//分布器（distribution）：将随机数映射到特定范围 ➜ 比如 uniform_real_distribution<float>(a, b) 表示 [a, b) 之间的均匀分布
Box::Box( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist )
	:
	r( rdist( rng ) ),
	droll( ddist( rng ) ),
	dpitch( ddist( rng ) ),
	dyaw( ddist( rng ) ),
	dphi( odist( rng ) ),
	dtheta( odist( rng ) ),
	dchi( odist( rng ) ),
	chi( adist( rng ) ),	//z轴角
	theta( adist( rng ) ), //水平角
	phi( adist( rng ) )	//垂直角
{

	if ( IsStaticInitialized() )
	{
		AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );
		return;
	}
	
	//ddist → 垂直方向（pitch） adist → 水平方向（yaw）
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};
	//{0, 0, 0} 通常会出现在屏幕的中心。
	const std::vector<Vertex> vertices =
	{
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f },
		{ -1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
	};
	AddBind( std::make_unique<VertexBuffer>( gfx,vertices ) );
	
	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

	auto pvs = std::make_unique<VertexShader>( gfx,L"./ShaderProject/VertexShader.cso" );
	auto pvsbc = pvs->GetBytecode();
	AddBind( std::move( pvs ) );
	

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) ); //创建并绑定 InputLayout, 告诉 GPU 如何把顶点数据传给 VS
	
	AddBind( std::make_unique<PixelShader>( gfx,L"./ShaderProject/PixelShader.cso" ) );

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.0f,0.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};

	AddBind( std::make_unique<PixelConstantBuffer<ConstantBuffer2>>( gfx,cb2 ) ); //提供颜色、光照、材质等参数
	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) ); //告诉 GPU 如何把顶点组合成几何体（如三角形、线段）

	AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );

}

void Box::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	//DirectX 是 行向量 × 矩阵 的方式运算（右乘生效）。
	//所以矩阵乘法从右往左执行。
	//模型坐标
	//→ (4) 整个系统平移到 z=20
	//→ (3) 围绕原点旋转（公转）
	//→ (2) 平移到半径 r 的位置
	//→ (1) 自身旋转（自转）
	return DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		DirectX::XMMatrixTranslation( r,0.0f,0.0f ) *
		DirectX::XMMatrixRotationRollPitchYaw( theta,phi,chi ) *
		DirectX::XMMatrixTranslation( 0.0f,0.0f,20.0f );
	//XMMatrixTranslation(0.0f, 0.0f, 20.0f)	场景平移	把整个物体系统移动到摄像机前方 z=20 的位置（否则物体可能在摄像机后面不可见）
	//XMMatrixRotationRollPitchYaw(theta, phi, chi)	公转旋转	模拟物体围绕某中心（比如原点）旋转
	//XMMatrixTranslation(r, 0.0f, 0.0f)	半径偏移	让物体沿 x 轴偏移 r 个单位（离旋转中心的距离）
	//XMMatrixRotationRollPitchYaw(pitch, yaw, roll)	自转	模拟物体自身旋转（例如地球自转）
}
