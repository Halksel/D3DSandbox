#include "Time.h"
#include <chrono>

TimeClass::TimeClass()
{
	ResetTime();
}

void TimeClass::Frame()
{
	float start = getTimeSec();
	m_DeltaTime = (start - m_CurrentTime) * m_TimeScale;
	m_CurrentTime = start;
}

float TimeClass::GetMyCurrentTime()
{
	return m_CurrentTime;
}

float TimeClass::GetDeltaTime()
{
	return m_DeltaTime;
}

void TimeClass::ResetTime()
{
	m_TimeScale = 1.0f;
	m_CurrentTime = getTimeSec();
	m_DeltaTime = 0.0f;
}

void TimeClass::SetTimeScale(float scale)
{
	m_TimeScale = scale;
}

float TimeClass::getTimeSec()
{
	return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count())/1000000000;
}
