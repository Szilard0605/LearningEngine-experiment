#pragma once

class Timestep
{
public:
	Timestep(float time)
		: m_time(time)
	{

	}

	float Seconds() { return m_time; }
	float Milliseconds() { return (m_time * 1000.0f); }

	operator float() const { return m_time; }

private:
	float m_time = 0.0f;
};

