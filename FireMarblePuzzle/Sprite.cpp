#include "stdafx.h"
#include "Sprite.h"
#include "D2D.h"

#pragma comment(lib, "windowscodecs.lib")

CSprite::CSprite() : m_wicFactory(nullptr), 
m_wicDecoder(nullptr), m_wicFrame(nullptr), 
m_wicConverter(nullptr), m_bitmap(nullptr),
m_fOpacity(1.0f), m_fSize(D2D1::SizeF()), m_fPos(D2D1_POINT_2F())
{
	ZeroMemory(&m_rcDest, sizeof(D2D1_RECT_F));
	ZeroMemory(&m_rcSrc, sizeof(D2D1_RECT_F));
}

CSprite::~CSprite()
{
	Free();
}

VOID CSprite::Free()
{
	SAFE_RELEASE(m_bitmap);
	SAFE_RELEASE(m_wicConverter);
	SAFE_RELEASE(m_wicFrame);
	SAFE_RELEASE(m_wicDecoder);
	SAFE_RELEASE(m_wicFactory);
}

BOOL CSprite::Load(std::wstring filename, CD2D *renderer)
{
	// To avoid memory leaks
	Free();

	// Initialize COM objects
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&m_wicFactory));
	if (!SUCCEEDED(hr) || m_wicFactory == nullptr) return FALSE;

	hr = m_wicFactory->CreateDecoderFromFilename(filename.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_wicDecoder);
	if (!SUCCEEDED(hr) || m_wicDecoder == nullptr) return FALSE;

	hr = m_wicDecoder->GetFrame(0, &m_wicFrame);
	if (!SUCCEEDED(hr) || m_wicFrame == nullptr) return FALSE;

	hr = m_wicFactory->CreateFormatConverter(&m_wicConverter);
	if (!SUCCEEDED(hr) || m_wicConverter == nullptr) return FALSE;

	hr = m_wicConverter->Initialize(m_wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
	if (!SUCCEEDED(hr)) return FALSE;

	// Create a D2D-compatible bitmap
	ID2D1RenderTarget *rt = renderer->GetRenderTarget();
	if (rt == nullptr) return FALSE;
	hr = rt->CreateBitmapFromWicBitmap(m_wicConverter, nullptr, &m_bitmap);
	if (!SUCCEEDED(hr) || m_bitmap == nullptr) return FALSE;

	m_fSize = m_bitmap->GetSize();
	m_rcSrc = D2D1::RectF(0.0f, 0.0f, m_fSize.width, m_fSize.height);
	m_rcDest = D2D1::RectF(0.0f, 0.0f, m_fSize.width, m_fSize.height);

	return TRUE;
}

D2D1_POINT_2F CSprite::GetPosition()
{
	return m_fPos;
}

VOID CSprite::SetPosition(float X, float Y)
{
	m_fPos.x = X;
	m_fPos.y = Y;
	m_rcDest = D2D1::RectF(m_fPos.x, m_fPos.y, m_fPos.x + m_fSize.width, m_fPos.y + m_fSize.height);
}

FLOAT CSprite::GetWidth() {
	return m_fSize.width;
}

FLOAT CSprite::GetHeight() {
	return m_fSize.height;
}

BOOL CSprite::Render(CD2D *renderer)
{
	ID2D1RenderTarget *rt = renderer->GetRenderTarget();
	if (rt == nullptr || m_bitmap == nullptr) return FALSE;
	rt->DrawBitmap(m_bitmap, m_rcDest, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, m_rcSrc);
	return TRUE;
}