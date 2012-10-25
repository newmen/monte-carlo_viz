#ifndef RENDERAREA_CONTEXT_H
#define RENDERAREA_CONTEXT_H

#include <QWidget>

#include "../../src/datas/area_data.h"

class RenderAreaContext : public QWidget
{
    Q_OBJECT
public:
    explicit RenderAreaContext(const AreaData *area, float cellSideLength, bool needBorder);

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    const AreaData *_area;
    const float _cellSideLength;
    bool _needBorder;
};

#endif // RENDERAREA_CONTEXT_H
