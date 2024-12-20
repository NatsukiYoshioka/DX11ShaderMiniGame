#include"Header.hlsli"

Texture2D<float4> Texture1 : register(t0);
Texture2D<float4> Texture2 : register(t1);
Texture2D<float4> Texture3 : register(t2);
Texture2D<float4> Texture4 : register(t3);
Texture2D<float4> playerDepth : register(t4);
Texture2D<float4> Shadow : register(t10);
SamplerState Sampler : register(s0);

struct PSOut
{
    float4 BackBuffer : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Color : SV_Target2;
};

PSOut main(BlockPS pout)
{
    PSOut Out;
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
    if (affect < 0.f)
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
    if (affect < 0.f)
    {
        affect = 0.f;
    }
    
    //影響の仕方を指数関数的にする。
    affect = pow(affect, 0.3f);
    
    //角度による影響率を反射光に乗算して、影響を弱める
    diffuseLight *= affect;
    specularLight *= affect;
    
    //スポットライトの反射光を最終的な反射光に足し算する
    float3 ambientLight = 0.5f;
    float3 finalLight = ambientLight + diffuseLight + specularLight;
    
    float2 shadowMapUV = pout.PosInLVP.xy / pout.PosInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float zInLVP = pout.PosInLVP.z / pout.PosInLVP.w;
    
    //UV座標を使ってシャドウマップから影情報をサンプリング
    if (shadowMapUV.x > 0.f && shadowMapUV.x < 1.f && shadowMapUV.y > 0.f && shadowMapUV.y < 1.f)
    {
        float zInShadowMap = Shadow.Sample(Sampler, shadowMapUV).r;
        if (zInLVP > zInShadowMap + 0.00001f)
        {
            finalLight = ambientLight;
        }
    }
    
    finalLight *= LightColor;
    
    float4 finalColor;
    if (pout.textureID == 0)
    {
        finalColor = Texture1.Sample(Sampler, pout.TexCoord);
    }
    else if (pout.textureID == 1)
    {
        finalColor = Texture2.Sample(Sampler, pout.TexCoord);
    }
    else if (pout.textureID == 2)
    {
        finalColor = Texture3.Sample(Sampler, pout.TexCoord);
    }
    else
    {
        finalColor = Texture4.Sample(Sampler, pout.TexCoord);
    }
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= finalLight;
    
    //ディザリング処理
    float2 depthUV = float2(pout.Position.x / ScreenSize.x, pout.Position.y / ScreenSize.y);
    float depth = pout.Position.z;
    if (depthUV.x > 0.f && depthUV.x < 1.f && depthUV.y > 0.f && depthUV.y < 1.f)
    {
        //プレイヤーの深度値をサンプリングしてブロックの深度値の方が低ければピクセルキルする
        float zInPlayerDepth = playerDepth.Sample(Sampler, depthUV).r;
        if (depth < zInPlayerDepth && zInPlayerDepth != 1.f)
        {
            int x = (int) fmod(pout.Position.x, 4.f);
            int y = (int) fmod(pout.Position.y, 4.f);
            int dither = pattern[y][x];
            
            clip(dither - 30.f);
            //finalColor.xyz = float3(1.f, 1.f, 1.f);
        }
    }
    
    Out.BackBuffer = finalColor;
    //法線ベクトル計算要素の出力
    Out.Normal.xy = normalize(mul(float4(pout.Normal, 0), mul(World, View)).xyz).xy;
    //深度値を出力
    Out.Normal.zw = pout.Position.zw;
    
    //色素出力
    Out.Color = Out.BackBuffer;
        
    return Out;
}