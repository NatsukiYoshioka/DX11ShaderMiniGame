#include"Header.hlsli"

PSOutput main(VSOutput vout,uniform bool hasSkin)
{
    PSOutput pout;
    float4 worldPos = mul(World, vout.Position);
    pout.Position = mul(View, worldPos);
    pout.Position = mul(Projection, pout.Position);
    pout.WorldPos = vout.Position;
    
    pout.Normal = mul(World, vout.Normal);
    
    pout.TexCoord = vout.TexCoord;
    
    pout.NormalInView = mul(View, pout.Normal);
    
    pout.Tangent = normalize(mul(World, vout.Tangent));
    pout.BiNormal = normalize(mul(World, vout.BiNormal));

    return pout;
}