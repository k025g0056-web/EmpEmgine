#include"Object3d.hlsli"
struct Material{
    float4 color;
    int32_t enableLighting;
};

struct DirectionalLight{
    float4 color;//!<ライトの色
    float3 direction;//!<ライトの向き
    float intensity;//!<輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gtexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PixelShaderOutput{
    float4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input){
    PixelShaderOutput output;
    float4 textureColor = gtexture.Sample(gSampler, input.texcoord);
    if (gMaterial.enableLighting){
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }else{
        output.color = gMaterial.color * textureColor;
    
    }
    
    return output;
}