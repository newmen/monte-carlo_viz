#ifndef MAINWINDOW_CONTEXT_H
#define MAINWINDOW_CONTEXT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "playbutton.h"
#include "renderarea_context.h"

#include "../../src/contexts/basereactor_context.h"
#include "../../src/contexts/basesimulation_context.h"

class MainWindowContext : public QWidget
{
    Q_OBJECT
public:
    MainWindowContext();
    ~MainWindowContext();
    
//signals:
//public slots:
private slots:
    void doReaction();

    void playAnimation();
    void stopAnimation();

private:
    void updateCell(const CellData *cell);
    void updateStatusBar();

private:
    AreaData _area;
    BaseReactorContext *_reactor;
    BaseSimulationContext *_simulationContext;
    RenderAreaContext *_renderArea;
    float _cellSideLength;

    QPushButton *_doButton;
    PlayButton *_playButton;
    QTimer *_animationTimer;

    long double _totalTime;
    QLabel *_totalTimeTextLabel;
    QLabel *_totalTimeValueLabel;
    QLabel *_timeDimLabel;
};

#endif // MAINWINDOW_CONTEXT_H
