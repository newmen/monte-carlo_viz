#include <QtGui>
#include "renderarea_context.h"
#include "drawing_role.h"

RenderAreaContext::RenderAreaContext(const AreaData *area, float cellSideLength, bool needBorder) :
    _area(area), _cellSideLength(cellSideLength), _needBorder(needBorder)
{
    setMinimumSize(_cellSideLength * _area->sizeX(), _cellSideLength * _area->sizeY());
}

void RenderAreaContext::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen((_needBorder) ? Qt::gray : Qt::transparent);
    painter.setPen(pen);

    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRect &r = event->rect();

    // lol condition xD
    if (r.width() == _cellSideLength || r.height() == _cellSideLength) {
        static_cast<const DrawingRole<AreaData> *>(_area)->redrawRegion(&painter, r, _cellSideLength);
    } else {
        static_cast<const DrawingRole<AreaData> *>(_area)->draw(&painter, _cellSideLength);
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
}
