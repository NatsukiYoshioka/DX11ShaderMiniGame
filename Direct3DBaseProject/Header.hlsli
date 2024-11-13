//���[���h�A�r���[�A�v���W�F�N�V�����s��̒萔
cbuffer Constants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

//�{�[���ϊ��s��萔
cbuffer SkinnedConstants : register(b1)
{
    float4x3 Bones[72];
}

//���C�g���
cbuffer Light : register(b2)
{
    float3 LightDirection;
    float LightRange;
    float3 LightPosition;
    float LightAngle;
    float3 EyePosition;
    float3 LightColor;
}

//���C�g�r���[�v���W�F�N�V�����s��
cbuffer LVP : register(b3)
{
    float4x4 LVP;
}

//�X�N���[���T�C�Y�萔
cbuffer ScreenSize : register(b4)
{
    float2 ScreenSize;
}

//�u���[�p�萔�o�b�t�@
cbuffer Blur : register(b5)
{
    float4 weight[2];
}

//���_�\����
struct VSOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
};

//�s�N�Z���\����
struct PSOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXVOORD1;
    float4 PosInLVP : TEXCOORD2;
};

//�u���b�N�p�C���X�^���V���O���_�\����
struct BlockVS
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4x4 instanceTransform : INSTANCE_TRANSFORM;
    int textureID : TEXCOORD1;
};

//�u���b�N�p�s�N�Z���\����
struct BlockPS
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXCOORD2;
    float4 PosInLVP : TEXCOORD3;
    int textureID : TEXCOORD1;
};

//�f�B�U�p�^�[��
static const int pattern[4][4] =
{
    { 0, 32, 8, 40 },
    { 48, 16, 56, 24 },
    { 12, 44, 4, 36 },
    { 60, 28, 52, 20 }
};

//�|�X�g�v���Z�X�p���_�\����
struct PPVS
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

//�|�X�g�v���Z�X�p�s�N�Z���\����
struct PPPS
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

//�K�E�V�A���u���[�p�s�N�Z���\����
struct BlurPS
{
    float4 Position : SV_Position;
    float4 TexCoord0 : TEXCOORD0;
    float4 TexCoord1 : TEXCOORD1;
    float4 TexCoord2 : TEXCOORD2;
    float4 TexCoord3 : TEXCOORD3;
    float4 TexCoord4 : TEXCOORD4;
    float4 TexCoord5 : TEXCOORD5;
    float4 TexCoord6 : TEXCOORD6;
    float4 TexCoord7 : TEXCOORD7;
};