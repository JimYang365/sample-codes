#include "stdafx.h"
#include "GifImage.h"


CGifImage::CGifImage(LPCWSTR lpszFileName, int nPauseMultiple) : Gdiplus::Image(lpszFileName), m_nPauseMultiple(nPauseMultiple)
{
    TestForAnimatedGIF();
}

CGifImage::CGifImage(LPBYTE pBuf, size_t nSize, int nPauseMultiple) : m_nPauseMultiple(nPauseMultiple)
{
    LoadFromBuffer(pBuf, nSize);

    TestForAnimatedGIF();
}

CGifImage::~CGifImage()
{
}

bool CGifImage::LoadFromBuffer(BYTE* pBuff, int nSize)
{
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
    if (hGlobal)
    {
        void* pData = GlobalLock(hGlobal);
        if (pData)
            memcpy(pData, pBuff, nSize);

        GlobalUnlock(hGlobal);

        IStream* pStream;
        CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);

        nativeImage = NULL;
        DllExports::GdipLoadImageFromStreamICM(pStream, &nativeImage);
    }

    return true;
}

bool CGifImage::TestForAnimatedGIF()
{
    UINT count = 0;
    count = GetFrameDimensionsCount();
    ATLASSERT(count);
    GUID* pDimensionIDs = new GUID[count];

    // Get the list of frame dimensions from the Image object.
    GetFrameDimensionsList(pDimensionIDs, count);

    // Get the number of frames in the first dimension.
    m_nFrameCount = GetFrameCount(&pDimensionIDs[0]);

    // Assume that the image has a property item of type PropertyItemEquipMake.
    // Get the size of that property item.
    int nSize = GetPropertyItemSize(PropertyTagFrameDelay);
    if (nSize > 0)
    {
        // Allocate a buffer to receive the property item.
        m_pPropertyItem = (PropertyItem*)malloc(nSize);
        GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
    }

    delete  pDimensionIDs;

    return m_nFrameCount > 1;
}

void CGifImage::Draw(HDC hDC, int x, int y, UINT cx, UINT cy)
{
    Graphics graphics(hDC);
    graphics.DrawImage(this, x, y, cx, cy);
}

UINT CGifImage::NextFrame()
{
    if (++m_nFramePosition >= m_nFrameCount)
        m_nFramePosition = 0;

    static const GUID   pageGuid = FrameDimensionTime;
    SelectActiveFrame(&pageGuid, m_nFramePosition);

    UINT nPause = 100;
    if (m_pPropertyItem)
    {
        nPause = ((UINT*)m_pPropertyItem->value)[m_nFramePosition] * m_nPauseMultiple;
        if (nPause <= 100)
            nPause = 100;
    }
    return nPause;
}
