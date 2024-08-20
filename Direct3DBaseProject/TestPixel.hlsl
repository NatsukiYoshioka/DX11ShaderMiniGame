#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout) : SV_TARGET0
{
    float3 dirDirection = LightDirection;
    float3 color = float3(0.5f, 0.5f, 0.5f);
    //dirDirection = normalize(dirDirection);
    
    float t = dot(dirDirection, pout.Normal);
    
    t *= -1.f;
    
    if (t < 0.f)
    {
        t = 0.f;
    }
    
    float diffuseLight = color * t;
    
    //���˃x�N�g�������߂�
    float3 reflectionVector = reflect(dirDirection, pout.Normal);
    
    //�������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = EyePosition - pout.WorldPos;
    
    //���K������
    toEye = normalize(toEye);
    
    //���ʔ��˂̋��������߂�
    //dot�֐��𗘗p����reflectionVector��toEye�̓��ς����߂�
    t = dot(reflectionVector, toEye);
    
    //���ς̌��ʂ̓}�C�i�X�ɂȂ�̂ŁA�}�C�i�X�̏ꍇ��0�ɂ���
    if (t < 0.f)
    {
        t = 0.f;
    }
    
    //���ʔ��˂̋������i��
    t = pow(t, 5.f);
    
    //���ʔ��ˌ������߂�
    float3 specularLight = t;
    
    //�g�U���ˌ��Ƌ��ʔ��ˌ��𑫂��Z���āA�ŏI�I�Ȍ������߂�
    float3 light = diffuseLight + specularLight;
    
    float3 AmbientLight = 0.3f;
    
    //���C�g�̂��ׂĂ̗v�f���ꗥ�Œ�グ����
    light.x += AmbientLight.x;
    light.y += AmbientLight.y;
    light.z += AmbientLight.z;
    
    float4 finalColor = Texture.Sample(Sampler, pout.TexCoord);
    
    //�e�N�X�`���J���[�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
    finalColor.xyz *= light;
    
    return finalColor;
}