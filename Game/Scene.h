/*
	@file	Scene.h
	@brief	一般的なシーンクラス
*/
#pragma once

// 前方宣言
class CommonResources;

namespace mylib
{
	class DebugCamera;
	class GridFloor;
	class DepthStencil;
}


// 一般的なシーンクラス
class Scene
{
private:
	// 共通リソース
	CommonResources* m_commonResources;

	// デバッグカメラ
	std::unique_ptr<mylib::DebugCamera> m_debugCamera;
	
	// 格子床
	std::unique_ptr<mylib::GridFloor> m_gridFloor;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	// トーラスモデル
	std::unique_ptr<DirectX::Model> m_torusModel;

	//// 頂点シェーダ
	//Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;

	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;


public:
	Scene();
	~Scene();

	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	// カメラに関する設定をする
	void CreateCamera();
	// モデルをロードする
	void LoadModel(ID3D11Device* device);
	// シェーダをロードする
	void LoadShader(ID3D11Device* device);
};
