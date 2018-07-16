#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stack>
#include <vector>

class CSprite;
class CCard;

class CLevel
{
	// class functions
	public:
		CLevel();
		virtual ~CLevel();
		virtual bool Initialise(int _iWidth, int _iHeight);
		virtual void Draw();
		virtual void Process(float _fDeltaTick);

		void SetMousePos(int _iX, int _iY);
		void SetMouseClick(bool _bClick);

		void ResetLevel();

	protected:	

	private:
		void ProcessMouseClick();
		void ResetHighlight();
		void CheckValidMove();
		void MoveCards(int _iNewPile);
		void DrawText();
		void CheckGameOver();

	// class members
	public:

	protected:
		int m_iWidth;
		int m_iHeight;

	private:
		CSprite* m_pEmptyAceSlots[4];
		CSprite* m_pDeckCard;
		CSprite* m_pEmptyDeckCard;
		CSprite* m_pEmptyPileSprite[7];		

		std::stack<CCard*> m_pDrawDeck;
		std::stack<CCard*> m_pWasteDeck;
		std::stack<CCard*> m_pAceSlots[4];

		std::vector<CCard*> m_pPiles[7];
		std::vector<CSprite*> m_pHiddenPileSprites[6];
		std::vector<CSprite*> m_pHighlight;

		int m_iMouseX;
		int m_iMouseY;
		bool m_bMouseClick;
		float m_fTime;

		CCard* m_pSelectedCard;
		int m_iSelectedCardLocation;

		int m_iMoves;
		CClock* m_pTimer;
};

#endif // __LEVEL_H__