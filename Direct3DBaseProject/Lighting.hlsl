#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
Texture2D<float4> Shadow : register(t10);
Texture2D<float4> CharacterShadow : register(t11);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout):SV_Target0
{
    //ピクセルの座標 - スポットライトの座標を計算
    float3 lightDirection = pout.WorldPos - LightPosition;
    //正規化して大きさ1のベクトルにする
    lightDirection = normalize(lightDirection);
    
    //減衰なしのLambert拡散反射光を計算する
    //ピクセルの法線とライトの方向の内積を計算する
    float t = dot(lightDirection, pout.Normal) * -1.0f;
    //内積の値を0以上の値にする
    t = max(0.f, t);
    //拡散反射光を定義する
    float3 diffuseLight = t;
    
    //減衰なしのPhong鏡面反射光を計算する
    //反射ベクトルを求める
    float3 reflectionVector = reflect(lightDirection, pout.Normal);
    //光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = EyePosition - pout.WorldPos;
    toEye = normalize(toEye);
    //鏡面反射の強さを求める
    t = dot(reflectionVector, toEye);
    //鏡面反射の強さを0以上の数値にする
    t = max(0.f, t);
    //鏡面反射の強さを絞る
    t = pow(t, 5.f);
    //鏡面反射光を定義する
    float3 specularLight = t;
    
    //スポットライトとの距離を計算する
    float3 distance = length(pout.WorldPos - LightPosition);
    
    //影響率は距離に比例して小さくなっていく
    float affect = 1.f - 1.f / LightRange * distance;
    
    //影響率がマイナスにならないように補正をかける
    if(affect<0.f)
    {
        affect = 0.f;
    }
    
    //影響の仕方を指数関数的にする。
    affect = pow(affect, 2.f);
    
    //影響率を乗算して影響を弱める
    diffuseLight *= affect;
    specularLight *= affect;
    
    //入射光と射出方向の角度を求める
    float angle = dot(lightDirection, LightDirection);
    
    //求めた値をacos()に渡して角度を求める
    angle = abs(acos(angle));
    
    //角度に比例して小さくなっていく影響率を計算する
    affect = 1.f - 1.f / LightAngle * angle;
    
    //影響率がマイナスにならないように補正をかける
    if(affect<0.f)
    {
        affect = 0.f;
    }
    
    //影響の仕方を指数関数的にする。
    affect = pow(affect, 0.3f);
    
    //角度による影響率を反射光に乗算して、影響を弱める
    diffuseLight *= affect;
    specularLight *= affect;
    
    //スポットライトの反射光を最終的な反射光に足し算する
    float3 ambientLight = 0.3f;
    float3 finalLight = ambientLight + diffuseLight + specularLight;
    
    float2 shadowMapUV = pout.PosInLVP.xy / pout.PosInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float zInLVP = pout.PosInLVP.z / pout.PosInLVP.w;
    
    //UV座標を使ってシャドウマップから影情報をサンプリング
    if (shadowMapUV.x > 0.f && shadowMapUV.x < 1.f && shadowMapUV.y > 0.f && shadowMapUV.y < 1.f)
    {
        float zInShadowMap = Shadow.Sample(Sampler, shadowMapUV).r;
        float zInCharacterShadowMap = CharacterShadow.Sample(Sampler, shadowMapUV).r;
        if (zInLVP > zInShadowMap + 0.00001f || zInLVP > zInCharacterShadowMap + 0.00001f)
        {
            finalLight = ambientLight;
        }
    }
    
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    float4 finalColor = Texture.Sample(Sampler, pout.TexCoord);
    finalColor.xyz *= finalLight;
    
    return finalColor;
}