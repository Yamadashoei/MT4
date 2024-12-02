#include <Novice.h>
#include <cmath> 

const char kWindowTitle[] = "LE2C_26_ヤマダ_ショウエイ";

static const int kRowHeight = 20;
static const int kColumnWidth = 100;

// Quaternion型の定義
struct Quaternion {
    float x, y, z, w;
};
// Vector3型の定義
struct Vector3 {
    float x, y, z;
};

// 指定した軸と角度を基に回転クォータニオンを生成する関数
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
    // 正規化された軸を前提とする（軸が正規化されていない場合、ここで正規化が必要）
    float halfAngle = angle / 2.0f;
    float sinHalfAngle = sin(halfAngle);
    float cosHalfAngle = cos(halfAngle);

    return {
        axis.x * sinHalfAngle, // x成分
        axis.y * sinHalfAngle, // y成分
        axis.z * sinHalfAngle, // z成分
        cosHalfAngle          // w成分
    };
}

// クォータニオンを画面に出力する関数
void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label) {
    Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
    Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
    Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
    Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
    Novice::ScreenPrintf(x + kColumnWidth * 4, y, "%s", label);
}


// 球面線形補間 (Slerp) 関数の実装例
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
    // 内積を計算
    float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

    // 内積が負の場合、クォータニオンを反転
    Quaternion q1Adjusted = q1;
    if (dot < 0.0f) {
        dot = -dot;
        q1Adjusted.x = -q1.x;
        q1Adjusted.y = -q1.y;
        q1Adjusted.z = -q1.z;
        q1Adjusted.w = -q1.w;
    }

    // クォータニオンの補間
    float theta = acos(dot);
    float sinTheta = sin(theta);

    float scale0 = sin((1.0f - t) * theta) / sinTheta;
    float scale1 = sin(t * theta) / sinTheta;

    return {
        scale0 * q0.x + scale1 * q1Adjusted.x,
        scale0 * q0.y + scale1 * q1Adjusted.y,
        scale0 * q0.z + scale1 * q1Adjusted.z,
        scale0 * q0.w + scale1 * q1Adjusted.w
    };
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // 回転クォータニオンの定義
    Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
    Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, 3.141592f);

    // 球面線形補間 (Slerp)
    Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
    Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
    Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
    Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
    Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓描画処理ここから
        ///

        // 各クォータニオン補間結果の描画
        QuaternionScreenPrintf(0, kRowHeight * 0, interpolate0, ": interpolate0, Slerp(q0, q1, 0.0f)");
        QuaternionScreenPrintf(0, kRowHeight * 1, interpolate1, ": interpolate1, Slerp(q0, q1, 0.3f)");
        QuaternionScreenPrintf(0, kRowHeight * 2, interpolate2, ": interpolate2, Slerp(q0, q1, 0.5f)");
        QuaternionScreenPrintf(0, kRowHeight * 3, interpolate3, ": interpolate3, Slerp(q0, q1, 0.7f)");
        QuaternionScreenPrintf(0, kRowHeight * 4, interpolate4, ": interpolate4, Slerp(q0, q1, 1.0f)");

        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}
