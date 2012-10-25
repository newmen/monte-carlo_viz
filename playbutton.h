#ifndef PLAYBUTTON_H
#define PLAYBUTTON_H

#include <QPushButton>

class PlayButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PlayButton(const QString& startText, const QString& stopText, QWidget *parent = 0);

    bool started() const;

signals:
    void timerStart();
    void timerStop();

private slots:
    void changeState();

private:
    QString _startText, _stopText;
    bool _started;
};

#endif // PLAYBUTTON_H
