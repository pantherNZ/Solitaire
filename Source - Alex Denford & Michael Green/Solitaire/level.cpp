// library includes
#include <algorithm>
#include <vector>
#include <math.h>
#include <sstream>
#include <string>

// local includes
#include "sprite.h"
#include "resources.h"
#include "card.h"
#include "game.h"
#include "backbuffer.h"

// self include
#include "level.h"

// constructor
CLevel::CLevel()
{

}

// destructor
CLevel::~CLevel()
{
	for(int i = 0; i < 4; ++i)
	{
		delete m_pEmptyAceSlots[i];
		m_pEmptyAceSlots[i] = nullptr;

		while(m_pAceSlots[i].empty() == false)
		{
			delete m_pAceSlots[i].top();
			m_pAceSlots[i].pop();
		}
	}

	delete m_pDeckCard;
	m_pDeckCard = nullptr;

	while(m_pDrawDeck.empty() == false)
	{
		delete m_pDrawDeck.top();
		m_pDrawDeck.pop();
	}
	
	while(m_pWasteDeck.empty() == false)
	{
		delete m_pWasteDeck.top();
		m_pWasteDeck.pop();
	}

	if(m_pSelectedCard != nullptr)
	{
		m_pSelectedCard = nullptr;
	}

	for(int i = 0; i < 7; ++i)
	{		
		if(i < 6) 
		{
			m_pHiddenPileSprites[i].clear();
		}

		m_pPiles[i].clear();
	}

	m_pHighlight.clear();
}

