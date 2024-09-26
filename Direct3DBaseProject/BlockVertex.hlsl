#include"Header.hlsli"

BlockPS main(BlockVS vout, uniform bool hasSkin)
{
    BlockPS pout;
    float4 worldPos = mul(vout.instanceTransform, vout.Position);
    pout.WorldPos = worldPos;
    pout.Position = mul(View, worldPos);
    pout.Position = mul(Projection, pout.Position);
    
    pout.Normal = mul(vout.instanceTransform, vout.Normal);
    pout.Normal = normalize(pout.Normal);
    
    pout.TexCoord = vout.TexCoord;
    
    pout.PosInLVP = mul(LVP, worldPos);
    
    pout.textureID = vout.textureID;
    
    return pout;
}