#include"Header.hlsli"

AOPS main(AOVS vin)
{
    AOPS pout;
    
    pout.Position = float4(vin.Position, 1);
    pout.TexCoord = vin.TexCoord;
    
    return pout;
}