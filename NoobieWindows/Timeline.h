#pragma once
#include <list>
#include <functional>
#include <DirectXMath.h>

struct Action
{
	float startTime;
	std::function<void()> callback;
};

struct Tweener
{
	float duration;
	std::function<void(float* tweenedVars)> onUpdate;
	std::vector<float> starts;
	std::vector<float> targets;
	float startTime;

	Tweener(float duration,
		std::function<void(float* tweenedVars)> onUpdate,
		std::vector<float> starts,
		std::vector<float> targets);
	Tweener(float duration,
		std::function<void(float* tweenedVars)> onUpdate,
		DirectX::XMFLOAT3 start3s, DirectX::XMFLOAT4 start4s,
		DirectX::XMFLOAT3 target3s, DirectX::XMFLOAT4 target4s);
};

class Timeline
{
private:
	float time;
	std::list<Action> actions;
	std::list<Tweener> tweeners;

public:
	Timeline();
	~Timeline();

	void AddAction(Action action);
	void RunTweener(Tweener tweener);

	void Tick(float time);
};

