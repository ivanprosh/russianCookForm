#include "receptimage.h"

void ReceptImage::mousePressEvent(QMouseEvent *me)
{
   if(me->buttons() & Qt::LeftButton)
   {
       QPainter painter(this);
       QString path = QFileDialog::getOpenFileName(0,"Выбрать файл изображения","","*.png *.jpg *.bmp");
       if(path.isEmpty()) return;

       QPixmap img;
       img.load(path);
       img = img.scaled(this->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
       this->setPixmap(img);
   }
}

