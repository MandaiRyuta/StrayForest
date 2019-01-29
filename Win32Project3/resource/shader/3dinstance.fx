float4x4 mWVP;
texture texDecal;

struct PsIn
{
    float4 sv_position : POSITION;
    float2 texcoord : TEXCOORD;
    float4 f4Color : COLOR0;
};

sampler Sampler = sampler_state //ÉTÉìÉvÉâÅ[
{
    Texture = <texDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

PsIn VS(
    float3 pos : POSITION,
    float2 localUV : TEXCOORD0,
    float3 normal : NORMAL,
    
    float4 world1 : TEXCOORD1,
    float4 world2 : TEXCOORD2,
    float4 world3 : TEXCOORD3,
    float4 world4 : TEXCOORD4
)
{
    PsIn output = (PsIn) 0;

    float4x4 MtxWorld = (float4x4) 0;
	
    MtxWorld._11 = world1.x;
    MtxWorld._12 = world1.y;
    MtxWorld._13 = world1.z;
    MtxWorld._14 = world1.w;
	
    MtxWorld._21 = world2.x;
    MtxWorld._22 = world2.y;
    MtxWorld._23 = world2.z;
    MtxWorld._24 = world2.w;

    MtxWorld._31 = world3.x;
    MtxWorld._32 = world3.y;
    MtxWorld._33 = world3.z;
    MtxWorld._34 = world3.w;

    MtxWorld._41 = world4.x;
    MtxWorld._42 = world4.y;
    MtxWorld._43 = world4.z;
    MtxWorld._44 = world4.w;

    MtxWorld = mWVP;

    output.sv_position = float4(pos, 1);
    output.sv_position = mul(output.sv_position, MtxWorld);
    output.texcoord = localUV;

    return output;

}

float4 PS(float2 texcoord : TEXCOORD) : COLOR
{
    return tex2D(Sampler, texcoord);
}

technique tech
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}