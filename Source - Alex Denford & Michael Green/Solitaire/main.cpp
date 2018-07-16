// lbirary includes
#include <Windows.h>
#include <WindowsX.h>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <string>

// local includes
#include "game.h"
#include "resources.h"

#define WINDOW_CLASS_NAME L"Solitaire - Alex Denford & Michael Green"

LRESULT CALLBACK WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uiMsg)
	{
		case WM_MOUSEMOVE:
		{
			int iMouseX = LOWORD(_lParam);
			int iMouseY = HIWORD(_lParam);

			CGame::GetInstance().GetLevel()->SetMousePos(iMouseX, iMouseY);

			return(0);
		}
		break;

		case WM_LBUTTONDOWN:
		{
			CGame::GetInstance().GetLevel()->SetMouseClick(true);
		}
		break;

		case WM_LBUTTONUP:
		{
			CGame::GetInstance().GetLevel()->SetMouseClick(false);
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}
		break;

		case WM_COMMAND:
			{
				switch(LOWORD(_wParam))
				{
				case ID_FILE_NEWGAME:
					{
						CGame::GetInstance().ResetGame();
					}
					break;

				case ID_FILE_QUIT:
					{
						PostQuitMessage(0);
					}
					break;

				case ID_HELP_HOWTOPLAY:
					{
						std::stringstream ss;

						ss << "How to Play Solitaire: \n\nYou start with 7 piles across the game, these are called the tableau.";
						ss << "There is the deck in the top left, cards can be taken off of the deck (1 at a time) to the deck pile, which can then be used within the game.\n";
						ss << "\nThere are 4 foundation piles at the top right of the screen, for each suit. These are built in ascending order (Ace - King). Once all these piles are complete, you finish the game.\n\n";

						ss << "Players make moves in several different ways: (A move is only valid if the move is in descending order eg. Moving a jack onto a queen is valid, and the cards must alternate colours)\n";
						ss << "  - You can move from the deck pile to the tableau\n";
						ss << "  - You can move cards around within the tableau\n";
						ss << "  - You can move cards from the foundations to the tableau and vice versa.\n";
						ss << "  - You can move Kings from the deck pile or from another pile onto an empty slot in the tableau.";

						std::string s = ss.str();
						int iSize = s.size();

						std::wstring stemp = std::wstring(s.begin(), s.end());
						LPCWSTR resultText = stemp.c_str();

						MessageBox(NULL, resultText, L"How-To-Play", MB_OK);
					}
					break;

				case ID_HELP_ABOUT:
					{
						MessageBox(NULL, L"Solitaire - Media Design School - GD1P02\n\nMade by Michael Green and Alex Denford\n18/09/13\n\nCopyright (C) Media Design School", L"About", MB_OK);
					}
					break;
				}
			}
			break;

		default: break;

	}

	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

HWND CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, LPCWSTR _pcTitle)
{
	WNDCLASSEX winclass;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = static_cast<HBRUSH> (GetStockObject(NULL_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	HMENU hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU1));


	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	HWND hwnd;
	hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, _pcTitle, 
						WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, _iWidth, _iHeight,
						NULL, NULL, _hInstance, NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	SetMenu(hwnd, hMenu);

	return (hwnd);
}

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	bool bGameActive = true;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	srand(static_cast<unsigned int>(time(0)));

	const int kiWidth = 700;
	const int kiHeight = 700;

	HWND hwnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, WINDOW_CLASS_NAME);

	//while(bGameActive)
	//{
		// Get an instance of the CGame class
		CGame& rGame = CGame::GetInstance();

		if (!rGame.Initialise(_hInstance, hwnd, kiWidth, kiHeight))
		{
			// Failed
			return (0);
		}

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				// Handle Windows messages if there are any
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// any game code stuff done in this block
				rGame.ExecuteOneFrame(); //(bGameActive);	
			}
		}

		// scoring message box, post game stuff done here

		rGame.DestroyInstance();
	//}

	return (static_cast<int>(msg.wParam));
}