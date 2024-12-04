/*
    練習用ピクセルシェーダ
*/

#include "Common.hlsli"

struct PSInput
{
    //float4 PositionWS : TEXCOORD0;
    //float4 NormalWS   : TEXCOORD1;
    float4 Diffus     : COLOR; // COLORセマンティクスを追加
};

////演習1
//float4 main() : SV_Target0
//{
//    return float4(1.0f, 0.0f, 0.0f, 1.0f);
//}

////演習2
float4 main(PSInput input) : SV_Target0
{
    return input.Diffus; // Diffusをそのまま返す
}

////演習３
//float4 main(PSInput input) : SV_Target0
//{
//    // 法線を正規化する
//    float3 normal = normalize(input.NormalWS);
    
//    // 入射ベクトルの逆ベクトルを正規化する=ライトがある方向に向かうベクトル
//    float3 toLight = normalize(-LightDirection[0]);
    
//    // 法線ベクトルとライトベクトルから反射光の強さを計算する
//    float NdotL = dot(normal, toLight);
    
//    // 光が当たる方が1、影の方がゼロ
//    float zeroL = step(0, NdotL);
    
//    // ライトの総量を計算する
//    float lightAmount = zeroL * NdotL;
    
//    // 拡散反射の強さを計算する
//    float3 diffuse = mul(lightAmount, LightDiffuseColor[0]) * DiffuseColor.rgb + EmissiveColor;
    
//    // 最終的な色を計算する。今回はあまり意味がない代入
//    float3 finalColor = diffuse;
    
//    return float4(finalColor, 1.0f);
//}





