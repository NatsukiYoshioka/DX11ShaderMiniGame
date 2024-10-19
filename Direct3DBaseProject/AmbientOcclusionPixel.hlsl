#include"Header.hlsli"
#define KERNEL_SIZE 64

cbuffer AOConstats : register(b4)
{
    float4x4 projection;
    float4x4 invProj;
    float4 sampleKernel[KERNEL_SIZE];
    float radius;
    float ZFar;
    float AOPower;
}

Texture2D<float4> Normal : register(t0);
SamplerState Sampler : register(s0);

float4 main(AOPS pout):SV_Target0
{
    float4 Out;
    
    //�@���}�b�v+�[�x�}�b�v�̎擾
    float4 NormalDepthMap = Normal.Sample(Sampler, pout.TexCoord);
    
    //�@���x�N�g���̍쐬
    float3 normal;
    normal.xy = NormalDepthMap.xy;
    normal.z = -sqrt(max(1 - normal.x * normal.x - normal.y * normal.y, 0));
    normal = normalize(normal);
    
    //�`��s�N�Z���̃e�N�Z�����W����N���b�v��ԏ�̍��W���v�Z
    float4 ClipPos;
    ClipPos.xy = (pout.TexCoord * float2(2.f, -2.f) + float2(-1.f, 1.f)) * NormalDepthMap.w;
    ClipPos.zw = NormalDepthMap.zw;
    
    //�J������ԏ�ł̍��W�v�Z
    float4 ViewPos = mul(ClipPos, invProj);
        
    float normAO = 0;
    float depthAO = 0;
    for (int i = 0; i < KERNEL_SIZE;i++)
    {
        //���C�̕����x�N�g��
        float3 ray = sampleKernel[i].xyz * radius;
        
        //���C�̕����x�N�g����`��s�N�Z���̖@�����̔������Ɏ��܂�悤�ɕ�����ϊ�����
        ray = sign(dot(normal, ray)) * ray;
        
        //���̓s�N�Z���̍��W
        float4 envPos;
        //���C�̕����Ɉړ�
        envPos.xyz = ViewPos.xyz + ray;
        //�N���b�v��ԏ�ɍs��ϊ�
        envPos = mul(float4(envPos.xyz, 1), projection);
        //�X�N���[����ԏ�ɕϊ�
        envPos.xy = envPos.xy / envPos.w * float2(0.5f, -0.5f) + 0.5f;
        
        //���̓s�N�Z���̃T���v�����O
        float4 envNormalMap = Normal.Sample(Sampler, envPos.xy);
        
        //�@���x�N�g���̍쐬
        float3 envNormal;
        envNormal.xy = envNormalMap.xy;
        envNormal.z = -sqrt(max(1 - envNormal.x * envNormal.x - envNormal.y * envNormal.y, 0));
        envNormal = normalize(envNormal);
        
        //���ς̊p�x���傫���Ȃ�قǊ����Օ��W�����傫���Ȃ�悤�ɂ���
        float n = dot(normal, envNormal) * 0.5f + 0.5f;
        //�G�b�W���ʂɂȂ��Ă��镔���͎Օ�����Ȃ��悤�ɂ���
        n += step(NormalDepthMap.z, envNormalMap.z);
        normAO += min(n, 1);
        
        //�[�x�l�̋������傫���قǊ����Օ��̉e���͂��������Ȃ�悤�ɂ���
        depthAO += abs(NormalDepthMap.z - envNormalMap.z) / ZFar;
    }
    
    Out = normAO / (float) KERNEL_SIZE + depthAO;
    Out = pow(abs(Out), AOPower);
    
    return Out;
}