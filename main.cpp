#include <Novice.h>
#include <cassert>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

const char kWindowTitle[] = "LE2C_26_ヤマダショウエイ_MT4_01_01";

// Quaternion構造体
struct Quaternion {
    float x, y, z, w;
};

// Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
    Quaternion result;
    result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
    result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
    result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
    result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
    return result;
}


// 単位Quaternionを返す
Quaternion IdentityQuaternion() {
    return { 0.0f, 0.0f, 0.0f, 1.0f };
}

// 共役Quaternionを返す
Quaternion Conjugate(const Quaternion& quaternion) {
    return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

// Quaternionのノルムを計算
float Norm(const Quaternion& quaternion) {
    return std::sqrt(quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z +
        quaternion.w * quaternion.w);
}

// 正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion) {
    float norm = Norm(quaternion);
    assert(norm != 0.0f);
    return { quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm };
}

// 逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion) {
    float norm = Norm(quaternion);
    assert(norm != 0.0f);
    Quaternion conjugate = Conjugate(quaternion);
    return { conjugate.x / (norm * norm), conjugate.y / (norm * norm), conjugate.z / (norm * norm), conjugate.w / (norm * norm) };
}

// 表形式でQuaternionを描画
void PrintQuaternionToScreen(int x, int y, const char* label, const Quaternion& q) {
    std::ostringstream stream;
    stream << label << ": ";
    stream << std::fixed << std::setprecision(2)
        << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    Novice::ScreenPrintf(x, y, stream.str().c_str());
}

// ノルムを描画
void PrintNormToScreen(int x, int y, const char* label, float value) {
    std::ostringstream stream;
    stream << label << ": " << std::fixed << std::setprecision(2) << value;
    Novice::ScreenPrintf(x, y, stream.str().c_str());
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

    // クォータニオンの計算
    Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
    Quaternion q2 = { 1.0f, 5.0f, 4.0f, 2.0f };
    Quaternion identity = IdentityQuaternion();
    Quaternion conj = Conjugate(q1);
    Quaternion inv = Inverse(q1);
    Quaternion normal = Normalize(q1);
    Quaternion mul1 = Multiply(q1, q2);
    Quaternion mul2 = Multiply(q2, q1);
    float norm = Norm(q1);

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
        PrintQuaternionToScreen(0, 0, "Identity", identity);
        PrintQuaternionToScreen(0, 20, "Conjugate", conj);
        PrintQuaternionToScreen(0, 40, "Inverse", inv);
        PrintQuaternionToScreen(0, 60, "Normalize", normal);
        PrintQuaternionToScreen(0, 80, "Multiply(q1, q2)", mul1);
        PrintQuaternionToScreen(0, 100, "Multiply(q2, q1)", mul2);
        PrintNormToScreen(0, 120, "Norm", norm);

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
