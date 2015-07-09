#pragma once
#include <d2d1.h>
#include <wincodec.h>

class CD2D;

class CSprite
{
public:
	CSprite();
	~CSprite();

	VOID Free();
	BOOL Load(std::wstring filename, CD2D *renderer);
	D2D1_POINT_2F GetPosition();
	VOID SetPosition(float X, float Y);
	FLOAT GetWidth();
	FLOAT GetHeight();
	BOOL Render(CD2D *renderer);

private:
	IWICImagingFactory		*m_wicFactory;
	IWICBitmapDecoder		*m_wicDecoder;
	IWICBitmapFrameDecode	*m_wicFrame;
	IWICFormatConverter		*m_wicConverter;
	ID2D1Bitmap				*m_bitmap;
	D2D1_SIZE_F				m_fSize;
	D2D1_POINT_2F			m_fPos;
	D2D1_RECT_F				m_rcDest, m_rcSrc;
	FLOAT					m_fOpacity;
};

