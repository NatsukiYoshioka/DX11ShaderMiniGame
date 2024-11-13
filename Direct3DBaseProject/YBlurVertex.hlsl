#include"Header.hlsli"

Texture2D<float4> SceneTexture : register(t0);

BlurPS main(PPVS vin)
{
    BlurPS pout;
    
    pout.Position = float4(vin.Position, 1);
    
    //�e�N�X�`���T�C�Y���擾
    float2 textureSize;
    float level;
    SceneTexture.GetDimensions(0, textureSize.x, textureSize.y, level);
    
    //��e�N�Z����UV���W���L�^
    float2 texCoord = vin.TexCoord;
    
    //��e�N�Z������V���W�����炷���߂̃I�t�Z�b�g�v�Z
    pout.TexCoord0.xy = float2(0.f, 1.f / textureSize.y);
    pout.TexCoord1.xy = float2(0.f, 3.f / textureSize.y);
    pout.TexCoord2.xy = float2(0.f, 5.f / textureSize.y);
    pout.TexCoord3.xy = float2(0.f, 7.f / textureSize.y);
    pout.TexCoord4.xy = float2(0.f, 9.f / textureSize.y);
    pout.TexCoord5.xy = float2(0.f, 11.f / textureSize.y);
    pout.TexCoord6.xy = float2(0.f, 13.f / textureSize.y);
    pout.TexCoord7.xy = float2(0.f, 15.f / textureSize.y);
    
    //�I�t�Z�b�g��-1�������ă}�C�i�X�����̃I�t�Z�b�g���v�Z
    pout.TexCoord0.zw = pout.TexCoord0.xy * -1.f;
    pout.TexCoord1.zw = pout.TexCoord1.xy * -1.f;
    pout.TexCoord2.zw = pout.TexCoord2.xy * -1.f;
    pout.TexCoord3.zw = pout.TexCoord3.xy * -1.f;
    pout.TexCoord4.zw = pout.TexCoord4.xy * -1.f;
    pout.TexCoord5.zw = pout.TexCoord5.xy * -1.f;
    pout.TexCoord6.zw = pout.TexCoord6.xy * -1.f;
    pout.TexCoord7.zw = pout.TexCoord7.xy * -1.f;
    
    //���ۂɃT���v�����O����UV���W�ɕϊ�
    pout.TexCoord0 += float4(texCoord, texCoord);
    pout.TexCoord1 += float4(texCoord, texCoord);
    pout.TexCoord2 += float4(texCoord, texCoord);
    pout.TexCoord3 += float4(texCoord, texCoord);
    pout.TexCoord4 += float4(texCoord, texCoord);
    pout.TexCoord5 += float4(texCoord, texCoord);
    pout.TexCoord6 += float4(texCoord, texCoord);
    pout.TexCoord7 += float4(texCoord, texCoord);
    
    return pout;
}