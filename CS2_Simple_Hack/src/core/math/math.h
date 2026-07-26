#pragma once
#include <cmath>

struct Vec2
{
	float x, y;

	Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct Vec3
{
    float x, y, z;

    constexpr Vec3()
        : x(0.f), y(0.f), z(0.f) {
    }

    constexpr Vec3(float x, float y, float z)
        : x(x), y(y), z(z) {
    }

    // Addition
    constexpr Vec3 operator+(const Vec3& rhs) const
    {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    // Subtraction
    constexpr Vec3 operator-(const Vec3& rhs) const
    {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    // Scalar multiplication
    constexpr Vec3 operator*(float s) const
    {
        return { x * s, y * s, z * s };
    }

    // Scalar division
    constexpr Vec3 operator/(float s) const
    {
        return { x / s, y / s, z / s };
    }

    // +=
    Vec3& operator+=(const Vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    // -=
    Vec3& operator-=(const Vec3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    // *=
    Vec3& operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    // /=
    Vec3& operator/=(float s)
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    float Length2D() const
    {
        return std::sqrt(x * x + y * y);
    }

    float Dot(const Vec3& rhs) const
    {
        return x * rhs.x +
            y * rhs.y +
            z * rhs.z;
    }

    Vec3 Cross(const Vec3& rhs) const
    {
        return
        {
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        };
    }

    Vec3 Normalize() const
    {
        float len = Length();

        if (len == 0.f)
            return {};

        return *this / len;
    }
};

struct QAngle_t
{
    float pitch;
    float yaw;
    float roll;

    constexpr QAngle_t()
        : pitch(0.f), yaw(0.f), roll(0.f)
    {
    }

    constexpr QAngle_t(float pitch, float yaw, float roll = 0.f)
        : pitch(pitch), yaw(yaw), roll(roll)
    {
    }

    static float NormalizeAngle(float angle)
    {
        angle = std::fmod(angle, 360.0f);

        if (angle <= -180.0f)
            angle += 360.0f;

        if (angle > 180.0f)
            angle -= 360.0f;

        return angle;
    }

    void Normalize()
    {
        pitch = NormalizeAngle(pitch);
        yaw = NormalizeAngle(yaw);
        roll = NormalizeAngle(roll);
    }

    [[nodiscard]]
    QAngle_t Normalized() const
    {
        QAngle_t result = *this;
        result.Normalize();
        return result;
    }

    [[nodiscard]]
    float LengthSquared() const
    {
        return
            pitch * pitch +
            yaw * yaw +
            roll * roll;
    }

    [[nodiscard]]
    float Length2DSquared() const
    {
        return
            pitch * pitch +
            yaw * yaw;
    }

    //-------------------------
    // Arithmetic operators
    //-------------------------

    QAngle_t operator+(const QAngle_t& rhs) const
    {
        return QAngle_t(
            pitch + rhs.pitch,
            yaw + rhs.yaw,
            roll + rhs.roll
        ).Normalized();
    }

    QAngle_t operator-(const QAngle_t& rhs) const
    {
        return QAngle_t(
            pitch - rhs.pitch,
            yaw - rhs.yaw,
            roll - rhs.roll
        ).Normalized();
    }

    QAngle_t operator*(float scalar) const
    {
        return QAngle_t(
            pitch * scalar,
            yaw * scalar,
            roll * scalar
        ).Normalized();
    }

    QAngle_t operator/(float scalar) const
    {
        return QAngle_t(
            pitch / scalar,
            yaw / scalar,
            roll / scalar
        ).Normalized();
    }

    //-------------------------
    // Compound assignment
    //-------------------------

    QAngle_t& operator+=(const QAngle_t& rhs)
    {
        pitch += rhs.pitch;
        yaw += rhs.yaw;
        roll += rhs.roll;

        Normalize();
        return *this;
    }

    QAngle_t& operator-=(const QAngle_t& rhs)
    {
        pitch -= rhs.pitch;
        yaw -= rhs.yaw;
        roll -= rhs.roll;

        Normalize();
        return *this;
    }

    QAngle_t& operator*=(float scalar)
    {
        pitch *= scalar;
        yaw *= scalar;
        roll *= scalar;

        Normalize();
        return *this;
    }

    QAngle_t& operator/=(float scalar)
    {
        pitch /= scalar;
        yaw /= scalar;
        roll /= scalar;

        Normalize();
        return *this;
    }

    //-------------------------
    // Comparison
    //-------------------------

    bool operator==(const QAngle_t& rhs) const
    {
        return pitch == rhs.pitch &&
            yaw == rhs.yaw &&
            roll == rhs.roll;
    }

    bool operator!=(const QAngle_t& rhs) const
    {
        return !(*this == rhs);
    }
};