bool CLevel::Initialise(int _iWidth, int _iHeight)
{
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	CGame::GetInstance().GetTimer()->ResetTimeElapsed();

	m_bMouseClick = false;
	m_iMouseX = 0;
	m_iMouseY = 0;
	m_pSelectedCard = nullptr;
	m_iSelectedCardLocation = 0; 
	m_iMoves = 0;
	m_fTime = 0.0f;

	// test setup ---------------------- 

	/*
	if(true)
	{
		for(int j = 0; j < 4; ++j)
		{
			m_pEmptyAceSlots[j] = new CSprite;
			m_pEmptyAceSlots[j]->Initialise(IDB_CARD_ACESLOT_DIAMONDS + j, IDB_CARD_MASK);
			m_pEmptyAceSlots[j]->TranslateAbsolute(_iWidth - 345 + j * 85, 60);
		}

		for(int i = 0; i < 51; ++i)
		{
			
			if(i < 13)
			{
				m_pAceSlots[0].push(new CCard);
				m_pAceSlots[0].top()->Initialise(101 + i, IDB_CARD_MASK);
				m_pAceSlots[0].top()->TranslateAbsolute(m_pEmptyAceSlots[0]->GetX(), m_pEmptyAceSlots[0]->GetY());
			}
			else if(i < 26)
			{
				m_pAceSlots[1].push(new CCard);
				m_pAceSlots[1].top()->Initialise(101 + i, IDB_CARD_MASK);
				m_pAceSlots[1].top()->TranslateAbsolute(m_pEmptyAceSlots[1]->GetX(), m_pEmptyAceSlots[1]->GetY());
			}
			else if(i < 39)
			{
				m_pAceSlots[2].push(new CCard);
				m_pAceSlots[2].top()->Initialise(101 + i, IDB_CARD_MASK);
				m_pAceSlots[2].top()->TranslateAbsolute(m_pEmptyAceSlots[2]->GetX(), m_pEmptyAceSlots[2]->GetY());
			}
			else
			{
				m_pAceSlots[3].push(new CCard);
				m_pAceSlots[3].top()->Initialise(101 + i, IDB_CARD_MASK);
				m_pAceSlots[3].top()->TranslateAbsolute(m_pEmptyAceSlots[3]->GetX(), m_pEmptyAceSlots[3]->GetY());
			}	
		}

		m_pPiles[6].push_back(new CCard);
		m_pPiles[6].back()->Initialise(152, IDB_CARD_MASK);
		m_pPiles[6].back()->TranslateAbsolute(100, 200);

		// top of deck
		m_pDeckCard = new CSprite;
		m_pDeckCard->Initialise(IDB_CARD_BACK, IDB_CARD_MASK);
		m_pDeckCard->TranslateAbsolute(50, 60);

		// setup sprites for hidden cards
		for(int iStep = 0; iStep < 6; ++iStep)
		{
			for(int k = iStep; k < 6; ++k)
			{
				m_pHiddenPileSprites[k].push_back(new CCard);	
				m_pHiddenPileSprites[k].back()->Initialise(IDB_CARD_BACK, IDB_CARD_MASK);
				m_pHiddenPileSprites[k].back()->TranslateAbsolute(185 + k * 85, 200 + iStep * 20);
			}
		}

		m_pEmptyDeckCard = new CSprite;
		m_pEmptyDeckCard->Initialise(IDB_DECK_REFRESH, IDB_DECK_REFRESH_MASK);
		m_pEmptyDeckCard->TranslateAbsolute(50, 60);

		// sprites for if any of the piles are empty
		for(int i = 0; i < 7; ++i)
		{
			m_pEmptyPileSprite[i] = new CCard;
			m_pEmptyPileSprite[i]->Initialise(IDB_CARD_SLOT, IDB_CARD_SLOT_MASK);
			m_pEmptyPileSprite[i]->TranslateAbsolute(100 + i * 85, 200);
		}

		return(true);
	}
	*/
	// test setup end -------------------





	// setup ace slots 

	for(int i = 0; i < 4; ++i)
	{
		m_pEmptyAceSlots[i] = new CSprite;
		m_pEmptyAceSlots[i]->Initialise(IDB_CARD_ACESLOT_DIAMONDS + i, IDB_CARD_MASK);
		m_pEmptyAceSlots[i]->TranslateAbsolute(_iWidth - 345 + i * 85, 60);
	}

	// create vector for deck of cards (for shuffling)
	std::vector<CCard*> pTempDeck(52);

	for(int i = 0; i < 52; ++i)
	{
		pTempDeck[i] = new CCard;
		pTempDeck[i]->Initialise(101 + i, IDB_CARD_MASK);
		pTempDeck[i]->TranslateAbsolute(50, 60);
	}

	// randomise (shuffle)
	std::random_shuffle(pTempDeck.begin(), pTempDeck.end());

	// move temp vector to the permeneant stack
	for(int i = 0; i < 52; ++i)
	{
		m_pDrawDeck.push(pTempDeck[i]);
	}
	
	// top of deck
	m_pDeckCard = new CSprite;
	m_pDeckCard->Initialise(IDB_CARD_BACK, IDB_CARD_MASK);
	m_pDeckCard->TranslateAbsolute(50, 60);

	// deal cards from deck onto each pile  (1, 2, 3, 4, 5, 6, 7)
	for(int iStep = 0; iStep < 7; ++iStep)
	{
		for(int i = iStep; i < 7; ++i)
		{
			CCard* pCard = m_pDrawDeck.top();
			m_pDrawDeck.pop();

			pCard->TranslateAbsolute(100 + i * 85, 200 + iStep * 20);
				
			m_pPiles[i].push_back(pCard);
		}

		m_pPiles[iStep].back()->SetFlipped(true);
	}

	// setup sprites for hidden cards
	for(int iStep = 0; iStep < 6; ++iStep)
	{
		for(int i = iStep; i < 6; ++i)
		{
			m_pHiddenPileSprites[i].push_back(new CCard);	
			m_pHiddenPileSprites[i].back()->Initialise(IDB_CARD_BACK, IDB_CARD_MASK);
			m_pHiddenPileSprites[i].back()->TranslateAbsolute(185 + i * 85, 200 + iStep * 20);
		}
	}

	// sprites for if any of the piles are empty
	for(int i = 0; i < 7; ++i)
	{
		m_pEmptyPileSprite[i] = new CCard;
		m_pEmptyPileSprite[i]->Initialise(IDB_CARD_SLOT, IDB_CARD_SLOT_MASK);
		m_pEmptyPileSprite[i]->TranslateAbsolute(100 + i * 85, 200);
	}

	m_pEmptyDeckCard = new CSprite;
	m_pEmptyDeckCard->Initialise(IDB_DECK_REFRESH, IDB_DECK_REFRESH_MASK);
	m_pEmptyDeckCard->TranslateAbsolute(50, 60);

	return(true);
}

void CLevel::Draw()
{
	// draw each ace slot
	for(int i = 0; i < 4; ++i)
	{
		if(m_pAceSlots[i].empty())
		{
			m_pEmptyAceSlots[i]->Draw();
		}
		else
		{
			m_pAceSlots[i].top()->Draw();
		}
	}

	// draw the top of the deck
	if(!m_pDrawDeck.empty())
	{
		m_pDeckCard->Draw();
	}
	else
	{
		m_pEmptyDeckCard->Draw();
	}

	bool bPiles = false;

	// draw the cards for each of the piles
	for(int i = 0; i < 7; ++i)
	{
		if(!m_pPiles[i].empty())
		{
			for(unsigned int j = 0; j < m_pPiles[i].size(); ++j)
			{
				if(m_pPiles[i][j]->GetFlipped())
				{
					m_pPiles[i][j]->Draw();
					
					if(m_pPiles[i][j]->GetHighlight())
					{
						bPiles = true;

						int iOffset = m_pPiles[i].size() - m_pHighlight.size();
						m_pHighlight[(j - iOffset)]->Draw();
					}
				}
				else
				{
					m_pHiddenPileSprites[i - 1][j]->Draw();
				}
			}
		}
		else
		{
			m_pEmptyPileSprite[i]->Draw();
		}	
	}

	if(!bPiles && m_pHighlight.empty() == false)
	{
		m_pHighlight[0]->Draw();
	}

	if(!m_pWasteDeck.empty())
	{
		m_pWasteDeck.top()->Draw();
	}

	DrawText();
}

