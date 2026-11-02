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

// PatternGeneratorView.cpp : implementation of the CPatternGeneratorView class
//
//Planetary Motion SOURCE CODE

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PatternGenerator.h"
#endif

#include "PatternGeneratorDoc.h"
#include "PatternGeneratorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatternGeneratorView

IMPLEMENT_DYNCREATE(CPatternGeneratorView, CView)

BEGIN_MESSAGE_MAP(CPatternGeneratorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPatternGeneratorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTON_START, &CPatternGeneratorView::OnButtonStart)
	ON_COMMAND(ID_BUTTON_STOP, &CPatternGeneratorView::OnButtonStop)
	ON_COMMAND(ID_BUTTON_RESUME, &CPatternGeneratorView::OnButtonResume)
END_MESSAGE_MAP()

// CPatternGeneratorView construction/destruction

CPatternGeneratorView::CPatternGeneratorView() noexcept
{
	// planets start at different angles
	m_mercuryAngle = 0.0;
	m_venusAngle = 1.2;
	m_earthAngle = 2.5;
	m_marsAngle = 3.8;
	m_saturnAngle = 5.0;
}

CPatternGeneratorView::~CPatternGeneratorView()
{
}

BOOL CPatternGeneratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPatternGeneratorView drawing

void CPatternGeneratorView::OnDraw(CDC* pDC)
{
	CPatternGeneratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// centre of the window is where the sun is
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.Width() / 2;
	int cy = rect.Height() / 2;

	// background
	pDC->FillSolidRect(&rect, RGB(10, 15, 35));

	// planet data: Mercury, Venus, Earth, Mars, Saturn
	// radiusX = half width of the orbit, radiusY = half height
	int radiusX[5] = { 70, 110, 160, 215, 275 };
	int radiusY[5] = { 50, 78, 110, 145, 185 };
	int size[5] = { 5, 8, 9, 7, 12 };
	COLORREF color[5] = {
		RGB(180, 180, 180),
		RGB(240, 170, 80),
		RGB(50, 130, 255),
		RGB(220, 75, 55),
		RGB(200, 160, 90)
	};
	double angle[5] = { m_mercuryAngle, m_venusAngle, m_earthAngle, m_marsAngle, m_saturnAngle };

	// orbits (ellipses with no fill)
	CPen orbitPen(PS_SOLID, 1, RGB(75, 85, 110));
	CPen* oldPen = pDC->SelectObject(&orbitPen);
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	for (int i = 0; i < 5; i++)
	{
		pDC->Ellipse(cx - radiusX[i], cy - radiusY[i], cx + radiusX[i], cy + radiusY[i]);
	}

	// sun
	CBrush sunBrush(RGB(255, 190, 30));
	pDC->SelectObject(&sunBrush);
	pDC->Ellipse(cx - 24, cy - 24, cx + 24, cy + 24);

	// planets
	for (int i = 0; i < 5; i++)
	{
		// position on the ellipse
		int x = cx + (int)(radiusX[i] * cos(angle[i]));
		int y = cy + (int)(radiusY[i] * sin(angle[i]));

		CBrush planetBrush(color[i]);
		CBrush* prevBrush = pDC->SelectObject(&planetBrush);
		pDC->Ellipse(x - size[i], y - size[i], x + size[i], y + size[i]);
		pDC->SelectObject(prevBrush);

		// Saturn (last planet) gets a yellow ring
		if (i == 4)
		{
			CPen ringPen(PS_SOLID, 2, RGB(255, 230, 0));
			CPen* prevPen = pDC->SelectObject(&ringPen);
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->Ellipse(x - 22, y - 6, x + 22, y + 6);
			pDC->SelectObject(prevPen);
		}
	}

	// put back the old pen and brush
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}


// CPatternGeneratorView printing


void CPatternGeneratorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPatternGeneratorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPatternGeneratorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPatternGeneratorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPatternGeneratorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPatternGeneratorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPatternGeneratorView diagnostics

#ifdef _DEBUG
void CPatternGeneratorView::AssertValid() const
{
	CView::AssertValid();
}

void CPatternGeneratorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPatternGeneratorDoc* CPatternGeneratorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPatternGeneratorDoc)));
	return (CPatternGeneratorDoc*)m_pDocument;
}
#endif //_DEBUG


// CPatternGeneratorView message handlers

// start: timer fires every 30 ms
void CPatternGeneratorView::OnButtonStart()
{
	SetTimer(1, 30, nullptr);
}

// stop: planets freeze
void CPatternGeneratorView::OnButtonStop()
{
	KillTimer(1);
}

// resume: planets continue from where they stopped
void CPatternGeneratorView::OnButtonResume()
{
	SetTimer(1, 30, nullptr);
}

// timer: move each planet a little and redraw
void CPatternGeneratorView::OnTimer(UINT_PTR nIDEvent)
{
	m_mercuryAngle += 0.08;
	m_venusAngle += 0.05;
	m_earthAngle += 0.03;
	m_marsAngle += 0.02;
	m_saturnAngle += 0.012;

	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}