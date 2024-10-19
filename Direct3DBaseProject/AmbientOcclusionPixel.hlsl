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
    
    //法線マップ+深度マップの取得
    float4 NormalDepthMap = Normal.Sample(Sampler, pout.TexCoord);
    
    //法線ベクトルの作成
    float3 normal;
    normal.xy = NormalDepthMap.xy;
    normal.z = -sqrt(max(1 - normal.x * normal.x - normal.y * normal.y, 0));
    normal = normalize(normal);
    
    //描画ピクセルのテクセル座標からクリップ空間上の座標を計算
    float4 ClipPos;
    ClipPos.xy = (pout.TexCoord * float2(2.f, -2.f) + float2(-1.f, 1.f)) * NormalDepthMap.w;
    ClipPos.zw = NormalDepthMap.zw;
    
    //カメラ空間上での座標計算
    float4 ViewPos = mul(ClipPos, invProj);
        
    float normAO = 0;
    float depthAO = 0;
    for (int i = 0; i < KERNEL_SIZE;i++)
    {
        //レイの方向ベクトル
        float3 ray = sampleKernel[i].xyz * radius;
        
        //レイの方向ベクトルを描画ピクセルの法線内の半球内に収まるように方向を変換する
        ray = sign(dot(normal, ray)) * ray;
        
        //周囲ピクセルの座標
        float4 envPos;
        //レイの方向に移動
        envPos.xyz = ViewPos.xyz + ray;
        //クリップ空間上に行列変換
        envPos = mul(float4(envPos.xyz, 1), projection);
        //スクリーン空間上に変換
        envPos.xy = envPos.xy / envPos.w * float2(0.5f, -0.5f) + 0.5f;
        
        //周囲ピクセルのサンプリング
        float4 envNormalMap = Normal.Sample(Sampler, envPos.xy);
        
        //法線ベクトルの作成
        float3 envNormal;
        envNormal.xy = envNormalMap.xy;
        envNormal.z = -sqrt(max(1 - envNormal.x * envNormal.x - envNormal.y * envNormal.y, 0));
        envNormal = normalize(envNormal);
        
        //内積の角度が大きくなるほど環境光遮蔽係数が大きくなるようにする
        float n = dot(normal, envNormal) * 0.5f + 0.5f;
        //エッジが凸になっている部分は遮蔽されないようにする
        n += step(NormalDepthMap.z, envNormalMap.z);
        normAO += min(n, 1);
        
        //深度値の距離が大きいほど環境光遮蔽の影響力が小さくなるようにする
        depthAO += abs(NormalDepthMap.z - envNormalMap.z) / ZFar;
    }
    
    Out = normAO / (float) KERNEL_SIZE + depthAO;
    Out = pow(abs(Out), AOPower);
    
    return Out;
}