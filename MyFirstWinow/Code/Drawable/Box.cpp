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
	AddBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );
	
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

	AddBind( std::make_unique<PixelConstantBuffer<ConstantBuffer2>>( gfx,cb2 ) );
	AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );

	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	
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
	return DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		DirectX::XMMatrixTranslation( r,0.0f,0.0f ) *
		DirectX::XMMatrixRotationRollPitchYaw( theta,phi,chi ) *
		DirectX::XMMatrixTranslation( 0.0f,0.0f,20.0f );
}
