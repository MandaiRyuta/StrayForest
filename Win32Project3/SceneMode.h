#pragma once

constexpr int MeshFiled_X = 100;
constexpr int MeshFiled_Z = 100;
constexpr float MeshFiled_XSize = 20.0f;
constexpr float MeshFiled_ZSize = 20.0f;

class CMode
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};


