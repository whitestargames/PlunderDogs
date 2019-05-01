#include "Timer.h"

Timer::Timer(float expirationTime, bool active)
	: m_expirationTime(expirationTime),
	m_elaspedTime(0),
	m_active(active)
{
}

bool Timer::isExpired() const
{
	return m_elaspedTime >= m_expirationTime;
}

float Timer::getElaspedTime() const
{
	return m_elaspedTime;
}

void Timer::update(float deltaTime)
{
	if (m_active)
	{
		m_elaspedTime += deltaTime;
	}
	
}

void Timer::reset()
{
	m_elaspedTime = 0;
}

void Timer::setActive(bool active)
{
	m_active = active;
}
