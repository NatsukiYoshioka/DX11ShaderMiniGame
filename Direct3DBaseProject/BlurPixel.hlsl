#include"Header.hlsli"

Texture2D<float4> SceneTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(BlurPS pout) : SV_TARGET
{
    float4 color;
    
    //基準テクセルからプラス方向に重み付きでサンプリング
    color = weight[0].x * SceneTexture.Sample(Sampler, pout.TexCoord0.xy);
    color += weight[0].y * SceneTexture.Sample(Sampler, pout.TexCoord1.xy);
    color += weight[0].z * SceneTexture.Sample(Sampler, pout.TexCoord2.xy);
    color += weight[0].w * SceneTexture.Sample(Sampler, pout.TexCoord3.xy);
    color += weight[1].x * SceneTexture.Sample(Sampler, pout.TexCoord4.xy);
    color += weight[1].y * SceneTexture.Sample(Sampler, pout.TexCoord5.xy);
    color += weight[1].z * SceneTexture.Sample(Sampler, pout.TexCoord6.xy);
    color += weight[1].w * SceneTexture.Sample(Sampler, pout.TexCoord7.xy);
    
    //基準テクセルからマイナス方向に重み付きでサンプリング
    color += weight[0].x * SceneTexture.Sample(Sampler, pout.TexCoord0.zw);
    color += weight[0].y * SceneTexture.Sample(Sampler, pout.TexCoord1.zw);
    color += weight[0].z * SceneTexture.Sample(Sampler, pout.TexCoord2.zw);
    color += weight[0].w * SceneTexture.Sample(Sampler, pout.TexCoord3.zw);
    color += weight[1].x * SceneTexture.Sample(Sampler, pout.TexCoord4.zw);
    color += weight[1].y * SceneTexture.Sample(Sampler, pout.TexCoord5.zw);
    color += weight[1].z * SceneTexture.Sample(Sampler, pout.TexCoord6.zw);
    color += weight[1].w * SceneTexture.Sample(Sampler, pout.TexCoord7.zw);
    
    return float4(color.xyz, 1.f);
}