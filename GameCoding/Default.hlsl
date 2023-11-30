
struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv       : TEXCOORD;
};;

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
};

cbuffer CameraData : register(b0)
{
    row_major matrix matView;
    row_major matrix matProjection;
}

cbuffer TransformData : register(b1)
{
    row_major matrix matWorld;
}

cbuffer AnimationData : register(b2)
{
    float2  spriteOffset;
    float2  spriteSize;
    float2  textureSize;
    float   useAnimation;
}

// IA - VS - RS - PS - OM
// �� �ܰ谡 Vertex Shader �ܰ� �������̴�.
// �Ʒ��� �Լ��� '����' ������ ����ȴ�.
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // WVP
    float4 position = mul(input.position, matWorld);
    position = mul(position, matView);
    position = mul(position, matProjection);
    
    output.position = position;
    output.uv = input.uv;
    
    if (useAnimation)
    {
        // 500 / 1000
        output.uv *= spriteSize / textureSize;
        output.uv += spriteOffset / textureSize;
    }
    
    return output;
}

Texture2D texture0 : register(t0);
// Texture2D texture1 : register(t1);

SamplerState sampler0 : register(s0); // ��� ���������� ���� �Ծ�
// SamplerState sampler1 : register(s1); // ��� ���������� ���� �Ծ�

// ������ ���õ� ���� ó���ϴ� �κ�
float4 PS(VS_OUTPUT input) : SV_Target
{   
    float4 color = texture0.Sample(sampler0, input.uv);
   //  float4 color2 = texture1.Sample(sampler1, input.uv);
    
    return color;
}