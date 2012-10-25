#ifndef RENDERAREA_CONTEXT_H
#define RENDERAREA_CONTEXT_H

#include <QWidget>

#include "../../src/datas/area_data.h"

class RenderAreaContext : public QWidget
{
    Q_OBJECT
public:
    RenderAreaContext(const AreaData *area, float cellSideLength, bool needBorder);

    void resetArea(const AreaData *area);
    void resetSideLength(float cellSideLength);

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    void processSize();

    const AreaData *_area;
    float _cellSideLength;
    bool _needBorder;
};

#endif // RENDERAREA_CONTEXT_H
