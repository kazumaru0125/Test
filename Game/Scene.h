/*
	@file	Scene.h
	@brief	��ʓI�ȃV�[���N���X
*/
#pragma once

// �O���錾
class CommonResources;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
	class DepthStencil;
}


// ��ʓI�ȃV�[���N���X
class Scene
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// �f�o�b�O�J����
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	
	// �i�q��
	std::unique_ptr<mylib::GridFloor> m_gridFloor;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;

	// �g�[���X���f��
	std::unique_ptr<DirectX::Model> m_torusModel;

	//// ���_�V�F�[�_
	//Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;

	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;


public:
	Scene();
	~Scene();

	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	// �J�����Ɋւ���ݒ������
	void CreateCamera();
	// ���f�������[�h����
	void LoadModel(ID3D11Device* device);
	// �V�F�[�_�����[�h����
	void LoadShader(ID3D11Device* device);
};
