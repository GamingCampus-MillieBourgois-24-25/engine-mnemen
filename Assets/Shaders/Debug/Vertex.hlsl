//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-11 11:30:10
//

struct VertexIn
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct VertexOut
{
    float4 Position : SV_Position;
    float3 Color : COLOR;
};

struct Settings
{
    column_major float4x4 Projection;
    column_major float4x4 View;
};

ConstantBuffer<Settings> PushConstants : register(b0);

VertexOut VSMain(VertexIn Input)
{
    VertexOut Output = (VertexOut)0;
    Output.Position = float4(Input.Position, 1.0);
    Output.Position = mul(PushConstants.View, Output.Position);
    Output.Position = mul(PushConstants.Projection, Output.Position);
    Output.Color = Input.Color;
    return Output;
}
