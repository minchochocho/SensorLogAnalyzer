#pragma once

#include <afxwin.h>
#include <vector>

class CSensorGraphCtrl : public CStatic {
public:
    void SetData(
        const std::vector<double>& values,
        double maxValue,
        COLORREF lineColor
    );


    virtual void DrawItem(
        LPDRAWITEMSTRUCT lpDrawItemStruct
    );

private:
    std::vector<double> m_values;
    double m_maxValue = 1.0;
    COLORREF m_lineColor = RGB(0, 120, 215);
};