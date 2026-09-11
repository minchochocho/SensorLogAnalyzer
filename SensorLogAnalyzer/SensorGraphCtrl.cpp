#include "pch.h"
#include "SensorGraphCtrl.h"

void CSensorGraphCtrl::SetData(
    const std::vector<double>& values,
    double maxValue,
    COLORREF lineColor
) {
    m_values = values;
    m_maxValue = maxValue > 0.0 ? maxValue : 1.0;
    m_lineColor = lineColor;

    if (GetSafeHwnd() != nullptr) {
        Invalidate(FALSE);
    }
}

void CSensorGraphCtrl::DrawItem(
	LPDRAWITEMSTRUCT lpDrawItemStruct
) {
	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect clientRect(lpDrawItemStruct->rcItem);
    dc->FillSolidRect(clientRect, RGB(255, 255, 255));

    CRect graphRect = clientRect;
    graphRect.DeflateRect(45, 15, 15, 30);

    // 격자
    CPen gridPen(PS_DOT, 1, RGB(220, 220, 220));
    CPen* oldPen = dc->SelectObject(&gridPen);

    for (int i = 1; i < 5; i++) {
        int y = graphRect.top
            + graphRect.Height() * i / 5;

        dc->MoveTo(graphRect.left, y);
        dc->LineTo(graphRect.right, y);
    }

    for (int i = 1; i < 5; i++) {
        int x = graphRect.left
            + graphRect.Width() * i / 5;

        dc->MoveTo(x, graphRect.top);
        dc->LineTo(x, graphRect.bottom);
    }

    // 그래프 외곽선
    CPen axisPen(PS_SOLID, 1, RGB(80, 80, 80));
    dc->SelectObject(&axisPen);

    CBrush* oldBrush =
        static_cast<CBrush*>(dc->SelectStockObject(NULL_BRUSH));

    dc->Rectangle(graphRect);

    if (m_values.size() >= 2) {
        std::vector<CPoint> points;

        for (size_t i = 0; i < m_values.size(); i++) {
            double value = m_values[i];

            if (value < 0.0) {
                value = 0.0;
            }
            else if (value > m_maxValue) {
                value = m_maxValue;
            }

            int x = graphRect.left
                + static_cast<int>(
                    i * graphRect.Width()
                    / (m_values.size() - 1)
                    );

            int y = graphRect.bottom
                - static_cast<int>(
                    value * graphRect.Height()
                    / m_maxValue
                    );

            points.emplace_back(x, y);
        }

        CPen dataPen(PS_SOLID, 2, m_lineColor);
        CPen* previousPen = dc->SelectObject(&dataPen);

        dc->Polyline(
            points.data(),
            static_cast<int>(points.size())
        );

        dc->SelectObject(previousPen);
    }

    dc->SelectObject(oldBrush);
    dc->SelectObject(oldPen);
}