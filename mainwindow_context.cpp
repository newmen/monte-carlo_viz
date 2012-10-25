#include <QtGui>
#include "mainwindow_context.h"

MainWindowContext::MainWindowContext() : _snapShotsPath(""), _everySecond(0.05), _secondsCounter(0),
    _readContext(0), _renderArea(0), _cellSideLength(3), _totalTime(0)
{
    setWindowTitle("Monte Carlo simulation");
    QVBoxLayout *layout = new QVBoxLayout;

    _area = new AreaData(0, 1); // костыль!
    _renderArea = new RenderAreaContext(_area, _cellSideLength, false);
    layout->addWidget(_renderArea);

    _loadButton = new QPushButton("Load MC Data");
    connect(_loadButton, SIGNAL(clicked()), this, SLOT(openFile()));
    layout->addWidget(_loadButton);

    _selectDirButton = new QPushButton("Select Snapshot dir");
    connect(_selectDirButton, SIGNAL(clicked()), this, SLOT(openDirectory()));
    layout->addWidget(_selectDirButton);

//    _selectDirLabel = new QLabel(_snapShotsPath);
//    layout->addWidget(_selectDirLabel);

    _sideLengthLabel = new QLabel("Cell side length: ");
    QString sideLengthStr;
    sideLengthStr.setNum((double)_cellSideLength);
    _sideLengthText = new QTextEdit(sideLengthStr);
    connect(_sideLengthText, SIGNAL(textChanged()), this, SLOT(updateSideLength()));

    QHBoxLayout *sideLengthLayout = new QHBoxLayout;
    sideLengthLayout->addWidget(_sideLengthLabel);
    sideLengthLayout->addWidget(_sideLengthText);
    QGroupBox *sideLengthGroup = new QGroupBox;
    sideLengthGroup->setLayout(sideLengthLayout);
    layout->addWidget(sideLengthGroup);

    _doButton = new QPushButton("Do reaction");
    connect(_doButton, SIGNAL(clicked()), this, SLOT(doReaction()));
    layout->addWidget(_doButton);

    _playButton = new PlayButton("Play", "Stop");
    connect(_playButton, SIGNAL(timerStart()), this, SLOT(playAnimation()));
    connect(_playButton, SIGNAL(timerStop()), this, SLOT(stopAnimation()));
    layout->addWidget(_playButton);

    _totalTimeTextLabel = new QLabel("Total time: ");
    _totalTimeValueLabel = new QLabel("0.0");
    _timeDimLabel = new QLabel(" sec");

    QHBoxLayout *totalTimeLayout = new QHBoxLayout;
    totalTimeLayout->addWidget(_totalTimeTextLabel);
    totalTimeLayout->addWidget(_totalTimeValueLabel);
    totalTimeLayout->addWidget(_timeDimLabel);
    QGroupBox *totalTimeGroup = new QGroupBox;
    totalTimeGroup->setLayout(totalTimeLayout);
    layout->addWidget(totalTimeGroup);

    setLayout(layout);

    _animationTimer = new QTimer(this);
    connect(_animationTimer, SIGNAL(timeout()), this, SLOT(doReaction()));

    updateStatusBar();
}

MainWindowContext::~MainWindowContext() {
    delete _readContext;
    delete _renderArea;
    delete _doButton;
    delete _playButton;
    delete _animationTimer;
}

void MainWindowContext::doReaction() {
    if (_readContext == 0) return;
    EventRecordData eRecord = _readContext->restoreEvent();

    _totalTime += eRecord.dt();
    updateStatusBar();
    if (eRecord.nothing()) {
        if (_playButton->started()) _playButton->click();
    } else if (eRecord.isCell()) {
        updateCell(eRecord.first(), eRecord.x(), eRecord.y());
    } else if (eRecord.isHorizontalDimer()) {
        updateCell(eRecord.first(), eRecord.x(), eRecord.y());
        updateCell(eRecord.second(), eRecord.x() + 1, eRecord.y());
    } else {
        updateCell(eRecord.first(), eRecord.x(), eRecord.y());
        updateCell(eRecord.second(), eRecord.x(), eRecord.y() + 1);
    }

    _secondsCounter += eRecord.dt();
    if (_secondsCounter > _everySecond) {
        _secondsCounter = 0;
        saveSnapShot();
    }
}

void MainWindowContext::playAnimation() {
    _animationTimer->start(0);
}

void MainWindowContext::stopAnimation() {
    _animationTimer->stop();
}

void MainWindowContext::updateCell(CellType value, CoordType x, CoordType y) {
    CoordType cx = (x == _area->sizeX()) ? 0 : x;
    CoordType cy = (y == _area->sizeY()) ? 0 : y;
    _area->setValue(value, cx, cy);
    _renderArea->update(cx * _cellSideLength, cy * _cellSideLength, _cellSideLength, _cellSideLength);
}

void MainWindowContext::updateStatusBar() {
    QString totalTimeText;
    totalTimeText.setNum((double)_totalTime);
    _totalTimeValueLabel->setText(totalTimeText);
}

void MainWindowContext::openFile() {
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Monte Carlo data file"),
                                                    tr("~/"),
                                                    tr("MC Data Files (*.mcd);;All Files (*)"),
                                                    &selectedFilter,
                                                    options);
    if (fileName.isEmpty()) return;

    if (_readContext != 0) delete _readContext;
    _readContext = new ReadEventContext(fileName.toStdString());

    Point2D sizes = _readContext->areaSizes();
    delete _area;
    _area = new AreaData(sizes.x, sizes.y);
    _renderArea->resetArea(_area);
    _totalTime = 0;
}

void MainWindowContext::openDirectory() {
    QFileDialog dialog(this);
    dialog.setDirectory(_snapShotsPath);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setFileMode(QFileDialog::Directory);

    if (dialog.exec()) {
        QStringList selectedFiles = dialog.selectedFiles();
        _snapShotsPath = selectedFiles[0];
        qDebug() << selectedFiles[0];

    }
}

void MainWindowContext::updateSideLength() {
    _cellSideLength = _sideLengthText->toPlainText().toInt();
    if (_cellSideLength == 0) return;
    _renderArea->resetSideLength(_cellSideLength);
}

void MainWindowContext::saveSnapShot() {
    if (_snapShotsPath == "") return;

    QPixmap pixmap = QPixmap::grabWidget(_renderArea);
//    pixmap.fill();
    QString out = _snapShotsPath + QString('/') + QString::number(_totalTime) + QString(".png");
//    QString out = QString::number(_totalTime) + QString(".png");
    if (pixmap.save(out)) {
        qDebug() << out;
    } else {
        QString msgText = tr("Failed to write into ") + out;
        QMessageBox::critical(this, tr("Error Writing"), msgText);
    }
}
