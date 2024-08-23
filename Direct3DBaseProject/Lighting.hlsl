#include"Header.hlsli"

Texture2D<float4> Texture : register(t0);
Texture2D<float4> Normal : register(t1);
Texture2D<float4> AO : register(t2);
Texture2D<float4> Shadow : register(t10);
Texture2D<float4> CharacterShadow : register(t11);
SamplerState Sampler : register(s0);

float4 main(PSOutput pout):SV_Target0
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
    
    //�����Ȃ���Phong���ʔ��ˌ����v�Z����
    //���˃x�N�g�������߂�
    float3 reflectionVector = reflect(lightDirection, pout.Normal);
    //�������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = EyePosition - pout.WorldPos;
    toEye = normalize(toEye);
    //���ʔ��˂̋��������߂�
    t = dot(reflectionVector, toEye);
    //���ʔ��˂̋�����0�ȏ�̐��l�ɂ���
    t = max(0.f, t);
    //���ʔ��˂̋������i��
    t = pow(t, 5.f);
    //���ʔ��ˌ����`����
    float3 specularLight = t;
    
    //�X�|�b�g���C�g�Ƃ̋������v�Z����
    float3 distance = length(pout.WorldPos - LightPosition);
    
    //�e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
    float affect = 1.f - 1.f / LightRange * distance;
    
    //�e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
    if(affect<0.f)
    {
        affect = 0.f;
    }
    
    //�e���̎d�����w���֐��I�ɂ���B
    affect = pow(affect, 2.f);
    
    //�e��������Z���ĉe������߂�
    diffuseLight *= affect;
    specularLight *= affect;
    
    //���ˌ��Ǝˏo�����̊p�x�����߂�
    float angle = dot(lightDirection, LightDirection);
    
    //���߂��l��acos()�ɓn���Ċp�x�����߂�
    angle = abs(acos(angle));
    
    //�p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
    affect = 1.f - 1.f / LightAngle * angle;
    
    //�e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
    if(affect<0.f)
    {
        affect = 0.f;
    }
    
    //�e���̎d�����w���֐��I�ɂ���B
    affect = pow(affect, 0.3f);
    
    //�p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
    diffuseLight *= affect;
    specularLight *= affect;
    
    //�X�|�b�g���C�g�̔��ˌ����ŏI�I�Ȕ��ˌ��ɑ����Z����
    float3 ambientLight = 0.3f;
    float3 finalLight = ambientLight + diffuseLight + specularLight;
    
    float2 shadowMapUV = pout.PosInLVP.xy / pout.PosInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float zInLVP = pout.PosInLVP.z / pout.PosInLVP.w;
    
    //UV���W���g���ăV���h�E�}�b�v����e�����T���v�����O
    if (shadowMapUV.x > 0.f && shadowMapUV.x < 1.f && shadowMapUV.y > 0.f && shadowMapUV.y < 1.f)
    {
        float zInShadowMap = Shadow.Sample(Sampler, shadowMapUV).r;
        float zInCharacterShadowMap = CharacterShadow.Sample(Sampler, shadowMapUV).r;
        if (zInLVP > zInShadowMap + 0.00001f || zInLVP > zInCharacterShadowMap + 0.00001f)
        {
            finalLight = ambientLight;
        }
    }
    
    //�e�N�X�`���J���[�ɋ��߂�������Z���čŏI�o�̓J���[�����߂�
    float4 finalColor = Texture.Sample(Sampler, pout.TexCoord);
    finalColor.xyz *= finalLight;
    
    return finalColor;
}