void CLevel::Process(float _fDeltaTick)
{
	// Do stuff if they click the mouse!
	ProcessMouseClick();

	for(int i = 0; i < 7; ++i)
	{
		if(m_pPiles[i].empty() == false)
		{
			if(!m_pPiles[i].back()->GetFlipped())
			{
				m_pPiles[i].back()->SetFlipped(true);
			}
		}
	}

	if(CGame::GetInstance().GetTimer()->GetTime() > m_fTime && m_fTime != 0.0f)
	{
		std::stringstream ss;

		ss << "Well done, you won! \nMoves: " << m_iMoves << "\nTime: " << int(CGame::GetInstance().GetTimer()->GetTime() / 1000.0f);
		ss << "\n\nWould you like to play again?";
		std::string s = ss.str();

		std::wstring stemp = std::wstring(s.begin(), s.end());

		LPCWSTR resultText = stemp.c_str();

		if(MessageBox(NULL, resultText, L"You won!", MB_YESNO) == IDNO)
		{
			PostQuitMessage(0);
		}
		else
		{
			ResetLevel();
		}
	}
}

void CLevel::ProcessMouseClick()
{
	if(m_bMouseClick)
	{
		if(m_pSelectedCard == nullptr)
		{
			m_bMouseClick = false;

			// --------- check for clicking on the deck ------------
			int iX = m_pDeckCard->GetX();
			int iY = m_pDeckCard->GetY();

			if((m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36) &&
			   (m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48))
			{	
				ResetHighlight();

				// check if deck is empty
				if(m_pDrawDeck.empty())
				{
					if(m_pWasteDeck.empty())
					{
						return;
					}

					while(!m_pWasteDeck.empty())
					{
						CCard* pTemp = m_pWasteDeck.top();
						pTemp->TranslateRelative(-100, 0);
						m_pWasteDeck.pop();
						m_pDrawDeck.push(pTemp);
					}
				}

				// move card from deck to "Wastedeck"
				CCard* pTemp = m_pDrawDeck.top();
				m_pDrawDeck.pop();

				pTemp->TranslateRelative(100, 0);

				m_pWasteDeck.push(pTemp);

				++m_iMoves;

				return;
			}

			// --------- check for clicking on any of the piles ------------
			for(int i = 0; i < 7; ++i)
			{
				for(int j = m_pPiles[i].size() - 1; j >= 0; --j)
				{
					iX = m_pPiles[i][j]->GetX();
					iY = m_pPiles[i][j]->GetY();

					if((m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36) &&
					   (m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48))
					{
						if(m_pPiles[i][j]->GetFlipped())
						{
							ResetHighlight();

							m_iSelectedCardLocation = i;
							m_pSelectedCard = m_pPiles[i][j];

							for(unsigned int k = j; k < m_pPiles[i].size(); ++k)
							{
								m_pPiles[i][k]->SetHighlight(true);

								m_pHighlight.push_back(new CSprite);
								m_pHighlight.back()->Initialise(IDB_CARD_HIGHLIGHT, IDB_CARD_HIGHLIGHT_MASK);
								m_pHighlight.back()->TranslateAbsolute(m_pPiles[i][k]->GetX(), m_pPiles[i][k]->GetY());
							}
						}

						return;
					}
				}
			}

			// --------- check for clicking on the waste pile ------------
			if(!m_pWasteDeck.empty())
			{
				int iX = m_pWasteDeck.top()->GetX();
				int iY = m_pWasteDeck.top()->GetY();

				if((m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36) &&
				   (m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48))
				{	
					ResetHighlight();

					m_iSelectedCardLocation = -1;
					m_pSelectedCard = m_pWasteDeck.top();

					m_pHighlight.push_back(new CSprite);
					m_pHighlight.back()->Initialise(IDB_CARD_HIGHLIGHT, IDB_CARD_HIGHLIGHT_MASK);
					m_pHighlight.back()->TranslateAbsolute(iX, iY);

					return;
				}
			}

			// --------- check for clicking on the aces pile --------------
			for(int i = 0; i < 4; ++i)
			{
				if(m_pAceSlots[i].empty())
				{
					break;
				}
				else
				{
					// check if they have clicked on it
					int iX = m_pAceSlots[i].top()->GetX();
					int iY = m_pAceSlots[i].top()->GetY();

					if((m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36) &&
					   (m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48))
					{
						ResetHighlight();

						m_iSelectedCardLocation = -2 - i; // -2 -> -5
						m_pSelectedCard = m_pAceSlots[i].top();

						m_pHighlight.push_back(new CSprite);
						m_pHighlight.back()->Initialise(IDB_CARD_HIGHLIGHT, IDB_CARD_HIGHLIGHT_MASK);
						m_pHighlight.back()->TranslateAbsolute(iX, iY);

						return;
					}
				}
			}

			ResetHighlight();
		}
		else
		{
			m_bMouseClick = false;

			CheckValidMove();
		}
	}
}

