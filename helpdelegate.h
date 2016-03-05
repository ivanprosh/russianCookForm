#ifndef HELPDELEGATE_H
#define HELPDELEGATE_H

#include "sqlform.h"
#include "global.h"

class HelpDelegate : public QStyledItemDelegate {
public:
    HelpDelegate(QObject* pobj = 0) : QStyledItemDelegate(pobj)
    {
    }
    QWidget* createEditor(QWidget * parent,
                          const QStyleOptionViewItem & option,
                          const QModelIndex & index) const
    {
        switch(index.column())
        {
        case ColumnNameCompos::name:
        {
            QComboBox* boxeditor = new QComboBox(parent);
            boxeditor->setEditable(true);
            boxeditor->addItems(listAvailableNames);
            boxeditor->setValidator(new QRegExpValidator(QRegExp("[А-я,0-9 -]{,30}")));
            //boxeditor->keyboardGrabber()
            return boxeditor;
        }
        case ColumnNameCompos::count:
        {
            QDoubleSpinBox* spineditor = new QDoubleSpinBox(parent);
            spineditor->setButtonSymbols(QSpinBox::PlusMinus);
            spineditor->setRange(0,100000);
            spineditor->setSingleStep(1);
            return spineditor;
        }
        case ColumnNameCompos::metrics:
        {
            QComboBox* boxeditor = new QComboBox(parent);
            QStringList result;
            if(!listforcurProdMetrics.isEmpty())
            {
            result << listforcurProdMetrics.at(index.row()) << listAvailableMetrics;
            result.removeDuplicates();
            boxeditor->setEditable(true);
            boxeditor->addItems(result);
            boxeditor->setCurrentIndex(0);
            boxeditor->insertSeparator(listforcurProdMetrics.at(index.row()).size());
            boxeditor->setStyleSheet("QComboBox::separator{height: 2px;}");
            boxeditor->setValidator(new QRegExpValidator(QRegExp("[а-я]{,10}")));
            }
            else
                boxeditor->setEnabled(false);
            return boxeditor;

        }
        case ColumnNameCompos::type:
        {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setValidator(new QRegExpValidator(QRegExp("[а-я ]{,25}")));
            return editor;
        }
        default:
            return QStyledItemDelegate::createEditor(parent,option,index);
        }

    }

    void paint(QPainter*                   pPainter,
               const QStyleOptionViewItem& option,
               const QModelIndex&          index
              ) const
    {
        QStyledItemDelegate::paint(pPainter, option, index);
    }
    void setEditorData(QWidget * editor,
                           const QModelIndex & index) const
    {
        switch(index.column())
        {
        case ColumnNameCompos::name:
        {
            QComboBox* boxeditor = qobject_cast<QComboBox*>(editor);
            boxeditor->setCurrentText("");
            qDebug()<<"In setEditorData";
        }
        }
    }
signals:
    void commitData(QWidget* editor)
    {
        QStyledItemDelegate::commitData(editor);
    }
public slots:
    void fillItem(const QModelIndex& index);

};

#endif // HELPDELEGATE_H
