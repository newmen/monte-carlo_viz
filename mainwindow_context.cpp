#include <QtGui>
#include "mainwindow_context.h"

#include "../../src/contexts/abcdcellreactor_context.h"
#include "../../src/contexts/abcddimerreactor_context.h"
#include "../../src/contexts/nocoreactor_context.h"
#include "../../src/contexts/lotkareactor_context.h"
#include "../../src/contexts/lotkathreereactor_context.h"

#include "../../src/contexts/rejectionsimulation_context.h"
#include "../../src/contexts/rejectionfreesimulation_context.h"
#include "../../src/contexts/dynamicsimulation_context.h"
#include "../../src/contexts/kineticsimulation_context.h"
#include "../../src/contexts/treebasedsimulation_context.h"

MainWindowContext::MainWindowContext() : _area(50, 50), _cellSideLength(5), _totalTime(0) {
    setWindowTitle("Monte Carlo simulation");

//    _reactor = new ABCDCellReactorContext;
//    _reactor = new ABCDDimerReactorContext;
    _reactor = new NOCOReactorContext;
//    _reactor = new LotkaReactorContext;
//    _reactor = new LotkaThreeReactorContext;

//    _simulationContext = new RejectionSimulationContext(&_area, _reactor);
//    _simulationContext = new RejectionFreeSimulationContext(&_area, _reactor);
//    _simulationContext = new DynamicSimulationContext(&_area, _reactor);
//    _simulationContext = new KineticSimulationContext(&_area, _reactor);
    _simulationContext = new TreeBasedSimulationContext(&_area, _reactor);
    _renderArea = new RenderAreaContext(&_area, _cellSideLength, false);

    _doButton = new QPushButton("Do reaction");
    connect(_doButton, SIGNAL(clicked()), this, SLOT(doReaction()));

    _playButton = new PlayButton("Play", "Stop");
    connect(_playButton, SIGNAL(timerStart()), this, SLOT(playAnimation()));
    connect(_playButton, SIGNAL(timerStop()), this, SLOT(stopAnimation()));

    _totalTimeTextLabel = new QLabel("Total time: ");
    _totalTimeValueLabel = new QLabel("0.0");
    _timeDimLabel = new QLabel(" sec");

    QHBoxLayout *totalTimeLayout = new QHBoxLayout;
    totalTimeLayout->addWidget(_totalTimeTextLabel);
    totalTimeLayout->addWidget(_totalTimeValueLabel);
    totalTimeLayout->addWidget(_timeDimLabel);
    QGroupBox *totalTimeGroup = new QGroupBox;
    totalTimeGroup->setLayout(totalTimeLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_renderArea);
    layout->addWidget(_doButton);
    layout->addWidget(_playButton);
    layout->addWidget(totalTimeGroup);
    setLayout(layout);

    _animationTimer = new QTimer(this);
    connect(_animationTimer, SIGNAL(timeout()), this, SLOT(doReaction()));

    updateStatusBar();
}

MainWindowContext::~MainWindowContext() {
    delete _simulationContext;
    delete _reactor;
    delete _renderArea;
    delete _doButton;
    delete _playButton;
    delete _animationTimer;
}

void MainWindowContext::doReaction() {
    EventInfoData ei = _simulationContext->doReaction();

    long double dt = ei.dt();
    _totalTime += dt;
    updateStatusBar();
    if (dt == 0.0) {
        if (_playButton->started()) _playButton->click();
    } else if (ei.cell() != 0) {
        updateCell(ei.cell());
    } else {
        updateCell(ei.dimer()->first);
        updateCell(ei.dimer()->second);
    }
}

void MainWindowContext::playAnimation() {
    _animationTimer->start(0);
}

void MainWindowContext::stopAnimation() {
    _animationTimer->stop();
}

void MainWindowContext::updateCell(const CellData *cell) {
    _renderArea->update(cell->x() * _cellSideLength, cell->y() * _cellSideLength, _cellSideLength, _cellSideLength);
}

void MainWindowContext::updateStatusBar() {
    QString totalTimeText;
    totalTimeText.setNum((double)_totalTime);
    _totalTimeValueLabel->setText(totalTimeText);
}
