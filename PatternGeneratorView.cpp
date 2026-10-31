//Planetary Motion SOURCE CODE
// PatternGeneratorView.cpp : implementation of the CPatternGeneratorView class

#include "pch.h"
#include "framework.h"
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
	// printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPatternGeneratorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	// toolbar buttons
	ON_COMMAND(ID_BUTTON_START, &CPatternGeneratorView::OnButtonStart)
	ON_COMMAND(ID_BUTTON_STOP, &CPatternGeneratorView::OnButtonStop)
	ON_COMMAND(ID_BUTTON_RESUME, &CPatternGeneratorView::OnButtonResume)
END_MESSAGE_MAP()


// constructor
CPatternGeneratorView::CPatternGeneratorView() noexcept
{
	m_pCurrentThread = NULL;
	m_iCounter = 0;

	// all planets start at angle 0
	m_mercuryAngle = 0.0;
	m_venusAngle = 0.0;
	m_earthAngle = 0.0;
	m_marsAngle = 0.0;
}

CPatternGeneratorView::~CPatternGeneratorView()
{
}

BOOL CPatternGeneratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


// drawing
void CPatternGeneratorView::OnDraw(CDC* pDC)
{
	CPatternGeneratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// find the centre of the window
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.Width() / 2;
	int cy = rect.Height() / 2;

	// dark background
	pDC->FillSolidRect(&rect, RGB(10, 15, 35));

	// planet data: orbit radius, planet size, colour, current angle
	int radius[4] = { 65, 100, 145, 195 };
	int size[4] = { 5, 8, 9, 7 };
	COLORREF color[4] = {
		RGB(180, 180, 180),   // Mercury
		RGB(240, 170, 80),    // Venus
		RGB(50, 130, 255),    // Earth
		RGB(220, 75, 55)      // Mars
	};
	double angle[4] = { m_mercuryAngle, m_venusAngle, m_earthAngle, m_marsAngle };

	// draw the orbit circles (no fill)
	CPen orbitPen(PS_SOLID, 1, RGB(75, 85, 110));
	CPen* oldPen = pDC->SelectObject(&orbitPen);
	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	for (int i = 0; i < 4; i++)
	{
		pDC->Ellipse(cx - radius[i], cy - radius[i], cx + radius[i], cy + radius[i]);
	}

	// draw the sun in the middle
	CPen sunPen(PS_SOLID, 1, RGB(255, 220, 80));
	CBrush sunBrush(RGB(255, 190, 30));
	pDC->SelectObject(&sunPen);
	pDC->SelectObject(&sunBrush);
	pDC->Ellipse(cx - 24, cy - 24, cx + 24, cy + 24);

	// draw each planet on its orbit
	for (int i = 0; i < 4; i++)
	{
		// x = r*cos(angle), y = r*sin(angle)
		int x = cx + (int)(radius[i] * cos(angle[i]));
		int y = cy + (int)(radius[i] * sin(angle[i]));

		CBrush planetBrush(color[i]);
		CBrush* prevBrush = pDC->SelectObject(&planetBrush);
		pDC->Ellipse(x - size[i], y - size[i], x + size[i], y + size[i]);
		pDC->SelectObject(prevBrush);
	}

	// put the old pen and brush back
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	// show the frame number on top left
	CString status;
	status.Format(L"Planetary Motion Simulation | Frame: %d", m_iCounter);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(15, 15, status);
}


// printing
void CPatternGeneratorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPatternGeneratorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CPatternGeneratorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPatternGeneratorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
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


// diagnostics
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


// button handlers

// start: timer 1 fires every 30 ms
void CPatternGeneratorView::OnButtonStart()
{
	SetTimer(1, 30, nullptr);
}

// stop: kill the timer so the planets freeze
void CPatternGeneratorView::OnButtonStop()
{
	KillTimer(1);
}

// resume: start the timer again, planets continue from where they stopped
void CPatternGeneratorView::OnButtonResume()
{
	SetTimer(1, 30, nullptr);
}


// timer: runs every 30 ms after Start is pressed
void CPatternGeneratorView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// move each planet a little (inner planets move faster)
		m_mercuryAngle += 0.08;
		m_venusAngle += 0.05;
		m_earthAngle += 0.03;
		m_marsAngle += 0.02;

		// count the frames
		m_iCounter++;

		// redraw the window
		Invalidate(FALSE);
	}

	CView::OnTimer(nIDEvent);
}