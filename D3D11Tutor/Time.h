#pragma once

class TimeClass {
public:
	explicit TimeClass();

	void Frame();
	float GetMyCurrentTime();
	float GetDeltaTime();
	void ResetTime();
	void SetTimeScale(float);

private:
	float getTimeSec();

private:
	float m_CurrentTime;
	float m_DeltaTime;
	float m_TimeScale;
};
