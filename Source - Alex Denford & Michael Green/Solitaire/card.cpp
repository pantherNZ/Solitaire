// library includes:

// local includes:

// self include:
#include "card.h"

// constructor
CCard::CCard() : m_bFlipped(false), m_bRed(false), m_bHighlight(false), m_iCardNum(0)
{

}

// destructor
CCard::~CCard()
{

}

bool CCard::Initialise(int _iSpriteResourceID, int _iMaskResourceID)
{
	VALIDATE(CSprite::Initialise(_iSpriteResourceID, _iMaskResourceID));

	if( (_iSpriteResourceID > 100 && _iSpriteResourceID < 114) || (_iSpriteResourceID > 126 && _iSpriteResourceID < 140) )
	{
		m_bRed = true;
	}
	else
	{
		m_bRed = false;
	}

	m_iCardNum = _iSpriteResourceID - 100;

	return(true);
}

void CCard::SetFlipped(bool _bFlipped)
{
	m_bFlipped = _bFlipped;
}

bool CCard::GetFlipped() const
{
	return(m_bFlipped);
}

void CCard::SetHighlight(bool _bHighlight)
{
	m_bHighlight = _bHighlight;
}

bool CCard::GetHighlight() const
{
	return(m_bHighlight);
}

int CCard::GetCardNum() const
{
	return(m_iCardNum);
}

bool CCard::IsRed() const
{
	return(m_bRed);
}