#pragma once

struct Vec2
{
	float x, y;

	Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct Vec3
{
	float x, y, z;

	Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

struct QAngle_t
{
	float pitch, yaw, roll;

	QAngle_t(float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f) : pitch(pitch), yaw(yaw), roll(roll) {}
};