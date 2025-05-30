#include "Camera.h"

void Camera::Initialize()
{
	//カメラの初期位置
	cameraTranslate = { 0.0f,1.9f,-6.49f };
	//カメラの初期回転
	cameraRotate = { 0.26f,0.0f,0.0f };

	cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
	viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
	viewProjectionMatrix = MakeViewProjectionMatrix(cameraMatrix, float(kWindowWidth) / float(kWindowHeight));

}

void Camera::Update(char keys[], char preKeys[])
{
	cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
	viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
	viewProjectionMatrix = MakeViewProjectionMatrix(cameraMatrix, float(kWindowWidth) / float(kWindowHeight));
	Move(keys, preKeys);

	ImGui();

}

void Camera::Move(char keys[], char preKeys[])
{
	//カメラが移動するか
	if (keys[DIK_TAB] && !preKeys[DIK_TAB]) {
		if (!IsMoveCamera) {
			IsMoveCamera = true;
		} else {
			IsMoveCamera = false;
		}
	}



	if (IsMoveCamera) {

		//移動
		if (keys[DIK_SPACE]) {
			//上下移動はSPACE押されているときのみ
			if (keys[DIK_W]) { cameraTranslate.y += 0.01f; }
			if (keys[DIK_S]) { cameraTranslate.y -= 0.01f; }
		} else {
			if (keys[DIK_W]) { cameraTranslate.z += 0.01f; }
			if (keys[DIK_A]) { cameraTranslate.x -= 0.01f; }
			if (keys[DIK_D]) { cameraTranslate.x += 0.01f; }
			if (keys[DIK_S]) { cameraTranslate.z -= 0.01f; }


		}
		//マウスホイールの値を持つ
		int mouseWheel = 0;
		mouseWheel += Novice::GetWheel();
		cameraTranslate.z += mouseWheel * 0.001f;
		///正規化
		//回転
		if (Novice::IsPressMouse(0)) {
			//マウスの移動量を取得
			int mouseX, mouseY;
			Novice::GetMousePosition(&mouseX, &mouseY);


			//マウスの移動量を取得
			int deltaX = mouseX - (kWindowWidth / 2);
			int deltaY = mouseY - (kWindowHeight / 2);


			//マウスの移動量をカメラの回転に反映
			cameraRotate.x += deltaY * 0.00001f;
			cameraRotate.y += deltaX * 0.00001f;

			//カメラの回転を制限
			if (cameraRotate.x > 1.5f) cameraRotate.x = 1.5f;
			if (cameraRotate.x < -1.5f) cameraRotate.x = -1.5f;
			if (cameraRotate.y > 1.5f) cameraRotate.y = 1.5f;
			if (cameraRotate.y < -1.5f) cameraRotate.y = -1.5f;
			//カメラの回転を反映
			cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		}

	}



}

void Camera::ImGui()
{

	ImGui::Begin("Camera");

	ImGui::Text("ChangeMovingMode : TAB Key");
	ImGui::Text("CameraMove: %s", IsMoveCamera ? "TRUE" : "FALSE");
	
	//カメラの移動
	ImGui::Checkbox("Moving Camera", &IsMoveCamera);


	ImGui::Text("CameraTranslate: %f,%f,%f", cameraTranslate.x, cameraTranslate.y, cameraTranslate.z);
	ImGui::Text("CameraRotate: %f,%f,%f", cameraRotate.x, cameraRotate.y, cameraRotate.z);

	//カメラを初期状態に戻す
	if (ImGui::Button("Reset Camera")) {
		Initialize();
	}
	ImGui::End();
}
