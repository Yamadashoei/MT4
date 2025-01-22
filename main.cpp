#include <Novice.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>

const char kWindowTitle[] = "MT4_01_03_Basic";

struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float x, y, z, w;
};

// 単位クォータニオン
Quaternion IdentityQuaternion() {
    return { 0.0f, 0.0f, 0.0f, 1.0f };
}

// クォータニオンの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
    return {
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
        lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    };
}

// 共役クォータニオン
Quaternion Conjugate(const Quaternion& quaternion) {
    return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

// クォータニオンのノルム
float Norm(const Quaternion& quaternion) {
    return std::sqrt(
        quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z +
        quaternion.w * quaternion.w
    );
}

// 正規化されたクォータニオン
Quaternion Normalize(const Quaternion& quaternion) {
    float norm = Norm(quaternion);
    return {
        quaternion.x / norm,
        quaternion.y / norm,
        quaternion.z / norm,
        quaternion.w / norm
    };
}

// 逆クォータニオン
Quaternion Inverse(const Quaternion& quaternion) {
    Quaternion conj = Conjugate(quaternion);
    float normSquared = Norm(quaternion) * Norm(quaternion);
    return {
        conj.x / normSquared,
        conj.y / normSquared,
        conj.z / normSquared,
        conj.w / normSquared
    };
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void QuaternionScreenPrintf(int x, int y, Quaternion quaternion, const char* label) {
    Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
    Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
    Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
    Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
    Novice::ScreenPrintf(x + kColumnWidth * 4, y, label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    const int kWindowWidth = 1280;
    const int kWindowHeight = 720;
    Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
        Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };
        Quaternion identity = IdentityQuaternion();
        Quaternion conj = Conjugate(q1);
        Quaternion inv = Inverse(q1);
        Quaternion normal = Normalize(q1);
        Quaternion mul1 = Multiply(q1, q2);
        Quaternion mul2 = Multiply(q2, q1);
        float norm = Norm(q1);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        QuaternionScreenPrintf(0, kRowHeight * 0, identity, "   : Identity");
        QuaternionScreenPrintf(0, kRowHeight * 1, conj, "   : Conjugate");
        QuaternionScreenPrintf(0, kRowHeight * 2, inv, "   : Inverse");
        QuaternionScreenPrintf(0, kRowHeight * 3, normal, "   : Normalize");
        QuaternionScreenPrintf(0, kRowHeight * 4, mul1, "   : Multiply(q1, q2)");
        QuaternionScreenPrintf(0, kRowHeight * 5, mul2, "   : Multiply(q2, q1)");
        Novice::ScreenPrintf(0, kRowHeight * 6, "%.02f                          : Norm", norm);

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
