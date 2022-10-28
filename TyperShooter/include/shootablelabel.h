#ifndef SHOOTABLELABEL_H
#define SHOOTABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QtDebug>

class shootableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit shootableLabel(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *ev);

signals:

    void Mouse_Pressed(QPoint);

public slots:
};

#endif // SHOOTABLELABEL_H
