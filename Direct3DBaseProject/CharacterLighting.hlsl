#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout) : SV_Target0
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
    
    //スポットライトとの距離を計算する
    float3 distance = length(pout.WorldPos - LightPosition);
    
    //影響率は距離に比例して小さくなっていく
    float affect = 1.f - 1.f / LightRange * distance;
    
    //影響率がマイナスにならないように補正をかける
    if (affect < 0.f)
    {
        affect = 0.f;
    }
    
    //影響の仕方を指数関数的にする。
    affect = pow(affect, 2.f);
    
    //影響率を乗算して影響を弱める
    diffuseLight *= affect;
    
    //入射光と射出方向の角度を求める
    float angle = dot(lightDirection, LightDirection);
    
    //求めた値をacos()に渡して角度を求める
    angle = abs(acos(angle));
    
    //角度に比例して小さくなっていく影響率を計算する
    affect = 1.f - 1.f / LightAngle * angle;
    
    //影響率がマイナスにならないように補正をかける
    if (affect < 0.f)
    {
        affect = 0.f;
    }
    
    //影響の仕方を指数関数的にする。
    affect = pow(affect, 0.3f);
    
    //角度による影響率を反射光に乗算して、影響を弱める
    diffuseLight *= affect;
    
    //スポットライトの反射光を最終的な反射光に足し算する
    float3 ambientLight = 0.3f;
    float3 finalLight = ambientLight + diffuseLight;
    
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    float4 finalColor = Texture.Sample(Sampler, pout.TexCoord);
    finalColor.xyz *= finalLight;
    
    return finalColor;
}