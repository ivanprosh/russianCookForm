#ifndef RECEPTIMAGE_H
#define RECEPTIMAGE_H

#include <QtWidgets>

class ReceptImage : public QLabel
{
    Q_OBJECT
public:
    explicit ReceptImage(QWidget *parent = 0):QLabel(parent){}

protected:
    virtual void mousePressEvent(QMouseEvent* me);


signals:

public slots:
    //void setPixmap(const QPixmap&);
};

#endif // RECEPTIMAGE_H
