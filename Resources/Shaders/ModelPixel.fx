#include "AddLight.fx"

float camNear;			//ī�޶� �ٰŸ� ���
float camFar;			//ī�޶� ���Ÿ� ���

float gTimes;

float4		vColor1=float4(1,0,0,1);			//��� �÷�
float4		vColor2=float4(0,1,0,1);			//��� �÷�
float4		vColor3=float4(0,0,1,1);			//��� �÷�

texture Diffuse_Tex;
sampler2D Diffuse = sampler_state
{
	Texture = (Diffuse_Tex);
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture Normal_Tex;
sampler2D Normal = sampler_state
{
	Texture = (Normal_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

texture Specular_Tex;
sampler2D Specular = sampler_state
{
	Texture = (Specular_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


texture Emission_Tex;
sampler2D Emission = sampler_state
{
	Texture = (Emission_Tex);
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
//
// Texture �� �޴´�.
//
texture Color_Tex;

//
// MyTex �� SamplingFilter �� ����
//
sampler2D ColorTexSamp = sampler_state
{
	Texture = Color_Tex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//
// Texture �� �޴´�.
//
texture Mask_Tex;

//
// MyTex �� SamplingFilter �� ����
//
sampler2D MyMaskSamp = sampler_state
{
	Texture = Mask_Tex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//
// Texture �� �޴´�.
//
texture TransitionTex;

//
// TransitionTex �� SamplingFilter �� ����
//
sampler2D TransitionTexSamp = sampler_state
{
	Texture = TransitionTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//
// Texture �� �޴´�.
//
texture MultiTex;

//
// MyTex �� SamplingFilter �� ����
//
sampler2D MultiSamp = sampler_state
{
	Texture = MultiTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


texture base_Tex;
sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture tex_water_nm_Tex;
sampler2D Texture1 = sampler_state
{
   Texture = (tex_water_nm_Tex);
   MAGFILTER = LINEAR;
};



float4x4 baseDirectionLight;			//�⺻������ ���


struct PS_INPUT
{
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;
	float4 FinalPos : TEXCOORD6;
};

//�ȼ����̴� ��� ����ü
struct PS_OUTPUT
{
	float4 baseColor : COLOR0;			//0�� �������� �÷�
	float4 normalDepth : COLOR1;		//1�� �������� �÷� ( RGB �븻, A ���� )
};

//---------------------------------------------------------------
// Render Base ����
//---------------------------------------------------------------

PS_OUTPUT ps_main(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		float3 viewDir = normalize(Input.viewDir);

		//���� ��
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//�⺻ ������ ó��

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;
	if (diff < 0.0)
		diff = abs(NdotL) * 0.3f;


	//����Ʈ �ݻ�
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//�⺻ ���� ����
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...

	//�߰��Ǵ� ����
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		//AddLingt.fx
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	}


	//
	// Diffuse
	//
	float3 diffuseColor = diffTex.rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;


		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.


	return Output;
}




//---------------------------------------------------------------
// CreateShadow ����
//---------------------------------------------------------------

struct PS_INPUT_SHADOW
{
	float4 FinalPos : TEXCOORD0;
	float2 Texcoord : TEXCOORD1;
};

float4 ps_CreateShadow(PS_INPUT_SHADOW Input) : COLOR0
{
	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	return float4(depth.xxx, 1);
}


//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//


float4x4 matLightViewProjection;			//���⼺ ���� ViewProjection ���

//������ Texture
texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
	Texture = (Shadow_Tex);
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
};

float bias = 0.01f;



struct PS_INPUT_RECIVESHADOW
{
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;

	float4 FinalPos : TEXCOORD6;
	float4 LightClipPos : TEXCOORD7;		//���� ���忡�� �ٶ� ��ġ
};


PS_OUTPUT ps_ReciveShadow(PS_INPUT_RECIVESHADOW Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.




	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		worldNormal = normalize(worldNormal);
	float3 viewDir = normalize(Input.viewDir);

		//���� ��
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//�⺻ ������ ó��

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;

	//�׸��ڰ� �׷����� �Ѵٸ� diff �� ������ ������ �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = abs(diff) * -1.0f;

	if (diff < 0.0)
		diff = abs(diff) * 0.3f;

	//����Ʈ �ݻ�
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//�⺻ ���� ����
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...


	//�߰��Ǵ� ����
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	}


	//
	// Diffuse
	//
	float3 diffuseColor = diffTex.rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;



		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}




//--------------------------------------------------------------//
// Render Toon
//--------------------------------------------------------------//

PS_OUTPUT ps_Toon(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;


	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	float3 worldNormal = normalize(Input.Normal);

	//������ ����
	float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	float3 lightDir = -dir;

	//������ �÷�
	float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


	float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;

	//������ ���뷮�� �ܰ� ���� ������.
	float interval = 1.0 / 4.0f;		//5�ܰ�
	toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
	toon = max(0.3f, toon);



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	//MaksTexture �÷�
	float3 maskColor = tex2D(MyMaskSamp, Input.Texcoord);
	float3 ColorTex = tex2D(ColorTexSamp, Input.Texcoord);
	
	float rColor=maskColor.r;
	float gColor=maskColor.g;
	float bColor=maskColor.b;
	//���÷� ���� ��� �Ǵ�?
	if(maskColor.r>0)
	{
		//rColor=float3(1,1,1);
	}
	if(maskColor.g>0)
	{
		//gColor=float3(1,1,1);
	}
	if(maskColor.b>0)
	{
		//bColor=float3(1,1,1);
	}
	

	//black �� ������ ��ŭ ���?
	float black = saturate(1.0f - (rColor + gColor + bColor));

	//������ ������ ���Ѵ�.
	float3 color1 = ColorTex.r * vColor1 * maskColor.r * finalColor.b;
	float3 color2 = ColorTex.g * vColor2 * maskColor.g * finalColor.g;
	float3 color3 = ColorTex.b * vColor3 * maskColor.b * finalColor.b;
	float3 color4 = finalColor * ColorTex * black;

	float3 finalColorMask = color1+color2+color3+color4;


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;

}

//--------------------------------------------------------------//
// Render ToonMask
//--------------------------------------------------------------//

PS_OUTPUT ps_ToonMask(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;


	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	float3 worldNormal = normalize(Input.Normal);

	//������ ����
	float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	float3 lightDir = -dir;

	//������ �÷�
	float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


	float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;

	//������ ���뷮�� �ܰ� ���� ������.
	float interval = 1.0 / 4.0f;		//5�ܰ�
	toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
	toon = max(0.3f, toon);



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	//MaksTexture �÷�
	float3 maskColor = tex2D(MyMaskSamp, Input.Texcoord);
	float3 ColorTex = tex2D(ColorTexSamp, Input.Texcoord);
	
	//���÷� ���� ��� �Ǵ�?
	if(maskColor.r>0)
	{
		//rColor=float3(1,1,1);
	}
	if(maskColor.g>0)
	{
		//gColor=float3(1,1,1);
	}
	if(maskColor.b>0)
	{
		//bColor=float3(1,1,1);
	}
	

	//black �� ������ ��ŭ ���?
	float black = saturate(1.0f - (maskColor.r + maskColor.g + maskColor.b));

	//������ ������ ���Ѵ�.
	float3 color1 = ColorTex.r * vColor1 * maskColor.r * finalColor.b;
	float3 color2 = ColorTex.g * vColor2 * maskColor.g * finalColor.g;
	float3 color3 = ColorTex.b * vColor3 * maskColor.b * finalColor.b;
	float3 color4 = finalColor * ColorTex * black;

	float3 finalColorMask = color1+color2+color3+color4;


	Output.baseColor = float4(finalColorMask, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;

}


//--------------------------------------------------------------//
// Render Distort
//--------------------------------------------------------------//

//�������� �׷��� ȭ���� ����
texture GrabTex;
sampler2D GrabSampler = sampler_state
{
	Texture = (GrabTex);
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
};

float distortAmout = 0.01f;
float scrollU = 0.0f;

PS_OUTPUT ps_Distort(PS_INPUT Input)
{

	PS_OUTPUT Output = (PS_OUTPUT)0;

	//���� ���� �׷����� ��ġ�� ȭ�� ��ǥ�� �÷��� �׷�����...
	float2 screenUV = Input.FinalPos.xy / Input.FinalPos.w;

	//-1 �� 1 ������ ���� 0 ~ 1 ������ ������..
	screenUV = (screenUV + 1.0f) * 0.5f;

	//Y �� ����
	screenUV.y = 1.0f - screenUV.y;
	
	float2 texUV = Input.Texcoord;
	texUV.y += scrollU;

	//�븻�� ��´�.
	float2 norXY = tex2D(Normal, texUV).xy;
	//�븻���� -1 ~ 1 ���̷�..
	norXY = (norXY * 2.0f) - 1.0f;

	//�븻 ����ŭ UV �ܰ�
	float2 uv = screenUV + norXY * distortAmout;
	//uv = saturate(uv);


	Output.baseColor = tex2D(GrabSampler, uv);

	return Output;
}



//--------------------------------------------------------------//
// Render With ShadowMap Toon
//--------------------------------------------------------------//

PS_OUTPUT ps_ReciveShadowToon(PS_INPUT_RECIVESHADOW Input) 
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.


	float3 worldNormal = normalize(Input.Normal);

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;
	//�׸��ڰ� �׷����� �Ѵٸ� diff �� 0.3f �� �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		toon = 0.3f;

	else
	{
		//������ ���뷮�� �ܰ� ���� ������.
		float interval = 1.0 / 4.0f;		//5�ܰ�
		toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
		toon = max(0.3f, toon);

	}



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}


//--------------------------------------------------------------//
// Render With ShadowMap Toon Mask
//--------------------------------------------------------------//

PS_OUTPUT ps_ReciveShadowToonMask(PS_INPUT_RECIVESHADOW Input) 
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.


	float3 worldNormal = normalize(Input.Normal);

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;
	//�׸��ڰ� �׷����� �Ѵٸ� diff �� 0.3f �� �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		toon = 0.3f;

	else
	{
		//������ ���뷮�� �ܰ� ���� ������.
		float interval = 1.0 / 4.0f;		//5�ܰ�
		toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
		toon = max(0.3f, toon);

	}



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);

	//MaksTexture �÷�
	float3 maskColor = tex2D(MyMaskSamp, Input.Texcoord);
	float3 ColorTex = tex2D(ColorTexSamp, Input.Texcoord);

	float2 uv=Input.Texcoord;

	float2 center=float2(0.5f,0.5f);
	float2 sc;
	
	sincos((-90.0f/180.0f*3.14159f),sc.x,sc.y);

	float2 uvtest=uv-center;
	
	float2 rotateduv;
	
	rotateduv.x=dot(uvtest,float2(sc.y,-sc.x));

	rotateduv.y=dot(uvtest,sc.xy);

	rotateduv+=center;
	
	uv=rotateduv;

	uv.x+=0.58f;
	uv.y+=0.71f;

	float3 RenderTex = tex2D(baseMap, uv);
	//���÷� ���� ��� �Ǵ�?
	if(maskColor.r>0)
	{
		//rColor=float3(1,1,1);
	}
	if(maskColor.g>0)
	{
		//gColor=float3(1,1,1);
	}
	if(maskColor.b>0)
	{
		//bColor=float3(1,1,1);
	}
	

	//black �� ������ ��ŭ ���?
	float black = saturate(1.0f - (maskColor.r + maskColor.g + maskColor.b));

	//������ ������ ���Ѵ�.
	float3 color1 = ColorTex.r * vColor1 * maskColor.r * finalColor.b;
	float3 color2 = ColorTex.g * vColor2 * maskColor.g * finalColor.g;
	float3 color3 = ColorTex.b * vColor3 * maskColor.b * finalColor.b*RenderTex;
	float3 color4 = finalColor * ColorTex * black;

	float3 finalColorMask = color1+color2+color3+color4;


	Output.baseColor = float4(finalColorMask, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}


//--------------------------------------------------------------//
// Render With ShadowMap Toon Mask Skag
//--------------------------------------------------------------//

PS_OUTPUT ps_ReciveShadowToonMaskSkag(PS_INPUT_RECIVESHADOW Input) 
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.


	float3 worldNormal = normalize(Input.Normal);

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;
	//�׸��ڰ� �׷����� �Ѵٸ� diff �� 0.3f �� �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		toon = 0.3f;

	else
	{
		//������ ���뷮�� �ܰ� ���� ������.
		float interval = 1.0 / 4.0f;		//5�ܰ�
		toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
		toon = max(0.3f, toon);

	}



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);

	//MaksTexture �÷�
	float3 maskColor = tex2D(MyMaskSamp, Input.Texcoord);
	float3 ColorTex = tex2D(ColorTexSamp, Input.Texcoord);
	
	
	//���÷� ���� ��� �Ǵ�?
	if(maskColor.r>0)
	{
		//rColor=float3(1,1,1);
	}
	if(maskColor.g>0)
	{
		//gColor=float3(1,1,1);
	}
	if(maskColor.b>0)
	{
		//bColor=float3(1,1,1);
	}
	

	//black �� ������ ��ŭ ���?
	float black = saturate(1.0f - (maskColor.r + maskColor.g + maskColor.b));

	//������ ������ ���Ѵ�.
	float3 color1 = ColorTex.r * vColor1 * maskColor.r * finalColor.r;
	float3 color2 = ColorTex.g * vColor2 * maskColor.g * finalColor.g;
	float3 color3 = ColorTex.b * vColor3 * maskColor.b * finalColor.b;
	float3 color4 = finalColor * ColorTex * black;

	float3 finalColorMask = color1+color2+color3+color4;


	Output.baseColor = float4(finalColorMask, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}

//--------------------------------------------------------------//
// Render With Outline
//--------------------------------------------------------------//


PS_OUTPUT ps_main_outline(PS_INPUT_RECIVESHADOW Input)
{
PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.




	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		worldNormal = normalize(worldNormal);
	float3 viewDir = normalize(Input.viewDir);

		//���� ��
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//�⺻ ������ ó��

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;

	//�׸��ڰ� �׷����� �Ѵٸ� diff �� ������ ������ �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = abs(diff) * -1.0f;

	if (diff < 0.0)
		diff = abs(diff) * 0.3f;

	//����Ʈ �ݻ�
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//�⺻ ���� ����
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...


	//�߰��Ǵ� ����
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	}


	//
	// Diffuse
	//
	float3 diffuseColor = diffTex.rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;



		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(0,0,0, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}

//---------------------------------------------------------------
// Render EV ����
//---------------------------------------------------------------

PS_OUTPUT ps_main_ev(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float2 testUV1=Input.Texcoord;
	
	Input.Texcoord.x+=gTimes*0.005f;
	//Input.Texcoord.y-=gTimes*0.005f;
	//gUV.x+=gTimes*0.005f;
	float2 testUV=float2(0.375f,0);
	
	testUV.x+=gTimes*0.025f;
	
	testUV1.y=testUV1.y/2.f;

	float4 MultiColor = tex2D(MultiSamp,testUV1);

	float4 TransitionColor = tex2D(TransitionTexSamp,testUV);

	float2 testUV0=Input.Texcoord;
	
	testUV0.x=testUV0.x/1000.f;

	testUV0.x+=gTimes*0.025f;
	
	float4 TransitionRealColor = tex2D(TransitionTexSamp,testUV0);

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		float3 viewDir = normalize(Input.viewDir);

		//���� ��
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//�⺻ ������ ó��

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;
	if (diff < 0.0)
		diff = abs(NdotL) * 0.3f;


	//����Ʈ �ݻ�
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//�⺻ ���� ����
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...

	//�߰��Ǵ� ����
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		//AddLingt.fx
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	}


	//
	// Diffuse
	//
	float3 diffuseColor = diffTex.rgb ;

		//
		// Specular 
		//
		float4 cloudysky = tex2D(Specular, Input.Texcoord);

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor=float3(0,0,0);
		
		finalColor = diffuseColor  + emissionColor;

		//finalColor.b=lerp(finalColor.b,finalColor.r,TransitionColor.b);
		cloudysky.b=lerp(cloudysky.b,cloudysky.g,TransitionColor.b);
		cloudysky.rgb=cloudysky.bbb;
		finalColor.b=lerp(finalColor.b,finalColor.g,TransitionColor.b);	
		finalColor.rgb=finalColor.bbb;
		//finalColor=cloudysky;
		//finalColor*=TransitionRealColor;

		//finalColor*=cloudysky;
		
		
		//MultiColor.g=MultiColor.r;
		//MultiColor.b=MultiColor.r;
		
		
		//finalColor*=cloudysky;
		//finalColor = float3(0.5f,0.5f,0.5f)-finalColor;
		finalColor+=TransitionRealColor;
		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.


	return Output;
}


PS_OUTPUT ps_main_water( PS_INPUT Input )
{   

	PS_OUTPUT Output = (PS_OUTPUT)0;

	float2 xy = Input.Texcoord.xy + ( gTimes * 0.005 );
         
  	float4 noize = tex2D(Texture1, xy);
   
   	float2 uv = Input.Texcoord.xy + noize.xy * 1.25;

	float4 diffTex = tex2D(Diffuse, uv);


	Output.baseColor = float4(diffTex.rgb, 1);
	Output.normalDepth = float4(1,1,1,1);		//alpha ���� ������ ���.


	return Output;
     
}

PS_OUTPUT ps_main_skag_spit( PS_INPUT Input )
{

		PS_OUTPUT Output = (PS_OUTPUT)0;

	float2 xy=Input.Texcoord.xy + (gTimes*0.1);
   
   	float4 noize=tex2D(Texture1,xy);
   
   	float2 uv=Input.Texcoord.xy+noize.xy*0.3f;

	float4 diffTex = tex2D(Diffuse, uv);




   	diffTex.rgb*=0.7f;
   	diffTex.g*=1.4f;

	Output.baseColor = float4(diffTex.rgb, 1);
	Output.normalDepth = float4(1,1,1,1);		//alpha ���� ������ ���.


	return Output;

   

   

   

   
}