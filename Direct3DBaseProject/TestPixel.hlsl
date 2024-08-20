#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout) : SV_TARGET0
{
    float3 dirDirection = LightDirection;
    float3 color = float3(0.5f, 0.5f, 0.5f);
    //dirDirection = normalize(dirDirection);
    
    float t = dot(dirDirection, pout.Normal);
    
    t *= -1.f;
    
    if (t < 0.f)
    {
        t = 0.f;
    }
    
    float diffuseLight = color * t;
    
    //反射ベクトルを求める
    float3 reflectionVector = reflect(dirDirection, pout.Normal);
    
    //光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = EyePosition - pout.WorldPos;
    
    //正規化する
    toEye = normalize(toEye);
    
    //鏡面反射の強さを求める
    //dot関数を利用してreflectionVectorとtoEyeの内積を求める
    t = dot(reflectionVector, toEye);
    
    //内積の結果はマイナスになるので、マイナスの場合は0にする
    if (t < 0.f)
    {
        t = 0.f;
    }
    
    //鏡面反射の強さを絞る
    t = pow(t, 5.f);
    
    //鏡面反射光を求める
    float3 specularLight = t;
    
    //拡散反射光と鏡面反射光を足し算して、最終的な光を求める
    float3 light = diffuseLight + specularLight;
    
    float3 AmbientLight = 0.3f;
    
    //ライトのすべての要素を一律で底上げする
    light.x += AmbientLight.x;
    light.y += AmbientLight.y;
    light.z += AmbientLight.z;
    
    float4 finalColor = Texture.Sample(Sampler, pout.TexCoord);
    
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= light;
    
    return finalColor;
}