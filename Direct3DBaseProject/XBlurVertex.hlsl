#include"Header.hlsli"

Texture2D<float4> SceneTexture : register(t0);

BlurPS main( PPVS vin )
{
    BlurPS pout;
    
    pout.Position = float4(vin.Position, 1);
    
    //テクスチャサイズを取得
    float2 textureSize;
    float level;
    SceneTexture.GetDimensions(0, textureSize.x, textureSize.y, level);
    
    //基準テクセルのUV座標を記録
    float2 texCoord = vin.TexCoord;
    
    //基準テクセルからU座標をずらすためのオフセット計算
    pout.TexCoord0.xy = float2(1.f / textureSize.x, 0.f);
    pout.TexCoord1.xy = float2(3.f / textureSize.x, 0.f);
    pout.TexCoord2.xy = float2(5.f / textureSize.x, 0.f);
    pout.TexCoord3.xy = float2(7.f / textureSize.x, 0.f);
    pout.TexCoord4.xy = float2(9.f / textureSize.x, 0.f);
    pout.TexCoord5.xy = float2(11.f / textureSize.x, 0.f);
    pout.TexCoord6.xy = float2(13.f / textureSize.x, 0.f);
    pout.TexCoord7.xy = float2(15.f / textureSize.x, 0.f);
    
    //オフセットに-1をかけてマイナス方向のオフセットも計算
    pout.TexCoord0.zw = pout.TexCoord0.xy * -1.f;
    pout.TexCoord1.zw = pout.TexCoord1.xy * -1.f;
    pout.TexCoord2.zw = pout.TexCoord2.xy * -1.f;
    pout.TexCoord3.zw = pout.TexCoord3.xy * -1.f;
    pout.TexCoord4.zw = pout.TexCoord4.xy * -1.f;
    pout.TexCoord5.zw = pout.TexCoord5.xy * -1.f;
    pout.TexCoord6.zw = pout.TexCoord6.xy * -1.f;
    pout.TexCoord7.zw = pout.TexCoord7.xy * -1.f;
    
    //実際にサンプリングするUV座標に変換
    pout.TexCoord0 += float4(texCoord, texCoord);
    pout.TexCoord1 += float4(texCoord, texCoord);
    pout.TexCoord2 += float4(texCoord, texCoord);
    pout.TexCoord3 += float4(texCoord, texCoord);
    pout.TexCoord4 += float4(texCoord, texCoord);
    pout.TexCoord5 += float4(texCoord, texCoord);
    pout.TexCoord6 += float4(texCoord, texCoord);
    pout.TexCoord7 += float4(texCoord, texCoord);
    
    return pout;
}