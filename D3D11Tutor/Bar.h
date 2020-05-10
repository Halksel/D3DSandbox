#pragma once

#include "Object/object.h"

class Bar : public Rect {
public:
	explicit Bar();
	explicit Bar(std::string name, XMFLOAT3 p0 = XMFLOAT3(), XMFLOAT3 p1 = XMFLOAT3());
	explicit Bar(Bar const&) = default;
	explicit Bar(Bar&&) = default;
	Bar& operator=(Bar &&) = default;
	Bar& operator=(const Bar&);

	bool OnInitialize() override;
	void OnUpdate(InputClass& input) override;
	void OnCollisionAfter() override;
	void OnShutdown() override;
protected:


private:
	float m_Speed = 0.5f;
};
