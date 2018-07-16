// library includes

// local includes
#include "clock.h"
#include "backbuffer.h"

// self include
#include "game.h"

// initialise pointer of instance to 0
CGame* CGame::s_pGame = 0;

// constructor
CGame::CGame(): m_pLevel(0), m_pClock(0), m_hApplicationInstance(0), m_hMainWindow(0), m_pBackBuffer(0)
{

}

// destructor
CGame::~CGame()
{	
	delete m_pLevel;
	m_pLevel = nullptr;

	delete m_pBackBuffer;
	m_pBackBuffer = nullptr;

	delete m_pClock;
	m_pClock = nullptr;
}

bool CGame::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hApplicationInstance = _hInstance;
	m_hMainWindow = _hWnd;

	m_pClock = new CClock();
	VALIDATE(m_pClock ->Initialise());
	m_pClock->Process();

	m_pBackBuffer = new CBackBuffer();
	VALIDATE(m_pBackBuffer->Initialise(_hWnd, _iWidth, _iHeight));

	m_pLevel = new CLevel();
	VALIDATE(m_pLevel->Initialise(_iWidth, _iHeight)); 

	return(true);
}

void CGame::Draw()
{
	m_pBackBuffer->Clear();

	m_pLevel->Draw();

	m_pBackBuffer->Present();
}

void CGame::Process(float _fDeltaTick)
{
	m_pLevel->Process(_fDeltaTick);
}

void CGame::ExecuteOneFrame() //(bool& _bGameActive)
{
	float fDT = m_pClock->GetDeltaTick();

	Process(fDT);
	Draw();

	m_pClock->Process();
	Sleep(1);
}

CGame& CGame::GetInstance()
{
	if (s_pGame == nullptr)
	{
		s_pGame = new CGame();
	}

	return (*s_pGame);
}

void CGame::DestroyInstance()
{
	delete s_pGame;
	s_pGame = nullptr;
}

CBackBuffer* CGame::GetBackBuffer() const
{
	return (m_pBackBuffer);
}

CLevel* CGame::GetLevel() const
{
	return(m_pLevel);
}

HINSTANCE CGame::GetAppInstance() const
{
	return (m_hApplicationInstance);
}

HWND CGame::GetWindow() const
{
	return (m_hMainWindow);
}

void CGame::ResetGame()
{
	m_pLevel->ResetLevel();
}

CClock* CGame::GetTimer() const
{
	return(m_pClock);
}