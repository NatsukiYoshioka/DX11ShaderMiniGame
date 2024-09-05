struct SphereBuffer
{
    float3 sphereCenter;
    float sphereRadius;
};

StructuredBuffer<float3> vertices : register(t0);
RWStructuredBuffer<SphereBuffer> outputVertices : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float3 vertex = vertices[DTid.x];
    float3 direction = outputVertices[0].sphereCenter - vertex;
    float distance = length(direction) - 0.25f;

    if (distance < outputVertices[0].sphereRadius)
    {
        float3 pushDirection = normalize(direction);
        float pushDistance = outputVertices[0].sphereRadius - distance;
        outputVertices[0].sphereCenter.xz += pushDirection.xz * pushDistance;
    }
}