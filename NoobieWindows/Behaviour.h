#pragma once


typedef unsigned int BehaviourID;


class Behaviour
{
private:
	static BehaviourID nextID;
	BehaviourID id;
public:
	inline Behaviour() : id(id) {}
	virtual ~Behaviour();

	virtual void Start() = 0;
	virtual void Update(float gt) = 0;
	virtual void Draw(float gt) = 0;

	BehaviourID GetID() { return id; }
};

