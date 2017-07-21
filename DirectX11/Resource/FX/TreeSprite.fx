#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;
	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};

cbuffer cbPerObject
{
	float4x4 gViewProj;
	Material gMaterial;
};

cbuffer cbFixed
{
	float2 gTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};

Texture2DArray gTreeMapArray;

SamplerState samLinear
{
	Filter   = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
};

struct VertexOut
{
	float3 CenterW : POSITION;
	float2 SizeW   : SIZE;
};

struct GeoOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
    uint   PrimID  : SV_PrimitiveID;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vOut;
    vOut.CenterW = vin.PosW;
    vOut.SizeW = vin.SizeW;

    return vOut;
}
 
[maxvertexcount(4)]
void GS(point VertexOut gin[1], 
        uint primID : SV_PrimitiveID, 
        inout TriangleStream<GeoOut> triStream)
{	
    //上向量
    float3 up = float3(0.0f, 1.0f, 0.0f);
    //注视向量
    float3 look = gEyePosW - gin[0].CenterW;
    look.y = 0.0f;
    look = normalize(look);
    //右向量
    float3 right = cross(up, look);

    float halfWidth = 0.5f * gin[0].SizeW.x;
    float halfHeight = 0.5f * gin[0].SizeW.y;

    GeoOut geoOut[4];
    geoOut[0].PosW = gin[0].CenterW + right * halfWidth - up * halfHeight;
    geoOut[1].PosW = gin[0].CenterW + right * halfWidth + up * halfHeight;
    geoOut[2].PosW = gin[0].CenterW - right * halfWidth - up * halfHeight;
    geoOut[3].PosW = gin[0].CenterW - right * halfWidth + up * halfHeight;
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        geoOut[i].PosH = mul(float4(geoOut[i].PosW, 1.0f), gViewProj);
        geoOut[i].NormalW = look;
        geoOut[i].Tex = gTexC[i];
        geoOut[i].PrimID = primID;
        triStream.Append(geoOut[i]);
    }
}

float4 PS(GeoOut pin) : SV_Target
{
    pin.NormalW = normalize(pin.NormalW);

    float3 toEye = gEyePosW - pin.PosW;
    //距离
    float distToEye = length(toEye);
    //归一化
    toEye = normalize(toEye);

    //默认纹理颜色
    float4 texColor = float4(1, 1, 1, 1);
    texColor = gTreeMapArray.Sample(samLinear, float3(pin.Tex, pin.PrimID % 4));
    clip(texColor.a - 0.05f);

    //计算光照
    float4 litColor = texColor;
    //初始化光照
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        float4 A, D, S;
        ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
            A, D, S);

        ambient += A;
        diffuse += D;
        spec += S;
    }

    litColor = texColor*(ambient + diffuse) + spec;

    //雾效
    float fogLerp = saturate((distToEye - gFogStart) / gFogRange);
    litColor = lerp(litColor, gFogColor, fogLerp);

    //Alpha
    litColor.a = gMaterial.Diffuse.a * texColor.a;

    return litColor;
}
            
technique11 Tech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader( gs_5_0, GS() ) );
        SetPixelShader( CompileShader( ps_5_0, PS() ));
    }
}
