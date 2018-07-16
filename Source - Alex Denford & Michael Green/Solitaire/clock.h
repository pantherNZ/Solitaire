#pragma once

#ifndef __CLOCK_H__
#define __CLOCK_H__

class CClock
{
	// Member Functions
	public:
		CClock();
		~CClock();
		bool Initialise();
		void Process();
		float GetDeltaTick() const;
		float GetTime() const;
		void Reset();
		void ResetTimeElapsed();

	protected:

	private:
		CClock(const CClock& _kr);
		CClock& operator= (const CClock& _kr);


	// class variables
	public:

	protected:
		float m_fTimeElapsed;
		float m_fDeltaTime;
		float m_fLastTime;
		float m_fCurrentTime;

	private:
};

#endif // __CLOCK_H__