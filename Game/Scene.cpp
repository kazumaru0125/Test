/*
	@file	Scene.cpp
	@brief	一般的なシーンクラス
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
// コンストラクタ
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
// デストラクタ
//---------------------------------------------------------
Scene::~Scene()
{
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void Scene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	// 必要なリソースを取得する
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// カメラ、モデル、シェーダを準備する
	this->CreateCamera();
	this->LoadModel(device);
	this->LoadShader(device);

	// グリッド床を作成する
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void Scene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// デバッグカメラを更新する
	m_debugCamera->Update(m_commonResources->GetInputManager());
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void Scene::Render()
{
	// 必要なリソースを取得する
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// ビュー行列を取得する
	Matrix view = m_debugCamera->GetViewMatrix();

	// 格子床を描画する
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


	// トーラスモデルを描画する
	Matrix world = Matrix::CreateTranslation(0.0f, 1.0f, 0.0f);
	m_torusModel->Draw(context, *states, world, view, m_projection, false, [&]()
		{
			//シェーダーの設定
			context->PSSetShader(m_ps.Get(), nullptr, 0);
		});

	



	// デバッグ情報を表示する
	auto debugString = m_commonResources->GetDebugString();
	debugString->AddString("Pixel Shader Sample");
}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void Scene::Finalize()
{
	// do nothing.
}

//---------------------------------------------------------
// カメラに関する設定をする
//---------------------------------------------------------
void Scene::CreateCamera()
{
	// 画面の大きさ
	RECT rect = m_commonResources->GetDeviceResources()->GetOutputSize();

	// デバッグカメラを作成する
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	m_debugCamera->Initialize(rect.right, rect.bottom);

	// 射影行列を作成する
	m_projection = Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 100.0f
	);
}

//---------------------------------------------------------
// モデルをロードする
//---------------------------------------------------------
void Scene::LoadModel(ID3D11Device* device)
{
	assert(device);

	// モデルを読み込む準備
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");

	// トーラスモデルをロードする
	m_torusModel = Model::CreateFromCMO(device, L"Resources/Models/Torus_noTex.cmo", *fx);
}

//---------------------------------------------------------
// シェーダをロードする
//---------------------------------------------------------
void Scene::LoadShader(ID3D11Device* device)
{
	assert(device);

	// シェーダ読み込み用変数
	std::vector<uint8_t> blob;

	// ○○用のピクセルシェーダをロードする
	blob = DX::ReadData(L"Resources/Shaders/PS_Sample.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);
}