void CLevel::CheckValidMove()
{
	// check if the player is trying to move to the aces pile
	for (int i = 0; i < 4; ++i)
	{
		int iX = m_pEmptyAceSlots[i]->GetX();
		int iY = m_pEmptyAceSlots[i]->GetY();

		int iX2 = m_pSelectedCard->GetX();
		int iY2 = m_pSelectedCard->GetY();

		// if clicking on the pile OR the player has double clicked on a card
		if( (m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36 && m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48) ||
			(m_iMouseX <= iX2 + 36 && m_iMouseX >= iX2 - 36 && m_iMouseY <= iY2 + 48 && m_iMouseY >= iY2 - 48) )
		{	
			if(m_pHighlight.size() == 1)
			{
				int iNum = 0;

				if (m_pAceSlots[i].empty())
				{
					iNum = i * 13;
				}
				else
				{
					iNum = m_pAceSlots[i].top()->GetCardNum();
				}

				if((iNum + 1) == m_pSelectedCard->GetCardNum())
				{
					m_pSelectedCard->TranslateAbsolute(m_pEmptyAceSlots[i]->GetX(), m_pEmptyAceSlots[i]->GetY());
					++m_iMoves;

					if(m_iSelectedCardLocation == -1)
					{
						m_pWasteDeck.pop();
						m_pAceSlots[i].push(m_pSelectedCard);
					}
					else if(m_iSelectedCardLocation >= 0)
					{
						m_pPiles[m_iSelectedCardLocation].pop_back();
						m_pAceSlots[i].push(m_pSelectedCard);
					}

					ResetHighlight();
					CheckGameOver();

					return;
				}
			}	

			// if end of loop and they havn't clicked on a pile or havn't selected a valid card to be double click moved
			if(i == 3)
			{
				// reset and return
				ResetHighlight();
				return;
			}
		}	
	}	

	// check if the player is trying to move to a pile
	for (int i = 0; i < 7; ++i)
	{
		if(!m_pPiles[i].empty())
		{
			int iX = m_pPiles[i].back()->GetX();
			int iY = m_pPiles[i].back()->GetY();

			if((m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36) &&
			   (m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48))
			{	
				// check colour is valid
				if((m_pPiles[i].back()->IsRed() && !m_pSelectedCard->IsRed()) || 
				   (!m_pPiles[i].back()->IsRed() && m_pSelectedCard->IsRed()))
				{
					int iGap = (m_pPiles[i].back()->GetCardNum() - 1) - m_pSelectedCard->GetCardNum();

					// check number is valid
					if(iGap % 13 == 0)
					{
						MoveCards(i);
					}
				}

				ResetHighlight();
				return;
			}
		}
		// check if the player is trying to move a king from a pile or from the deck to a blank spot
		else
		{
			int iNum = m_pSelectedCard->GetCardNum();

			// check if the card is a king
			if(iNum == 13 || iNum == 26 || iNum == 39 || iNum == 52)
			{
				int iX = m_pEmptyPileSprite[i]->GetX();
				int iY = m_pEmptyPileSprite[i]->GetY();

				// check for clicking on a blank pile
				if((m_iMouseX <= iX + 36 && m_iMouseX >= iX - 36) &&
				   (m_iMouseY <= iY + 48 && m_iMouseY >= iY - 48))
				{	
					MoveCards(i);
				}
			}
		}
	}

	ResetHighlight();
}

