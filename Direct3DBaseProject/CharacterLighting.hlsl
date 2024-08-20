#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout) : SV_Target0
{
    //�s�N�Z���̍��W - �X�|�b�g���C�g�̍��W���v�Z
    float3 lightDirection = pout.WorldPos - LightPosition;
    //���K�����đ傫��1�̃x�N�g���ɂ���
    lightDirection = normalize(lightDirection);
    
    //�����Ȃ���Lambert�g�U���ˌ����v�Z����
    //�s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
    float t = dot(lightDirection, pout.Normal) * -1.0f;
    //���ς̒l��0�ȏ�̒l�ɂ���
    t = max(0.f, t);
    //�g�U���ˌ����`����
    float3 diffuseLight = t;
    
    //�X�|�b�g���C�g�Ƃ̋������v�Z����
    float3 distance = length(pout.WorldPos - LightPosition);
    
    //�e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
    float affect = 1.f - 1.f / LightRange * distance;
    
    //�e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
    if (affect < 0.f)
    {
        affect = 0.f;
    }
    
    //�e���̎d�����w���֐��I�ɂ���B
    affect = pow(affect, 2.f);
    
    //�e��������Z���ĉe������߂�
    diffuseLight *= affect;
    
    //���ˌ��Ǝˏo�����̊p�x�����߂�
    float angle = dot(lightDirection, LightDirection);
    
    //���߂��l��acos()�ɓn���Ċp�x�����߂�
    angle = abs(acos(angle));
    
    //�p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
    affect = 1.f - 1.f / LightAngle * angle;
    
    //�e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
    if (affect < 0.f)
    {
        affect = 0.f;
    }
    
    //�e���̎d�����w���֐��I�ɂ���B
    affect = pow(affect, 0.3f);
    
    //�p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
    diffuseLight *= affect;
    
    //�X�|�b�g���C�g�̔��ˌ����ŏI�I�Ȕ��ˌ��ɑ����Z����
    float3 ambientLight = 0.3f;
    float3 finalLight = ambientLight + diffuseLight;
    
    //�e�N�X�`���J���[�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
    float4 finalColor = Texture.Sample(Sampler, pout.TexCoord);
    finalColor.xyz *= finalLight;
    
    return finalColor;
}