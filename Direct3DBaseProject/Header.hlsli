//���[���h�A�r���[�A�v���W�F�N�V�����s��̒萔
cbuffer Constants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

//���_�\����
struct VSOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 Tangent : TANGENT;
    float3 BiNormal : BINORMAL;
};

//�s�N�Z���\����
struct PSOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXCOORD1;
    float3 NormalInView : TEXCOORD2;
    float3 Tangent : TANGENT;
    float3 BiNormal : BINORMAL;
};