#include "Timeline.h"
#include <cmath>

using namespace DirectX;

Tweener::Tweener(float duration, std::function<void(float* tweenedVars)> onUpdate, std::vector<float> starts, std::vector<float> targets)
	: duration(duration), onUpdate(onUpdate), starts(starts), targets(targets), startTime(0)
{

}

Tweener::Tweener(float duration, std::function<void(float* tweenedVars)> onUpdate, XMFLOAT3 start3s, XMFLOAT4 start4s, XMFLOAT3 target3s, XMFLOAT4 target4s)
	: duration(duration), onUpdate(onUpdate), startTime(0)
{
	starts.push_back(start3s.x);
	starts.push_back(start3s.y);
	starts.push_back(start3s.z);

	targets.push_back(target3s.x);
	targets.push_back(target3s.y);
	targets.push_back(target3s.z);

	starts.push_back(start4s.x);
	starts.push_back(start4s.y);
	starts.push_back(start4s.z);
	starts.push_back(start4s.w);

	targets.push_back(target4s.x);
	targets.push_back(target4s.y);
	targets.push_back(target4s.z);
	targets.push_back(target4s.w);
}

Timeline::Timeline()
{
	time = 0;
}

Timeline::~Timeline()
{
}

void Timeline::AddAction(Action action)
{
	actions.push_back(action);
}

void Timeline::RunTweener(Tweener tweener)
{
	tweener.startTime = time;
	tweeners.push_back(tweener);
}

void Timeline::Tick(float time)
{
	this->time = time;

	// Start any actions that need to start
	std::vector<std::list<Action>::iterator> removeActions;
	for (std::list<Action>::iterator it = actions.begin(); it != actions.end(); it++)
	{
		auto action = *it;
		if (time > action.startTime)
		{
			action.callback();
			removeActions.push_back(it);
		}
	}
	for (const auto& it : removeActions)
	{
		actions.erase(it);
	}

	// Update all tweeners.
	std::vector<std::list<Tweener>::iterator> removeTweeners;
	for (std::list<Tweener>::iterator it = tweeners.begin(); it != tweeners.end(); it++)
	{
		auto tweener = *it;
		float progress = (time - tweener.startTime) / tweener.duration;

		auto numVars = tweener.starts.size();
		float* tweenedVars = new float[numVars];

		for (size_t i = 0; i < numVars; i++)
		{
			float start = tweener.starts[i], end = tweener.targets[i];
			// Lerp it
			tweenedVars[i] = start + progress * (end - start);
		}
		tweener.onUpdate(tweenedVars);
		if (progress > 1.0f) {
			removeTweeners.push_back(it);
		}
	}
	for (const auto& it : removeTweeners)
	{
		tweeners.erase(it);
	}
}
