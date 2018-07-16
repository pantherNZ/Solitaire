#pragma once

#ifndef __CARD_H__
#define __CARD_H__

#define VALIDATE(a) if (!a) return(false)

#include "sprite.h"

class CCard : public CSprite
{
	// Member Functions
	public:
		CCard();
		~CCard();
		bool Initialise(int _iSpriteResourceID, int _iMaskResourceID);

		void SetFlipped(bool _bFlipped);
		bool GetFlipped() const;

		void SetHighlight(bool _bHighlight);
		bool GetHighlight() const;

		int GetCardNum() const;
		bool IsRed() const;

		//void Process();

	protected:
	
	private:


	// class variables
	public:

	protected:
		bool m_bRed;
		bool m_bFlipped;
		bool m_bHighlight;
		int m_iCardNum;

	private:

};

#endif // __CARD_H__