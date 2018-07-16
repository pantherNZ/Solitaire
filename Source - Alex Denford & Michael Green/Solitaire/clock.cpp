#include <windows.h>
#include <time.h>
#include "clock.h"

// constructor
CClock::CClock(): m_fTimeElapsed(0.0f), m_fDeltaTime(0.0f), m_fLastTime(0.0f), m_fCurrentTime(0.0f)
{

}

// destructor
CClock::~CClock()
{

}

bool CClock::Initialise()
{
	return(true);
}

void CClock::Process()
{
	m_fLastTime = m_fCurrentTime;
	m_fCurrentTime = static_cast<float>(clock());

	if (m_fLastTime == 0.0f)
	{
		m_fLastTime = m_fCurrentTime;
	}

	m_fDeltaTime = m_fCurrentTime - m_fLastTime;
	m_fTimeElapsed += m_fDeltaTime;
}

float CClock::GetDeltaTick() const
{
	return(m_fDeltaTime / 1000.0f);
}

float CClock::GetTime() const
{
	return(m_fTimeElapsed);
}

void CClock::Reset()
{
	m_fTimeElapsed = 0.0f;
	m_fLastTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_fDeltaTime = 0.0f;
}

void CClock::ResetTimeElapsed()
{
	m_fTimeElapsed = 0.0f;
}