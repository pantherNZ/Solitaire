#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include "clock.h"
#include "level.h"

#define VALIDATE(a) if (!a) return(false)

class CLevel;
class CBackBuffer;

class CGame
{
	// Class functions
	public:
		virtual ~CGame();
		virtual bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
		virtual void Draw();
		virtual void Process(float _fDeltaTick);

		void ExecuteOneFrame(); //(bool& _bGameActive);
		CBackBuffer* GetBackBuffer() const;
		CLevel* GetLevel()  const;
		HINSTANCE GetAppInstance() const;
		HWND GetWindow() const;
		CClock* GetTimer() const;

		// Singleton Methods
		static CGame& GetInstance();
		static void DestroyInstance();

		void ResetGame();

	protected:

	private:
		CGame();
		CGame(const CGame& _kr);
		CGame& operator= (const CGame& _kr);


	// class variables
	public:

	protected:
		// sub classes used in the game
		CClock* m_pClock;
		CLevel* m_pLevel;
		CBackBuffer* m_pBackBuffer;

		//bool m_bGameActive;

		//Application data
		HINSTANCE m_hApplicationInstance;
		HWND m_hMainWindow;

		// Singleton Instance
		static CGame* s_pGame;

	private:

};

#endif // __GAME_H__