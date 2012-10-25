#ifndef MAINWINDOW_CONTEXT_H
#define MAINWINDOW_CONTEXT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>
#include "playbutton.h"
#include "renderarea_context.h"

#include "../../src/contexts/readshot_context.h"
#include "../../src/datas/area_data.h"

class MainWindowContext : public QWidget
{
    Q_OBJECT
public:
    MainWindowContext();
    ~MainWindowContext();
    
private slots:
    void doReaction();

    void playAnimation();
    void stopAnimation();

    void openFile();
    void openDirectory();
    void updateSideLength();

    void saveSnapShot();

private:
//    void updateCell(CellType value, CoordType x, CoordType y);
    void updateStatusBar();

private:
    QString _snapShotsPath;
    double _everySecond;
    double _secondsCounter;

    AreaData *_area;
    ReadShotContext *_readContext;
    RenderAreaContext *_renderArea;
    float _cellSideLength;

    QPushButton *_loadButton;
    QPushButton *_selectDirButton;
    QLabel *_selectDirLabel;
    QLabel *_sideLengthLabel;
    QTextEdit *_sideLengthText;

    QPushButton *_doButton;
    PlayButton *_playButton;
    QTimer *_animationTimer;

    double _totalTime;
    QLabel *_totalTimeTextLabel;
    QLabel *_totalTimeValueLabel;
    QLabel *_timeDimLabel;
};

#endif // MAINWINDOW_CONTEXT_H
