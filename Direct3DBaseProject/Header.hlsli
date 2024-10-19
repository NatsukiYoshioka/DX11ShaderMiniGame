//ワールド、ビュー、プロジェクション行列の定数
cbuffer Constants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

cbuffer SkinnedConstants : register(b1)
{
    float4x3 Bones[72];
}

cbuffer Light : register(b2)
{
    float3 LightDirection;
    float LightRange;
    float3 LightPosition;
    float LightAngle;
    float3 EyePosition;
}

cbuffer LVP : register(b3)
{
    float4x4 LVP;
}

//頂点構造体
struct VSOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
};

//ピクセル構造体
struct PSOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXVOORD1;
    float4 PosInLVP : TEXCOORD2;
};

struct BlockVS
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4x4 instanceTransform : INSTANCE_TRANSFORM;
    int textureID : TEXCOORD1;
};

struct BlockPS
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXCOORD2;
    float4 PosInLVP : TEXCOORD3;
    int textureID : TEXCOORD1;
};

struct AOVS
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

struct AOPS
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};