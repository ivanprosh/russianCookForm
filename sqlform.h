#ifndef SQLFORM_H
#define SQLFORM_H

#include <QMainWindow>
#include <QtWidgets>
#include "mytablewidget.h"
#include "receptimage.h"

enum ColumnNameCompos
{
    name=0,count,metrics,type,description
};

class SQLForm : public QWidget
{
    Q_OBJECT

private:
    QTableWidget* TableComposition;
    QComboBox* RecName;
    QComboBox* RecType;
    QComboBox* RecSubType;
    QComboBox* MainProd;
    QComboBox* PostDay;
    QLabel* TextDescrLabel;
    QPlainTextEdit* description;
    QPlainTextEdit* RecComment;
    ReceptImage* LabRecImage;
    QPixmap RecInitImg,RecEmptyImg;

    QHBoxLayout* resultlayout;
    void init(QString curRecName);
public:

    SQLForm(QWidget *parent = 0);
    ~SQLForm();
    void updateAvailLists(QStringList* list, QString Name, QString Table);
    void updateAllLists();
    void paintLayout(QPalette *pal, QLayoutItem *item);
    void clearForm(QLayoutItem *item);
    QString findRec(QString TableName, QString SelectColumnName,QString lCondition1, QString rCondition1,
                    QString lCondition2="'0'",
                    QString rCondition2="0");//поиск
protected:
    virtual void resizeEvent(QResizeEvent *pe);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

public slots:
    void addIngredient();
    void delIngredient();
    void fitToSize();
    void checkItem(int,int);
    void addToBase();
    void fillOnSelectRec(QString SelectItem);
    void isChanged();
    void changedIsSaved();
    void isClear();
    void updateListSubType(int TypeIndex);
   // void NewSelectRec(QString SelectItem);
};

#endif // SQLFORM_H
