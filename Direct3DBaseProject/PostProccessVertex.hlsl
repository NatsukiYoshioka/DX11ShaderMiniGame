#include"Header.hlsli"

PPPS main(PPVS vin)
{
    PPPS pout;
    
    pout.Position = float4(vin.Position, 1);
    pout.TexCoord = vin.TexCoord;
    
    return pout;
}