#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>
#include <QtWidgets>

class MyTableWidget : public QTableWidget
{
public:
    MyTableWidget(QWidget* pobj = 0) : QTableWidget(pobj) {}
    MyTableWidget(int rows,int columns,QWidget* pobj = 0) : QTableWidget(rows,columns,pobj) {}
    QModelIndex myIndexFromItem(QTableWidgetItem * item) const;
};

#endif // MYTABLEWIDGET_H
