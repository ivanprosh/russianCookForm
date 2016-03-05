#include "mytablewidget.h"


QModelIndex MyTableWidget::myIndexFromItem(QTableWidgetItem* item) const
{
   return indexFromItem(item);
}
