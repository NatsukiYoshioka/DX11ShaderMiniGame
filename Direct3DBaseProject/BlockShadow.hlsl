#include"Header.hlsli"

float4 main(BlockPS pout) : SV_TARGET0
{
    return float4(pout.Position.z, pout.Position.z, pout.Position.z, 1.0f);
}