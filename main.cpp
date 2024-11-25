#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "MT4_01_04_Basic";

struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float x, y, z, w;
};

struct Matrix4x4 {
    float m[4][4];
};

Vector3 Normalize(const Vector3& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length != 0.0f) {
        return { v.x / length, v.y / length, v.z / length };
    }
    return { 0.0f, 0.0f, 0.0f };
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
    return {
        vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
        vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
        vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2],
    };
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
    float halfAngle = angle / 2.0f;
    float sinHalfAngle = std::sin(halfAngle);
    return {
        axis.x * sinHalfAngle,
        axis.y * sinHalfAngle,
        axis.z * sinHalfAngle,
        std::cos(halfAngle)
    };
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
    // Quaternion (q) を使ってベクトルを回転
    Quaternion conjugate = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
    Quaternion qv = { vector.x, vector.y, vector.z, 0.0f };

    Quaternion temp = {
        quaternion.w * qv.x + quaternion.y * qv.z - quaternion.z * qv.y,
        quaternion.w * qv.y + quaternion.z * qv.x - quaternion.x * qv.z,
        quaternion.w * qv.z + quaternion.x * qv.y - quaternion.y * qv.x,
        -quaternion.x * qv.x - quaternion.y * qv.y - quaternion.z * qv.z
    };

    Quaternion result = {
        temp.w * conjugate.x + temp.x * conjugate.w + temp.y * conjugate.z - temp.z * conjugate.y,
        temp.w * conjugate.y - temp.x * conjugate.z + temp.y * conjugate.w + temp.z * conjugate.x,
        temp.w * conjugate.z + temp.x * conjugate.y - temp.y * conjugate.x + temp.z * conjugate.w,
        0.0f
    };

    return { result.x, result.y, result.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
    return {
        1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z),
        2.0f * (quaternion.x * quaternion.y + quaternion.z * quaternion.w),
        2.0f * (quaternion.x * quaternion.z - quaternion.y * quaternion.w),
        0.0f,

        2.0f * (quaternion.x * quaternion.y - quaternion.z * quaternion.w),
        1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z),
        2.0f * (quaternion.y * quaternion.z + quaternion.x * quaternion.w),
        0.0f,

        2.0f * (quaternion.x * quaternion.z + quaternion.y * quaternion.w),
        2.0f * (quaternion.y * quaternion.z - quaternion.x * quaternion.w),
        1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y),
        0.0f,

        0.0f, 0.0f, 0.0f, 1.0f
    };
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            Novice::ScreenPrintf(
                x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.03f",
                matrix.m[row][column]);
        }
    }
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
    Novice::ScreenPrintf(x, y, "%.02f", vector.x);
    Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
    Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
    Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

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

        Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f);
        Vector3 pointY = { 2.1f, -0.9f, 1.3f };
        Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
        Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
        Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "   : rotation");
        MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
        VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, "   : rotateByQuaternion");
        VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, "   : rotateByMatrix");

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
