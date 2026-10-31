// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PatternGeneratorView.h : interface of the CPatternGeneratorView class
//
//Planetary Motion SOURCE CODE

#pragma once

#include <cmath>

class CPatternGeneratorView : public CView
{
    CWinThread* m_pCurrentThread;
    CArray<CPoint, CPoint> m_PointArray;
protected:
    CPatternGeneratorView() noexcept;
    DECLARE_DYNCREATE(CPatternGeneratorView)

public:
    int m_iCounter;
    

    // Planet angles, measured in radians
    double m_earthAngle;
    double m_marsAngle;
    double m_mercuryAngle;
    double m_venusAngle;
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    CPatternGeneratorDoc* GetDocument() const;

    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

    virtual ~CPatternGeneratorView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnButtonStart();
    afx_msg void OnButtonStop();
    afx_msg void OnButtonResume();

    static UINT StartThread(LPVOID Param);
};

#ifndef _DEBUG
inline CPatternGeneratorDoc*
CPatternGeneratorView::GetDocument() const
{
    return reinterpret_cast<CPatternGeneratorDoc*>(m_pDocument);
}
#endif
