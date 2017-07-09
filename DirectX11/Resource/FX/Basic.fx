#include "LightHelper.fx"

//֡��������
cbuffer cbPerFrame
{
    //��Դ
    DirectionalLight gDirLights[3];
    //�ӵ�
    float3 gEyePosW;
};

//����������
cbuffer cbPerObject
{
    //����任
    float4x4 gWorld;
    //�������ת�ñ任,���ڷ���
    float4x4 gWorldInvTranspose;
    //�������ͶӰ�任
    float4x4 gWorldViewProj;
    //����任
    float4x4 gTexTransform;
    //����
    Material gMaterial;
};
//����ͼ
Texture2D gDiffuseMap;

//����״̬
SamplerState samAnisotropic
{
    //�������Թ���
    Filter = ANISOTROPIC;   
    MaxAnisotropy = 4;
    //Ѱַ��ʽ
    AddressU = WRAP;
    AddressV = WRAP;
};
struct VertexIn
{
    //��������
    float3 PosL         : POSITION;
    //���ط���
    float3 NormalL      : NORMAL;
    //��������
    float2 Tex          : TEXCOORD;
};

struct VertexOut
{
    //��������
    float3 PosW         : POSITION;
    //���編��
    float3 NormalW      : NORMAL;
    //�������
    float4 PosH         : SV_POSITION;
    //����
    float2 Tex          : TEXCOORD;
};

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    //��������תΪ��������
    vOut.PosW = mul(float4(vIn.PosL, 1.0f), gWorld).xyz;
    //���ط���תΪ���編��
    vOut.NormalW = mul(vIn.NormalL, (float3x3)gWorldInvTranspose);
    //�������
    vOut.PosH = mul(float4(vIn.PosL, 1.0f), gWorldViewProj);
    //��������
    vOut.Tex = mul(float4(vIn.Tex, 0.0f, 1.0f), gTexTransform).xy;
    return vOut;
}

float4 PS(VertexOut vOut):SV_Target
{
    //��ֵ�ķ��߿���û�й�һ��
    vOut.NormalW = normalize(vOut.NormalW);

    //���ӵ������
    float3 toEye = gEyePosW - vOut.PosW;
    //����
    float distToEye = length(toEye);
    //��һ��
    toEye = normalize(toEye);

    //Ĭ��������ɫ
    float4 texColor = float4(1, 1, 1, 1);
    texColor = gDiffuseMap.Sample(samAnisotropic, vOut.Tex);
    clip(texColor.a - 0.1f);

    //�������
    float4 litColor = texColor;
    //��ʼ������
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