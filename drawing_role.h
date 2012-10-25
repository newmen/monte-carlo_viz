#ifndef DRAWING_ROLE_H
#define DRAWING_ROLE_H

#include <QPainter>

template <class AData>
class DrawingRole : public AData
{
public:
    DrawingRole() {}

    void draw(QPainter *painter, float cellSideLength) const;
    void redrawRegion(QPainter *painter, const QRect &rect, float cellSideLength) const;

private:
    void drawCell(QPainter *painter, int cellValue, int x, int y, float cellSideLength) const;
    QPointF offset(float cellSideLength, int x, int y) const;
};

template <class AData>
void DrawingRole<AData>::draw(QPainter *painter, float cellSideLength) const {
    this->eachCell([this, &painter, cellSideLength](CellType *const value, CoordType x, CoordType y) {
        this->drawCell(painter, *value, x, y, cellSideLength);
    });
}

template <class AData>
void DrawingRole<AData>::redrawRegion(QPainter *painter, const QRect &rect, float cellSideLength) const {
    int x = rect.left() / cellSideLength;
    int y = rect.top() / cellSideLength;
    auto drawLambda = [this, &painter, &x, &y, cellSideLength]() {
        this->drawCell(painter, this->value(this->index(x, y)), x, y, cellSideLength);
    };

    drawLambda();

    if (rect.width() == 2 * cellSideLength) ++x;
    else if (rect.width() == this->sizeX() * cellSideLength) x = this->sizeX() - 1;
    else if (rect.height() == 2 * cellSideLength) ++y;
    else if (rect.height() == this->sizeY() * cellSideLength) y = this->sizeY() - 1;
    else return;

    drawLambda();
}

template <class AData>
void DrawingRole<AData>::drawCell(QPainter *painter, int cellValue, int x, int y, float cellSideLength) const {
    QRect rect(0, 0, cellSideLength, cellSideLength);

    painter->save();
    painter->translate(this->offset(cellSideLength, x, y));

    QColor color;
    switch (cellValue) {
    case 1:
        color.setRgb(248, 248, 248);
        break;
    case 2:
        color.setRgb(222, 22, 22);
        break;
    case 3:
        color.setRgb(56, 56, 156);
        break;
    case 4:
        color.setRgb(33, 33, 33);
        break;
    case 5:
        color.setRgb(25, 225, 25);
        break;
    }
    painter->setBrush(color);
    painter->drawRect(rect);

    painter->restore();
}

template <class AData>
QPointF DrawingRole<AData>::offset(float cellSideLength, int x, int y) const {
    return QPointF(x * cellSideLength, y * cellSideLength);
}

#endif // DRAWING_ROLE_H
