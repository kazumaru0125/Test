/*
    ���K�p�s�N�Z���V�F�[�_
*/

#include "Common.hlsli"

struct PSInput
{
    //float4 PositionWS : TEXCOORD0;
    //float4 NormalWS   : TEXCOORD1;
    float4 Diffus     : COLOR; // COLOR�Z�}���e�B�N�X��ǉ�
};

////���K1
//float4 main() : SV_Target0
//{
//    return float4(1.0f, 0.0f, 0.0f, 1.0f);
//}

////���K2
float4 main(PSInput input) : SV_Target0
{
    return input.Diffus; // Diffus�����̂܂ܕԂ�
}

////���K�R
//float4 main(PSInput input) : SV_Target0
//{
//    // �@���𐳋K������
//    float3 normal = normalize(input.NormalWS);
    
//    // ���˃x�N�g���̋t�x�N�g���𐳋K������=���C�g����������Ɍ������x�N�g��
//    float3 toLight = normalize(-LightDirection[0]);
    
//    // �@���x�N�g���ƃ��C�g�x�N�g�����甽�ˌ��̋������v�Z����
//    float NdotL = dot(normal, toLight);
    
//    // �������������1�A�e�̕����[��
//    float zeroL = step(0, NdotL);
    
//    // ���C�g�̑��ʂ��v�Z����
//    float lightAmount = zeroL * NdotL;
    
//    // �g�U���˂̋������v�Z����
//    float3 diffuse = mul(lightAmount, LightDiffuseColor[0]) * DiffuseColor.rgb + EmissiveColor;
    
//    // �ŏI�I�ȐF���v�Z����B����͂��܂�Ӗ����Ȃ����
//    float3 finalColor = diffuse;
    
//    return float4(finalColor, 1.0f);
//}





