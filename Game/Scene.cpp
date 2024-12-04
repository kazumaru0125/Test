/*
	@file	Scene.cpp
	@brief	��ʓI�ȃV�[���N���X
*/
#include "pch.h"
#include "Scene.h"
#include "CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugCamera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>

#include "Libraries/Microsoft/ReadData.h"
#include "StepTimer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Scene::Scene()
	:
	m_commonResources{},
	m_debugCamera{},
	m_gridFloor{},
	m_projection{},
	m_torusModel{},
	//m_vs{},
	m_ps{}
{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Scene::~Scene()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Scene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	// �K�v�ȃ��\�[�X���擾����
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// �J�����A���f���A�V�F�[�_����������
	this->CreateCamera();
	this->LoadModel(device);
	this->LoadShader(device);

	// �O���b�h�����쐬����
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void Scene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// �f�o�b�O�J�������X�V����
	m_debugCamera->Update(m_commonResources->GetInputManager());
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void Scene::Render()
{
	// �K�v�ȃ��\�[�X���擾����
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// �r���[�s����擾����
	Matrix view = m_debugCamera->GetViewMatrix();

	// �i�q����`�悷��
	m_gridFloor->Render(context, view, m_projection);

	m_torusModel->UpdateEffects([&](IEffect* effect)
		{
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetLightingEnabled(false);
				basicEffect->SetPerPixelLighting(false);
				basicEffect->SetTextureEnabled(false);
				basicEffect->SetVertexColorEnabled(false);
				basicEffect->SetFogEnabled(false);
			}
		}
	);


	// �g�[���X���f����`�悷��
	Matrix world = Matrix::CreateTranslation(0.0f, 1.0f, 0.0f);
	m_torusModel->Draw(context, *states, world, view, m_projection, false, [&]()
		{
			//�V�F�[�_�[�̐ݒ�
			context->PSSetShader(m_ps.Get(), nullptr, 0);
		});

	



	// �f�o�b�O����\������
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Pixel Shader Sample");
}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void Scene::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// �J�����Ɋւ���ݒ������
//---------------------------------------------------------
void Scene::CreateCamera()
{
	// ��ʂ̑傫��
	RECT rect = m_commonResources->GetDeviceResources()->GetOutputSize();

	// �f�o�b�O�J�������쐬����
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// �ˉe�s����쐬����
	m_projection = Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}

//---------------------------------------------------------
// ���f�������[�h����
//---------------------------------------------------------
void Scene::LoadModel(ID3D11Device* device)
{
	assert(device);

	// ���f����ǂݍ��ޏ���
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// �g�[���X���f�������[�h����
	m_torusModel = Model::CreateFromCMO(device, L"Resources/Models/Torus_noTex.cmo", *fx);
}

//---------------------------------------------------------
// �V�F�[�_�����[�h����
//---------------------------------------------------------
void Scene::LoadShader(ID3D11Device* device)
{
	assert(device);

	// �V�F�[�_�ǂݍ��ݗp�ϐ�
	std::vector<uint8_t> blob;

	// �����p�̃s�N�Z���V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/PS_Sample.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);
}
