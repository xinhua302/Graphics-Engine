
//�����Դ
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
	float4 Specular;	//w = �߹�����
	float4 Reflect;
};

void ComputeDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)

{
	//��ʼ��
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//������
	ambient = mat.Ambient * light.Ambient;

	//��ķ�����
	float3 antiLightVec = -light.Direction;
	//���뷨�ߵļн�
	float diffuseFactor = dot(antiLightVec, normal);
	if (diffuseFactor > 0)
	{
		//ɢ���
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;

		//�����
		float3 vReflect = reflect(-antiLightVec, normal);
		float specFactor = pow(max(dot(vReflect, toEye), 0.0f), mat.Specular.w);
		spec = specFactor * mat.Specular * light.Specular;
	}
	
}