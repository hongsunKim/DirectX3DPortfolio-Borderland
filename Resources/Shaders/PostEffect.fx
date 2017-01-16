 
float pixelSizeU;			//
float pixelSizeV;
float pixelHalfSizeU;
float pixelHalfSizeV;

texture renderTexture_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {2048,2048};
   string Format="D3DFMT_R32F";
   float  ClearDepth=1.000000;
   int    ClearColor=-16777216;
>;

float4x4 mWVP;
float4x4 mWV;
float4x4 mRZ;

float gFar;
float gTime;

float WINSIZE_X;

float WINSIZE_Y;

struct VS_INPUT{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};



VS_OUTPUT vs_Base(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.pos = Input.pos;
	Output.uv = Input.uv;

	return Output;
}


// Base ///////////////////////////////////////////////////////////////

//Sampler
texture screenTex;
sampler2D screenSampler = sampler_state{
	Texture = screenTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	
};
texture ScopeGlassTex;
sampler2D ScopeGlassSampler = sampler_state{
	Texture = ScopeGlassTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	
};
texture ScopesDifTex;
sampler2D ScopesDifSampler = sampler_state{
	Texture = ScopesDifTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	
};
texture ScopesMasksTex;
sampler2D ScopesMasksSampler = sampler_state{
	Texture = ScopesMasksTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	
};

float4 ps_Base( VS_OUTPUT Input ) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	return tex2D( screenSampler, Input.uv );

	//return float4(tex2D(screenSampler, Input.uv).www, 1);

}


// OutLine ///////////////////////////////////////////////////////////////

//Sampler
texture normalTex;
sampler2D normalSampler = sampler_state{
	Texture = normalTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

texture scopeTex;
sampler2D scopeSampler = sampler_state{
	Texture = scopeTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float mask[9] = {
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1 };		//라플라스 필터

float coordX[3] = { -1, 0, 1 };
float coordY[3] = { -1, 0, 1 };


float4 ps_Outline(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float outlineStrength = 0.0f;

	float pX = pixelSizeU;
	float pY = pixelSizeV;

	//자신을 포함한 주변 픽셀정보 9개 
	for (int i = 0; i < 9; i++)
	{
		float2 uv = Input.uv + float2(coordX[i % 3] * pX, coordY[i / 3] * pY);

			float3 normal = tex2D(normalSampler, uv).rgb * mask[i];

		outlineStrength += normal.x;
		outlineStrength += normal.y;
		outlineStrength += normal.z;
	}

	outlineStrength = saturate( abs( outlineStrength ) );
	
	float4 outLineColor = float4(0, 0, 0, 1);
	float4 baseColor = tex2D(screenSampler, Input.uv);

	float4 finalColor = lerp(baseColor, outLineColor, outlineStrength);
	
	if(baseColor.r==1&&baseColor.g==0&&baseColor.b==1)
	{
		finalColor.r=1;
		finalColor.g=0;
		finalColor.b=1;
	}

	return finalColor;
}

// BlurX ///////////////////////////////////////////////////////////////

float BlurWeights[13] =
{
	0.002216,		// -6
	0.008764,       // -5
	0.026995,       // -4
	0.064759,       // -3
	0.120985,       // -2
	0.176033,       // -1
	0.199471,		//Center
	0.176033,		//	1
	0.120985,		//	2
	0.064759,		//	3
	0.026995,		//	4
	0.008764,		//	5
	0.002216		//	6
};

float bulrPos[13] =
{
	-6,		
	-5,     
	-4,     
	-3,     
	-2,     
	-1,     
	0,		
	1,		
	2,		
	3,		
	4,		
	5,		
	6
};

float blurScale = 1.0f;			//블러스케일

float4 ps_BlurX(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Input.uv + float2(bulrPos[i] * pixelSizeU, 0) * blurScale;

		//uv = saturate(uv);
		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		finalColor += tex2D(screenSampler, uv) * BlurWeights[i];
	}

	return finalColor;
}



float4 ps_BlurY(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Input.uv + float2(0, bulrPos[i] * pixelSizeV) * blurScale;
		//uv = saturate(uv);
		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		finalColor += tex2D(screenSampler, uv) * BlurWeights[i];
	}

	return finalColor;
}


// BBO ///////////////////////////////////////////////////////////////


