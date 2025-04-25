#pragma once
#include <Novice.h>
#include "MyFunction.h"
#include <imgui.h>
class Camera
{
	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

	Vector3 cameraTranslate = { 0.0f,0.0f,0.0f };
	Vector3 cameraRotate = { 0.0f,0.0f,0.0f };
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewportMatrix; 
	Matrix4x4 viewProjectionMatrix;


	//カメラが移動するか
	bool IsMoveCamera = false;



public:
	Camera() = default;
	~Camera() = default;
	//初期化
	void Initialize();

	//更新処理
	void Update(char keys[],char preKeys[]);
	//カメラの移動処理
	void Move(char keys[], char preKeys[]);
	//カメラのImGui
	void ImGui();

	//アクセッサ
	void SetCameraTranslate(Vector3 translate) { cameraTranslate = translate; }
	void SetCameraRotate(Vector3 rotate) { cameraRotate = rotate; }
	
	Vector3 GetCameraTranslate() const { return cameraTranslate; }
	Vector3 GetCameraRotate() const { return cameraRotate; }
	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix; }
	Matrix4x4 GetViewportMatrix() const { return viewportMatrix; }

};

