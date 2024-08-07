#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout) : SV_TARGET0
{
    float4 color = Texture.Sample(Sampler, pout.TexCoord);
	return color;
}