//Sampler
texture blurTex;
sampler2D blurSampler = sampler_state{
	Texture = blurTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float blurAmout = 3.0f;

float4 ps_BBO(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;
	
	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float4 blurTex = tex2D(blurSampler, Input.uv);

	return orignalTex + blurTex * blurAmout;
}

// Depth Of Field ///////////////////////////////////////////////////////////////

float focus = 0.01f;
float fieldRange = 5.0f;

float4 ps_DepthOfField(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float4 blurTex = tex2D(blurSampler, Input.uv);
	float depth = tex2D(normalSampler, Input.uv).a;


	return lerp(orignalTex, blurTex, saturate(fieldRange * abs(focus - depth)));
}

// Gray ///////////////////////////////////////////////////////////////

float4 ps_Gray(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 texColor = tex2D(screenSampler, Input.uv);

	float gray = texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f;
	return float4(gray, gray, gray, 1);
}

//outline///////////////////////////////////////////////////////////////////

float4 ps_outline(VS_OUTPUT Input) : COLOR0
{	
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

   	//float4 test=tex2D(Texture0,Input.mUV);
   	//float4 test1=tex2D(Texture1,Input.mUV);
   	float4 test2=tex2D( screenSampler, Input.uv );

	if((test2).r>0&&(test2).g==0&&(test2).b>0)
   	{
      		test2=float4(1,0,0,1);
   	}

	return test2;

	//Input.uv.x += pixelHalfSizeU;
	//Input.uv.y += pixelHalfSizeV;

	//return tex2D( screenSampler, Input.uv );
}
//scope///////////////////////////////////////////////////////////////////
float4 ps_scope(VS_OUTPUT Input) : COLOR0
{	
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float2 uv=Input.uv;
	
	float percent=WINSIZE_X/WINSIZE_Y;
	float percentDiffer=(WINSIZE_X-WINSIZE_Y)/(WINSIZE_Y*2);

	uv.x*=percent;
	uv.x-=percentDiffer;
	//if(uv.x<0 ) uv.x=0;
	
	float2 mUV=uv;
	if(uv.x<0 || uv.x>0.99 ) uv.x=0;
	float4 GlassTex=tex2D(ScopeGlassSampler, uv);

	
	uv=mUV;
	uv.y*=0.6f;
	uv.x*=0.6f;
	
	uv.x+=0.2f;
	uv.y+=0.2f;
	float2 center=float2(0.5f,0.5f);
	float2 sc;
	
	sincos((gTime/180.0f*3.14159f),sc.x,sc.y);

	float2 uvtest=uv-center;
	
	float2 rotateduv;
	
	rotateduv.x=dot(uvtest,float2(sc.y,-sc.x));

	rotateduv.y=dot(uvtest,sc.xy);

	rotateduv+=center;
	
	uv=rotateduv;
	
	//uv.x+=0.0025f;
	//uv.y+=0.0025f;
	//if(uv.x<0 || uv.x>0.99 ) uv.x=0.01;
	//if(uv.y<0 || uv.y>0.99 ) uv.y=0.01;
   	float4 CameraTex=tex2D( screenSampler, Input.uv );
	float4 ScopeTex= tex2D(scopeSampler, uv);
	float4 ScopeDifTex=tex2D(ScopesDifSampler, uv);
	float4 ScopeMasksTex=tex2D(ScopesMasksSampler, uv);

	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;


	float4 baseColor = tex2D(screenSampler, Input.uv);

	float4 finalColor;
	
	if(baseColor.r==1&&baseColor.g==0&&baseColor.b==1)
	{
		finalColor.r=1;
		finalColor.g=0;
		finalColor.b=1;
	}
	float4 finalScopeDif;
	
	//각컬러 량이 어떻게 되니?
	float r = ScopeMasksTex.r;
	float g = ScopeMasksTex.g;
	float b = ScopeMasksTex.b;

	//black 의 비율이 얼만큼 쎄니?
	float black = saturate(1.0f - (r + g + b));
	if(ScopeDifTex.b>0)
	{
		ScopeDifTex.r=0;
		ScopeDifTex.g=0;
		//finalScopeDif = ScopeDifTex*float4(1,1,0,1)*r+ScopeDifTex*float4(0,1,1,1)*g+ScopeDifTex*float4(0,0,1,1)*b+ScopeDifTex*black;
		finalScopeDif=ScopeDifTex*float4(1,1,1,1);
		GlassTex.b=0;
	}
	else
	{
		ScopeDifTex.r=0;
		ScopeDifTex.g=0;
		finalScopeDif.r=0;
		finalScopeDif.g=0;
		finalScopeDif.b=0;
	}
	if(finalScopeDif.b>0)
	{
		finalScopeDif.r=finalScopeDif.b;
		finalScopeDif.g=finalScopeDif.b;
	}
	if(ScopeTex.r==0&&ScopeTex.g==0&&ScopeTex.b==0)
	{
		GlassTex.b=0;
		GlassTex.r=0;
		GlassTex.g=0;
	}
	float4 realfinalColor=baseColor*ScopeTex;
	if(ScopeTex.r>0&&ScopeTex.g>0&&ScopeTex.b>0&&finalScopeDif.r==finalScopeDif.b&&finalScopeDif.g==finalScopeDif.b&&finalScopeDif.b!=0)
	{
		realfinalColor.r=0;
		realfinalColor.g=0;
		realfinalColor.b=0;
	}
	realfinalColor.r+=GlassTex.b;
	
	float4 test=(realfinalColor+finalScopeDif);
	
	
	if(uv.x>1.0f || uv.y>1.0f || uv.x<0.0f || uv.y<0.0f)
	{
		test.r=0;
		test.g=0;
		test.b=0;
	}
	
	return test;
	

	//return CameraTex*ScopeTex;
}


// ColorLevel ///////////////////////////////////////////////////////////////

float InBlack = 0.0f;			//컬러를 0 ~ 255 로 볼때 이값 미만이면 0 으로 처리한다.
float InWhite = 255.0f;			//컬러를 0 ~ 255 로 볼때 이값 이상이면 1 으로 처리한다.
float Gamma = 3.0f;				//증폭 
float OutBlack = 0.0f;			//감마 처리 이후 얻어진 값을 0 ~ 255 로 볼때 이값 미만이면 0 으로 처리한다.
float OutWhite = 255.0f;		//감마 처리 이후 얻어진 값을 0 ~ 255 로 볼때 이값 이상이면 1 으로 처리한다.



float LevelSet(float value){

	// 0 ~ 1 사이의 범위 의 컬러 값을 0 ~ 255 범위로...
	float c = value * 255.0f;

	//0 밑으로 안내려 가게....
	c = max(0, c - InBlack);

	//컬러 값 InBlack 에서 InWhite 에 대한 범위의 Factor 값 
	//이과정에서 c 는 0 ~ 1 사이의 값이 된다.
	c = saturate(c / ( InWhite - InBlack));

	//saturate ( value )  =  value 값은 0 ~ 1 사이의 값으로 Clamp 된다.

	//감마 처리 
	c = pow(c, Gamma);

	//컬러 Out 처리 	( 여기서 다시 0 ~ 255 의 컬러 범위 값이 된다 )	
	// c = 0.5
	// OutBlack = 100
	// OutWhite = 200
	// 최종값은 = 150 
	c = c * ( OutWhite - OutBlack) +  OutBlack;

	//최종 컬러 
	c = saturate(c / 255.0f);

	return c;
}


float4 ps_ColorLevel(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 texColor = tex2D(screenSampler, Input.uv);

	float4 finalColor;
	finalColor.r = LevelSet(texColor.r);
	finalColor.g = LevelSet(texColor.g);
	finalColor.b = LevelSet(texColor.b);
	finalColor.a = 1.0f;

	return finalColor;
}






technique Base
{
    pass p0
    {
		ZWRITEENABLE = FALSE;
        VertexShader = compile vs_3_0 vs_Base();
        PixelShader = compile ps_3_0 ps_Base();
    }
}

technique OutLine
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Outline();
	}
}

technique ScopeOutLine
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_scope();
	}
}

technique Outline
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_outline();
	}
}

technique BlurX
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurX();
	}
}

technique BlurY
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurY();
	}
}


technique BBo
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BBO();
	}
}


technique DepthOfField
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_DepthOfField();
	}
}


technique Gray
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Gray();
	}
}


technique ColorLevel
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_ColorLevel();
	}
}