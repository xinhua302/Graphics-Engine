
//方向光源
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;	//w = 高光因子
	float4 Reflect;
};

void ComputeDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)

{
	//初始化
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//环境光
	ambient = mat.Ambient * light.Ambient;

	//光的反向量
	float3 antiLightVec = -light.Direction;
	//光与法线的夹角
	float diffuseFactor = dot(antiLightVec, normal);
	if (diffuseFactor > 0)
	{
		//散射光
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;

		//镜面光
		float3 vReflect = reflect(-antiLightVec, normal);
		float specFactor = pow(max(dot(vReflect, toEye), 0.0f), mat.Specular.w);
		spec = specFactor * mat.Specular * light.Specular;
	}
	
}