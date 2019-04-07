#include "Timer.h"

Timer::Timer(float expirationTime)
	: m_expirationTime(expirationTime),
	m_elaspedTime(0)
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
	m_elaspedTime += deltaTime;
}

void Timer::reset()
{
	m_elaspedTime = 0;
}
