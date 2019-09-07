#pragma once
#include "../NoobieCore/NoobieD3D.h"
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include "NoobieCore/Utilities.h"
#include "NoobieCore/Shader.h"
#include "NoobieCore/StandardEffect.h"
#include "NoobieCore/BaseObject.h"
#include "FlyCamera.h"
#include "Timeline.h"

using namespace Noobie;
using namespace DirectX;

class NoobieApp :
	public NoobieD3D
{
private:
	vector<BaseObject *> scene;
	Timeline timeline;

	Shader shader;

	float accTime = 0;

	FlyCamera * camera;

	float effectAmount;

public:
	NoobieApp(std::string windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	~NoobieApp();

private:
	void Start() override;
	bool Update(float dt) override;
	void Draw(float dt) override;
};

