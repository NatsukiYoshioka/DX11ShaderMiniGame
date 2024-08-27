struct PixInfo
{
    int playerPixNum;
    int visiblePixNum;
};

Texture2D<float4> playerPix : register(t0);
Texture2D<float4> hitCheckPix : register(t1);
SamplerState Sampler : register(s0);

RWStructuredBuffer<PixInfo> BufferOut : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    float4 playerColor = playerPix[DTid.xy];
    float4 hitCheckColor = hitCheckPix[DTid.xy];
    
    if (playerColor.b > 0.4f)
    {
        BufferOut[0].playerPixNum++;
    }
    if (hitCheckColor.b > 0.4f)
    {
        BufferOut[0].visiblePixNum++;
    }
}