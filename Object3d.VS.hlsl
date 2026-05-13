struct TransformationMatrix{
    float4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
struct VertexShaderOutPut{
    float4 position : SV_Position;
};

struct VertexShaderInput{
    float4 position : POSITION0;
};

VertexShaderOutPut main(VertexShaderInput input){
    VertexShaderOutPut output;
    output.position = mul(input.position, gTransformationMatrix.WVP);
    return output;
}