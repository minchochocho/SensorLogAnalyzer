#include "pch.h"
#include "SensorGraphCtrl.h"
#include <cmath>

void CSensorGraphCtrl::SetData(
    const std::vector<double>& values,
    const std::vector<CString>& labels,
    double maxValue,
    COLORREF lineColor
) {
    m_values = values;
    m_labels = labels;
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

    // 세로축 눈금값
    dc->SetBkMode(TRANSPARENT);
    dc->SetTextColor(RGB(80, 80, 80));

    for (int i = 0; i <= 5; i++) {
        double value = m_maxValue * (5 - i) / 5.0;

        int y = graphRect.top
            + graphRect.Height() * i / 5;

        CString label;
        label.Format(_T("%.0f"), value);

        CRect labelRect(
            clientRect.left,
            y - 9,
            graphRect.left - 5,
            y + 9
        );

        dc->DrawText(
            label,
            labelRect,
            DT_RIGHT | DT_VCENTER | DT_SINGLELINE
        );
    }

    // 가로축에는 처음, 중간, 마지막 측정 시각만 표시
    if (!m_labels.empty() && m_labels.size() == m_values.size()) {
        const size_t labelCount =
            m_labels.size() < 3 ? m_labels.size() : 3;

        for (size_t slot = 0; slot < labelCount; slot++) {
            size_t index = 0;

            if (labelCount > 1) {
                index = slot * (m_labels.size() - 1)
                    / (labelCount - 1);
            }

            const CString& label = m_labels[index];

            if (label.IsEmpty()) {
                continue;
            }

            int x = graphRect.left;

            if (m_labels.size() > 1) {
                x += static_cast<int>(
                    index * graphRect.Width()
                    / (m_labels.size() - 1)
                );
            }

            CRect textRect;
            UINT textFormat = DT_SINGLELINE | DT_VCENTER;

            if (slot == 0) {
                textRect.SetRect(
                    graphRect.left,
                    graphRect.bottom + 5,
                    graphRect.left + 70,
                    clientRect.bottom
                );
                textFormat |= DT_LEFT;
            }
            else if (slot == labelCount - 1) {
                textRect.SetRect(
                    graphRect.right - 70,
                    graphRect.bottom + 5,
                    graphRect.right,
                    clientRect.bottom
                );
                textFormat |= DT_RIGHT;
            }
            else {
                textRect.SetRect(
                    x - 40,
                    graphRect.bottom + 5,
                    x + 40,
                    clientRect.bottom
                );
                textFormat |= DT_CENTER;
            }

            dc->DrawText(label, textRect, textFormat);
        }
    }

    // 선 그리기
    if (m_values.size() >= 2) {
        CPen dataPen(PS_SOLID, 2, m_lineColor);
        CPen* previousPen = dc->SelectObject(&dataPen);

        bool hasPreviousPoint = false;

        for (size_t i = 0; i < m_values.size(); i++) {
            double value = m_values[i];

            // 오류 행을 만나면 현재 선을 끊음
            if (!std::isfinite(value)) {
                hasPreviousPoint = false;
                continue;
            }

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

            if (!hasPreviousPoint) {
                dc->MoveTo(x, y);
                hasPreviousPoint = true;
            }
            else {
                dc->LineTo(x, y);
            }
        }

        dc->SelectObject(previousPen);
    }

    dc->SelectObject(oldBrush);
    dc->SelectObject(oldPen);
}
