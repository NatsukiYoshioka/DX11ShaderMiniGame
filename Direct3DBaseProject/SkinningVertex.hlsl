#include"Header.hlsli"

struct VSSkinningOutput
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float4 Position : SV_POSITION;
    uint4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

PSOutput main(VSSkinningOutput vin)
{
    PSOutput pout;
    
    float4x3 skinning = 0;

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        skinning += Bones[vin.Indices[i]] * vin.Weights[i];
    }

    vin.Position.xyz = mul(vin.Position, skinning);
    
    float4 worldPos = mul(World, vin.Position);
    pout.WorldPos = worldPos;
    pout.Position = mul(View, worldPos);
    pout.Position = mul(Projection, pout.Position);
    
    pout.Normal = mul(World, vin.Normal);
    pout.Normal = normalize(pout.Normal);
    
    pout.TexCoord = vin.TexCoord;
        
    return pout;
}