#include "LightHelper.fx"

//帧常量缓冲
cbuffer cbPerFrame
{
    //光源
    DirectionalLight gDirLights[3];
    //视点
    float3 gEyePosW;
};

//对象常量缓冲
cbuffer cbPerObject
{
    //世界变换
    float4x4 gWorld;
    //世界的逆转置变换,用于法线
    float4x4 gWorldInvTranspose;
    //世界相机投影变换
    float4x4 gWorldViewProj;
    //纹理变换
    float4x4 gTexTransform;
    //材质
    Material gMaterial;
};
//纹理图
Texture2D gDiffuseMap;

//采样状态
SamplerState samAnisotropic
{
    //各向异性过滤
    Filter = ANISOTROPIC;   
    MaxAnisotropy = 4;
    //寻址方式
    AddressU = WRAP;
    AddressV = WRAP;
};
struct VertexIn
{
    //本地坐标
    float3 PosL         : POSITION;
    //本地法线
    float3 NormalL      : NORMAL;
    //纹理坐标
    float2 Tex          : TEXCOORD;
};

struct VertexOut
{
    //世界坐标
    float3 PosW         : POSITION;
    //世界法线
    float3 NormalW      : NORMAL;
    //齐次坐标
    float4 PosH         : SV_POSITION;
    //纹理
    float2 Tex          : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    //本地坐标转为世界坐标
    vOut.PosW = mul(float4(vIn.PosL, 1.0f), gWorld).xyz;
    //本地法线转为世界法线
    vOut.NormalW = mul(vIn.NormalL, (float3x3)gWorldInvTranspose);
    //齐次坐标
    vOut.PosH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj);
    //纹理坐标
    vOut.Tex = mul(float4(vIn.Tex, 0.0f, 1.0f), gTexTransform).xy;
    return vOut;
}

float4 PS(VertexOut vOut):SV_Target
{
    //插值的法线可能没有归一化
    vOut.NormalW = normalize(vOut.NormalW);

    //到视点的向量
    float3 toEye = gEyePosW - vOut.PosW;
    //距离
    float distToEye = length(toEye);
    //归一化
    toEye = normalize(toEye);

    //默认纹理颜色
    float4 texColor = float4(1, 1, 1, 1);
    texColor = gDiffuseMap.Sample(samAnisotropic, vOut.Tex);
    clip(texColor.a - 0.1f);

    //计算光照
    float4 litColor = texColor;
    //初始化光照
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 3; i++)
    {
        float4 A, D, S;
        ComputeDirectionalLight(gMaterial, gDirLights[i], vOut.NormalW, toEye,
            A, D, S);

        ambient += A;
        diffuse += D;
        spec += S;
    }

    litColor = texColor*(ambient + diffuse) + spec;
    litColor.a = gMaterial.Diffuse.a * texColor.a;
    return litColor;
}

technique11 Tech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};