//ワールド、ビュー、プロジェクション行列の定数
cbuffer Constants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
}

//ボーン変換行列定数
cbuffer SkinnedConstants : register(b1)
{
    float4x3 Bones[72];
}

//ライト情報
cbuffer Light : register(b2)
{
    float3 LightDirection;
    float LightRange;
    float3 LightPosition;
    float LightAngle;
    float3 EyePosition;
    float3 LightColor;
}

//ライトビュープロジェクション行列
cbuffer LVP : register(b3)
{
    float4x4 LVP;
}

//スクリーンサイズ定数
cbuffer ScreenSize : register(b4)
{
    float2 ScreenSize;
}

//ブラー用定数バッファ
cbuffer Blur : register(b5)
{
    float4 weight[2];
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

//ブロック用インスタンシング頂点構造体
struct BlockVS
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4x4 instanceTransform : INSTANCE_TRANSFORM;
    int textureID : TEXCOORD1;
};

//ブロック用ピクセル構造体
struct BlockPS
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXCOORD2;
    float4 PosInLVP : TEXCOORD3;
    int textureID : TEXCOORD1;
};

//ディザパターン
static const int pattern[4][4] =
{
    { 0, 32, 8, 40 },
    { 48, 16, 56, 24 },
    { 12, 44, 4, 36 },
    { 60, 28, 52, 20 }
};

//ポストプロセス用頂点構造体
struct PPVS
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};

//ポストプロセス用ピクセル構造体
struct PPPS
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
};

//ガウシアンブラー用ピクセル構造体
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