#include "shootablelabel.h"

shootableLabel::shootableLabel(QWidget *parent) : QLabel(parent)
{

}

void shootableLabel::mousePressEvent(QMouseEvent *ev)
{
    QPoint target = ev->pos();
    emit Mouse_Pressed(target);
}