void CLevel::CheckGameOver()
{
	// check if the game is over 
	if(!m_pAceSlots[0].empty() && !m_pAceSlots[1].empty() && !m_pAceSlots[2].empty() && !m_pAceSlots[3].empty())
	{
		if(m_pAceSlots[0].top()->GetCardNum() == 13 && m_pAceSlots[1].top()->GetCardNum() == 26 && 
			m_pAceSlots[2].top()->GetCardNum() == 39 && m_pAceSlots[3].top()->GetCardNum() == 52)
		{
			m_fTime = CGame::GetInstance().GetTimer()->GetTime() + 5;
		}
	}	
}

void CLevel::MoveCards(int _iNewPile)
{
	++m_iMoves;

	std::stack<CCard*> pTempStack;
	CCard* pCurrentCard = nullptr;

	while(pCurrentCard != m_pSelectedCard)
	{		
		if(m_iSelectedCardLocation == -1)
		{
			pCurrentCard = m_pWasteDeck.top();
			pTempStack.push(pCurrentCard);
			m_pWasteDeck.pop();	
		}
		else if(m_iSelectedCardLocation <= -2)
		{
			int iIndex = -(m_iSelectedCardLocation + 2);

			pCurrentCard = m_pAceSlots[iIndex].top();
			pTempStack.push(pCurrentCard);
			m_pAceSlots[iIndex].pop();	
		}
		else
		{
			pCurrentCard = m_pPiles[m_iSelectedCardLocation].back();
			pTempStack.push(pCurrentCard);
			m_pPiles[m_iSelectedCardLocation].pop_back();	
		}					
	}

	while(pTempStack.empty() == false)
	{
		if(m_pPiles[_iNewPile].empty() == true)
		{
			pTempStack.top()->TranslateAbsolute(m_pEmptyPileSprite[_iNewPile]->GetX(), m_pEmptyPileSprite[_iNewPile]->GetY());
		}
		else
		{
			pTempStack.top()->TranslateAbsolute(m_pPiles[_iNewPile].back()->GetX(), m_pPiles[_iNewPile].back()->GetY() + 20);
		}

		m_pPiles[_iNewPile].push_back(pTempStack.top());
		pTempStack.pop();
	}
}

void CLevel::SetMousePos(int _iX, int _iY)
{
	m_iMouseX = _iX;
	m_iMouseY = _iY;
}

void CLevel::SetMouseClick(bool _bClick)
{
	m_bMouseClick = _bClick;
}

void CLevel::ResetHighlight()
{
	m_pHighlight.clear();

	for(int i = 0; i < 7; ++i)
	{
		for(unsigned int j = 0; j < m_pPiles[i].size(); ++j)
		{
			m_pPiles[i][j]->SetHighlight(false);
		}
	}

	m_pSelectedCard = nullptr;
}

void CLevel::ResetLevel()
{
	for(int i = 0; i < 4; ++i)
	{
		delete m_pEmptyAceSlots[i];
		m_pEmptyAceSlots[i] = nullptr;

		while(m_pAceSlots[i].empty() == false)
		{
			delete m_pAceSlots[i].top();
			m_pAceSlots[i].pop();
		}
	}

	delete m_pDeckCard;
	m_pDeckCard = nullptr;

	while(m_pDrawDeck.empty() == false)
	{
		delete m_pDrawDeck.top();
		m_pDrawDeck.pop();
	}
	
	while(m_pWasteDeck.empty() == false)
	{
		delete m_pWasteDeck.top();
		m_pWasteDeck.pop();
	}

	if(m_pSelectedCard != nullptr)
	{
		m_pSelectedCard = nullptr;
	}

	for(int i = 0; i < 7; ++i)
	{		
		if(i < 6) 
		{
			m_pHiddenPileSprites[i].clear();
		}

		m_pPiles[i].clear();
	}

	m_pHighlight.clear();

	Initialise(m_iWidth, m_iHeight);
}

void CLevel::DrawText()
{
	HDC hdc = CGame::GetInstance().GetBackBuffer()->GetBFDC();

	std::stringstream ss;

	ss << "Moves: " << m_iMoves;

	std::string s = ss.str();
	int iSize = s.size();

	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR resultText = stemp.c_str();

	TextOut(hdc, 25, (m_iHeight - 80), resultText, iSize);

	std::stringstream ss2;

	ss2 << "Time: " << int(CGame::GetInstance().GetTimer()->GetTime() / 1000.0f);

	std::string s2 = ss2.str();
	iSize = s2.size();

	stemp = std::wstring(s2.begin(), s2.end());
	resultText = stemp.c_str();

	TextOut(hdc, (m_iWidth - 10 - (iSize * 9)), (m_iHeight - 80), resultText, iSize);
}