#include "helpdelegate.h"
#include "sqlform.h"
#include "linecontainer.h"
#include "mytablewidget.h"
#include "global.h"
#include "u_error.h"
#include "receptimage.h"

#define TABLECOLUMNS 5

extern QMap<QString,QString> SQLMAP;

struct SQLDataAddProduct
{
    QString data[TABLECOLUMNS];
    QString curProdId;
};

void SQLForm::updateAllLists()
{
    updateAvailLists(&listAvailableMetrics,"Metric","Product");
    updateAvailLists(&listAvailableNames,"Name","Product");
    updateAvailLists(&listAvailableRecepts,"Name","Recept");
    updateAvailLists(&listAvailTypeRec,"Type","ReceptType");
    updateAvailLists(&listAvailableTypes,"Type","Product");
    updateAvailLists(&listAvailRacions,"Racion","Recept");
    //updateAvailLists(&listAvailSubTypeRec,"Subtype","ReceptType");
}



void SQLForm::init(QString curRecName="")
{
    updateAllLists();
    //Иниц. табл. состава
    TableComposition->clearContents();
    TableComposition->setRowCount(0);
    addIngredient();
    //Иниц. описания
    description->clear();

    //Иниц. списка типов рецептов
    RecType->clear();
    RecType->addItems(listAvailTypeRec);
    RecName->setCurrentText("");

    //Иниц. списка подтипов рецептов
    RecSubType->clear();
    //RecSubType->addItems(listAvailSubTypeRec);
    //RecSubType->setCurrentText("");


    //Иниц. списка для основных ингредиентов
    MainProd->clear();
    //MainProd->addItems(listAvailableNames);
    MainProd->setCurrentText("");
    //Иниц. списка рационов
    PostDay->clear();
    PostDay->addItems(listAvailRacions);
    PostDay->setCurrentText("");

    //Иниц. списка рецептов
    RecName->clear();
    RecName->addItems(listAvailableRecepts);
    if(curRecName!="")
    {
        RecName->setCurrentIndex(RecName->findText(curRecName));
        emit(RecName->activated(curRecName));
    }
    else RecName->setCurrentText("");

    changedIsSaved();
}

void SQLForm::updateAvailLists(QStringList* list, QString Name, QString Table)
{
    QSqlRecord rec;
    QSqlQuery query;
    QString str = "SELECT DISTINCT %1"
                  " FROM %2;";
    list->clear();

    if(query.exec(str.arg(Name).arg(Table)))
    {
        rec=query.record();
        while(query.next())
        {
            if(!query.value(rec.indexOf(Name)).toString().isEmpty())
                *list << query.value(rec.indexOf(Name)).toString();//Metric
            //qDebug() << listAvailableMetrics;
        }
    }
    qSort(*list);

}

void SQLForm::updateListSubType(int TypeIndex)
{
    QSqlRecord rec;
    QSqlQuery query;
    QString str = "SELECT DISTINCT %1 "
                  "FROM %2 "
                  "WHERE Type='%3';";
    listAvailSubTypeRec.clear();
    RecSubType->clear();
    qDebug() << "In updAvailSubTypes" << str.arg("SubType").arg("ReceptType").arg(RecType->itemText(TypeIndex));
    if(query.exec(str.arg("SubType").arg("ReceptType").arg(RecType->itemText(TypeIndex))))
    {
        rec=query.record();
        while(query.next())
        {
            if(!query.value(rec.indexOf("SubType")).toString().isEmpty())
                listAvailSubTypeRec << query.value(rec.indexOf("SubType")).toString();//Metric
            qDebug() << listAvailSubTypeRec;
        }
    }
    qSort(listAvailSubTypeRec);
    RecSubType->addItems(listAvailSubTypeRec);

}

SQLForm::SQLForm(QWidget *parent)
    : QWidget(parent)
{

    QFont Labels(QApplication::font().family(),11,1);
    QFont CompositionHead(QApplication::font().family(),9,1);
    QApplication::setFont(Labels);

    setAcceptDrops(true);
    //Палитра
    QPalette pal = this->palette();
    pal.setColor(QPalette::Background, Qt::gray);
    this->setPalette(pal);
    //Style

    //размеры
    resize(1200,500);
    //Имя рецепта
    QLabel* TitRecName = new QLabel(trUtf8("&Рецепт(наименование)"));
    TitRecName->setFont(Labels);
    RecName = new QComboBox();
    RecName->setEditable(true);
    RecName->setFont(Labels);
    RecName->setDuplicatesEnabled(false);
    RecName->setValidator(new QRegExpValidator(QRegExp("[А-я,0-9 -]{,50}")));
    RecName->setObjectName("Рецепт");
    TitRecName->setBuddy(RecName);
    //тип рецепта
    QLabel* TypeRecName = new QLabel(trUtf8("&Рецепт(тип)"));
    RecType = new QComboBox();
    RecType->setEditable(false);
    RecType->setFont(Labels);
    RecType->setDuplicatesEnabled(false);
    RecType->setValidator(new QRegExpValidator(QRegExp("[А-я,0-9 -]{,50}")));
    RecType->setObjectName("Рецепт:тип");
    TypeRecName->setBuddy(RecType);
    //подтип
    QLabel* SubTypeRecName = new QLabel(trUtf8("Рецепт(подтип)"));
    RecSubType = new QComboBox();
    RecSubType->setEditable(false);
    RecSubType->setFont(Labels);
    RecSubType->setDuplicatesEnabled(false);
    RecSubType->setValidator(new QRegExpValidator(QRegExp("[А-я,0-9 -]{,50}")));
    RecSubType->setObjectName("Рецепт:подтип");
    SubTypeRecName->setBuddy(RecSubType);
    //основной ингредиент
    QLabel* MainProdRecName = new QLabel(trUtf8("Основной компонент"));
    MainProd = new QComboBox();
    MainProd->setEditable(false);
    MainProd->setDuplicatesEnabled(false);
    MainProd->setFont(Labels);
    MainProd->setValidator(new QRegExpValidator(QRegExp("[А-я,0-9 -]{,50}")));
    MainProd->setObjectName("Рецепт:основной компонент");
    MainProdRecName->setBuddy(RecSubType);
    //рацион
    QLabel* PostDayName = new QLabel(trUtf8("Рацион"));
    PostDay = new QComboBox();
    PostDay->setEditable(true);
    PostDay->setDuplicatesEnabled(false);
    PostDay->setFont(Labels);
    PostDay->setValidator(new QRegExpValidator(QRegExp("[А-я, -]{,50}")));
    PostDay->setObjectName("Рецепт:рацион");
    PostDayName->setBuddy(PostDay);

    //Состав
    QLabel* TitTableName = new QLabel("&Состав");
    TableComposition = new QTableWidget(0,TABLECOLUMNS);
    QStringList lst;
    lst << "Наименование" << "Кол-во" << "Ед.изм." << "Тип" << "Описание";
    TableComposition->setHorizontalHeaderLabels(lst);
    TableComposition->horizontalHeader()->setFont(CompositionHead);
    TableComposition->verticalHeader()->setFont(CompositionHead);

    connect(TableComposition->horizontalHeader(),
    SIGNAL(sectionClicked(int)), TableComposition, SLOT(sortByColumn(int)));

    //TableComposition->setSortingEnabled(true);
    TableComposition->setItemDelegate(new HelpDelegate(TableComposition));
    TableComposition->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    //TableComposition->horizontalHeader()->setStretchLastSection(true);
    TitTableName->setBuddy(TableComposition);

    //кнопка добавления
    QPushButton* bAddRow = new QPushButton("Добавить компонент");
    connect(bAddRow,SIGNAL(clicked()),SLOT(addIngredient()));
    //кнопка удаления
    QPushButton* bDelRow = new QPushButton("Удалить компонент");
    connect(bDelRow,SIGNAL(clicked()),SLOT(delIngredient()));


    //изображение для рецепта
    QLabel* TitRecImage = new QLabel(trUtf8("Рецепт(изображение)"));
    LabRecImage = new ReceptImage(this);
    LabRecImage->setFixedSize(100,100);
    RecEmptyImg.load(":/Recept_empty");
    LabRecImage->setPixmap(RecEmptyImg);

    //описание рецепта
    TextDescrLabel = new QLabel("&Описание");
    description = new QPlainTextEdit;
    description->setObjectName("Описание");
    TextDescrLabel->setBuddy(description);
    description->setAcceptDrops(false);

    //комментарий к рецепту
    QLabel* TextCommentLabel = new QLabel("Комментарий");
    RecComment = new QPlainTextEdit;
    RecComment->setObjectName("Рецепт:Комментарий");
    TextCommentLabel->setBuddy(RecComment);
    //description->setAcceptDrops(false);
    //кнопка сохранения
    QPushButton* bSaveToBD = new QPushButton("Сохранить в базе");
    connect(bSaveToBD,SIGNAL(clicked()),SLOT(addToBase()));
    //кнопка очистки формы
    QPushButton* bClearForm = new QPushButton("Очистить форму");
    connect(bClearForm,SIGNAL(clicked()),SLOT(isClear()));

    connect(RecName,SIGNAL(activated(QString)),SLOT(fillOnSelectRec(QString)));
    //Сигналы о наличии несохраненных данных
    connect(RecName,SIGNAL(editTextChanged(QString)),SLOT(isChanged()));
    connect(RecType,SIGNAL(editTextChanged(QString)),SLOT(isChanged()));
    connect(RecSubType,SIGNAL(editTextChanged(QString)),SLOT(isChanged()));
    connect(MainProd,SIGNAL(editTextChanged(QString)),SLOT(isChanged()));
    connect(PostDay,SIGNAL(editTextChanged(QString)),SLOT(isChanged()));
    connect(TableComposition,SIGNAL(cellChanged(int,int)),
                                     SLOT(isChanged()));

    connect(description,SIGNAL(textChanged()),SLOT(isChanged()));
    connect(RecComment,SIGNAL(textChanged()),SLOT(isChanged()));
    //сигналы об изменении содержимого отдельных ячеек
    connect(TableComposition,SIGNAL(cellChanged(int,int)),
                                     SLOT(checkItem(int,int)));
    connect(RecType,SIGNAL(currentIndexChanged(int)),
                                     SLOT(updateListSubType(int)));

    //layouts
    QVBoxLayout* leftlayout = new QVBoxLayout;
    leftlayout->addWidget(TitRecName);
    leftlayout->addWidget(RecName);
    //Горизонтально для полей рецепта тип, подтип, основной компонент
        QGridLayout* reclayout =new QGridLayout;
        //reclayout->setMargin(10);
        //reclayout->setSpacing(10);
        reclayout->addWidget(TypeRecName,0,0);
        reclayout->addWidget(RecType,1,0);
        reclayout->addWidget(SubTypeRecName,2,0);
        reclayout->addWidget(RecSubType,3,0);
        reclayout->addWidget(MainProdRecName,0,1);
        reclayout->addWidget(MainProd,1,1);
        reclayout->addWidget(PostDayName,2,1);
        reclayout->addWidget(PostDay,3,1);
    //конец горизонтального размещения
    leftlayout->addLayout(reclayout);
    leftlayout->addWidget(TitTableName);
    leftlayout->addWidget(TableComposition);
    QHBoxLayout* btnlayout =new QHBoxLayout;
    btnlayout->addWidget(bAddRow);
    btnlayout->addWidget(bDelRow);
    leftlayout->addLayout(btnlayout);

    QVBoxLayout* rightlayout = new QVBoxLayout;
    rightlayout->addWidget(TitRecImage);
    rightlayout->addWidget(LabRecImage);
    rightlayout->addWidget(TextDescrLabel);
    rightlayout->addWidget(description);
    rightlayout->addWidget(TextCommentLabel);
    rightlayout->addWidget(RecComment);
    rightlayout->addWidget(bClearForm);
    rightlayout->addWidget(bSaveToBD);
    resultlayout =new QHBoxLayout;
    resultlayout->addLayout(leftlayout);
    resultlayout->addLayout(rightlayout);

    setLayout(resultlayout);
    init();
}

SQLForm::~SQLForm()
{

}

void SQLForm::fillOnSelectRec(QString SelectItem)
{
    try
    {
        if(listAvailableRecepts.contains(SelectItem))
        {
            int RMainPrID,RtypeID;
            QString Rname,Rid,Rdescr,Rtype,RSubType,RComment,RRacion;
            QStringList Pname,Pcount,Pmetr,Ptype,Pdescr;

            QString sqlq = "SELECT * FROM Recept "
                    "Where Name='%1'";
            QSqlRecord rec;
            QSqlQuery query;

            if(query.exec(sqlq.arg(SelectItem)))
            {
                rec = query.record();
                if(query.next())
                {
                    Rid=query.value(rec.indexOf("ID_REC")).toString();
                    Rname=query.value(rec.indexOf("Name")).toString();
                    Rdescr=query.value(rec.indexOf("Description")).toString();
                    RtypeID=query.value(rec.indexOf("Type")).toInt();
                    //RSubTypeID=query.value(rec.indexOf("Subtype")).toString();
                    RMainPrID=query.value(rec.indexOf("ID_MainPR")).toInt();
                    RComment=query.value(rec.indexOf("Comment")).toString();
                    RRacion=query.value(rec.indexOf("Racion")).toString();
                }
            }
            else
            {
               throw Error::QueryError(sqlq.arg(SelectItem));
            }

            //по найденному рецепту ищем его состав и изображение
            if(!Rname.isEmpty())
            {
                TableComposition->clearContents();
                TableComposition->setRowCount(0);
                MainProd->clear();
                if(query.exec(SQLMAP["GetCompOnRec"].arg(Rid)))
                {
                    rec = query.record();
                    int row = 0;
                    while(query.next())
                    {
                        addIngredient();
                        Pname << query.value(rec.indexOf("Name")).toString();
                        Pcount << query.value(rec.indexOf("Count")).toString();
                        Pmetr << query.value(rec.indexOf("Metric")).toString();
                        Ptype << query.value(rec.indexOf("Type")).toString();
                        Pdescr << query.value(rec.indexOf("Description")).toString();

                    }
                    //qDebug()<<"Lists of types"<<Ptype;
                    while(!Pname.isEmpty())
                    {
                        TableComposition->item(row,ColumnNameCompos::name)->setData(Qt::DisplayRole,Pname.takeLast());
                        TableComposition->item(row,ColumnNameCompos::count)->setData(Qt::DisplayRole,Pcount.takeLast().toDouble());
                        TableComposition->item(row,ColumnNameCompos::metrics)->setData(Qt::DisplayRole,Pmetr.takeLast());
                        TableComposition->item(row,ColumnNameCompos::type)->setData(Qt::DisplayRole,Ptype.takeLast());
                        TableComposition->item(row,ColumnNameCompos::description)->setData(Qt::DisplayRole,Pdescr.takeLast());
                        //TableComposition->item(row,ColumnNameCompos::type)->setFlags(Qt::ItemIsEditable & false);
                        row++;
                    }

                }
                else
                {
                    throw Error::QueryError(SQLMAP["GetCompOnRec"].arg(Rid));
                }
                //Добавляем описание рецепта
                description->document()->setPlainText(Rdescr);
                RecComment->setPlainText(RComment);
                PostDay->setCurrentText(RRacion);
                //поиск по id
                qDebug() << "ID main prod " << RMainPrID << ", id type " << RtypeID;
                MainProd->setCurrentText(findRec("Product","Name","ID_PR",QString::number(RMainPrID)));
                RecType->setCurrentText(findRec("ReceptType","Type","ID_Type",QString::number(RtypeID)));
                RecSubType->setCurrentText(findRec("ReceptType","SubType","ID_Type",QString::number(RtypeID)));
                //

                //и изображение
                QByteArray bytes;
                QBuffer buffer(&bytes);
                query.prepare("SELECT Image FROM Images "
                              "WHERE ID_REC=:ID_REC;");
                query.bindValue(":ID_REC",Rid);
                qDebug() << "Rec_id: " << Rid;
                buffer.open(QIODevice::ReadWrite);
                if(query.exec())
                {
                    rec = query.record();

                    if(query.next())
                    {
                        qDebug() << "Point_one: ";
                        buffer.write(query.value(rec.indexOf("Image")).toByteArray());
                        buffer.seek(0);
                        RecInitImg.loadFromData(buffer.readAll());
                        LabRecImage->setPixmap(RecInitImg);
                    }
                    else
                    {
                        LabRecImage->setPixmap(RecEmptyImg);
                    }
                }
                else
                {
                    qDebug() << "Попытка загрузить картину провалилась: " << query.lastError().text();
                }
                buffer.close();

            }
            changedIsSaved();
        }
        else
        {
            init(this->RecName->currentText());
        }
    }
    catch(Error::QueryError err)
    {
       // qDebug() << "In catch(QueryError)";
        QString msg = "Не удалось выполнить: " + err.data();
        QMessageBox* box= new QMessageBox(QMessageBox::Critical,
                                          "Ошибка выполнения запроса",
                                          msg,
                                          QMessageBox::Cancel);
        box->show();
    }
    // qDebug()<<"In Fill on List";
}

QString SQLForm::findRec(QString TableName, QString SelectColumnName,QString lCondition1, QString rCondition1,
                                                                     QString lCondition2,
                                                                     QString rCondition2)
{
    QSqlQuery query;

    QString sqlq = "SELECT %1 FROM %2 "
                    "Where %3='%4' AND %5='%6'";
    query.exec(sqlq.arg(SelectColumnName).arg(TableName).arg(lCondition1).arg(rCondition1)
                                                        .arg(lCondition2).arg(rCondition2));
    if(lCondition2 != "'0'")
    qDebug()<<"Curr Query is: "<< sqlq.arg(SelectColumnName).arg(TableName).arg(lCondition1).arg(rCondition1)
                                      .arg(lCondition2).arg(rCondition2);
    query.next();
    return(query.value(0).toString());
}

void SQLForm::isChanged()
{
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(255,218,185));
    QWidget* send = qobject_cast<QWidget*>(sender());
    send->setPalette(pal);
    qDebug()<<"set YELLOW to: "<<send->objectName();
    //RecName->setWindowModified(true);
}

void SQLForm::changedIsSaved()
{
    QPalette pal;
    QWidget* curWid;
    pal.setColor(QPalette::Base, Qt::white);
    paintLayout(&pal,layout());
    //RecName->setPalette(pal);
    //this->setWindowModified(false);
}

void SQLForm::isClear()
{
    clearForm(layout());
    MainProd->clear();
}

void SQLForm::clearForm(QLayoutItem *item)
{
    QLayout *layout = item->layout();
    if (layout) {
        for (int i = 0; i < layout->count(); ++i)
            clearForm(layout->itemAt(i));
    }
    if (QComboBox* curWidget = qobject_cast<QComboBox*>(item->widget()))
    {
        qDebug()<<"In clear, QComboBox* widget: "<<curWidget->objectName();
        curWidget->setCurrentText("");
    }
    if (QPlainTextEdit* curWidget = qobject_cast<QPlainTextEdit*>(item->widget()))
    {
        qDebug()<<"In clear, QPlainTextEdit* widget: "<<curWidget->objectName();
        curWidget->clear();
    }
    if (QTableWidget* curWidget = qobject_cast<QTableWidget*>(item->widget()))
    {
        qDebug()<<"In clear, QTableWidget* widget: "<<curWidget->objectName();
        curWidget->clearContents();
        curWidget->setRowCount(0);
        addIngredient();
    }
}

void SQLForm::paintLayout(QPalette *pal, QLayoutItem *item)
{
    QLayout *layout = item->layout();
    if (layout) {
        for (int i = 0; i < layout->count(); ++i)
            paintLayout(pal, layout->itemAt(i));
    }
    if (item->widget()) item->widget()->setPalette(*pal);
}

void SQLForm::addIngredient()
{
    int row = TableComposition->rowCount();
    TableComposition->insertRow(row);

    QTableWidgetItem* item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    TableComposition->setItem(row,0,item0);
    //qDebug() << "In add Name...";

    QTableWidgetItem* item1 = new QTableWidgetItem;
    item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    TableComposition->setItem(row,1,item1);
    //qDebug() << "In add Count...";

    QTableWidgetItem* item2 = new QTableWidgetItem;
    item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    TableComposition->setItem(row,2,item2);

    QTableWidgetItem* item3 = new QTableWidgetItem;
    item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    TableComposition->setItem(row,3,item3);

    QTableWidgetItem* item4 = new QTableWidgetItem;
    item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    TableComposition->setItem(row,4,item4);

    fitToSize();

    TableComposition->setCurrentItem(item0);
}

void SQLForm::delIngredient()
{
    QString item = TableComposition->item(TableComposition->currentRow(),ColumnNameCompos::name)
                                   ->data(Qt::DisplayRole).toString();
    MainProd->removeItem(MainProd->findText(item));
    TableComposition->removeRow(TableComposition->currentRow());
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(255,218,185));
    TableComposition->setPalette(pal);
}

void SQLForm::fitToSize()
{

    //TableComposition->resizeColumnsToContents();

    //TableComposition->resize(QSize(TableComposition->horizontalHeader()->length()+TableComposition->verticalHeader()->width(),
    //                               TableComposition->verticalHeader()->length()));

    //updateGeometry();
}

void SQLForm::checkItem(int row,int column)
{
    if(column==ColumnNameCompos::name)
    {
        qDebug() << "In check item";
        QString metrics,type;
        QSqlQuery queryForProd;
        QString value = TableComposition->item(row,column)->data(Qt::DisplayRole).toString();
        if(!value.isEmpty())
        {
            if(row>(listforcurProdMetrics.size()-1)) listforcurProdMetrics.resize(row+1);
            listforcurProdMetrics[row].clear();
            if(!listAvailableNames.contains(value))
            {
                TableComposition->item(row,column)->setBackground(Qt::yellow);
                return;
            }

            //заполняем лист текущих доступных метрик
            QSqlQuery queryForProd;
            if(queryForProd.exec(SQLMAP["GetlistMetrForProd"].arg(TableComposition->item(row,ColumnNameCompos::name)->data(Qt::DisplayRole).toString())))
                while(queryForProd.next())
                {
                    listforcurProdMetrics[row]<< queryForProd.value(0).toString();
                }
            //заполняем тип состава из базы(если 0, то разрешаем редактирование, иначе запрещаем
            type = "SELECT Type "
                    "FROM Product "
                    "WHERE Name = '%1';";
            if(queryForProd.exec(type.arg(value)))
                if(queryForProd.next())
                {
                    TableComposition->item(row,ColumnNameCompos::type)->setData(Qt::DisplayRole,queryForProd.value(0).toString());
                }
            if(queryForProd.value(0).toString().isEmpty())
                TableComposition->item(row,ColumnNameCompos::type)->setFlags(TableComposition->item(row,ColumnNameCompos::type)->flags()
                                                                             | Qt::ItemIsEditable);
            else
                TableComposition->item(row,ColumnNameCompos::type)->setFlags(TableComposition->item(row,ColumnNameCompos::type)->flags()
                                                                             & ~Qt::ItemIsEditable);
            //заполняем лист доступных основных продуктов
            MainProd->addItem(value);

        }
    }

}

void  SQLForm::addToBase()
{
    QSqlRecord rec;
    QSqlQuery query;
    QString lastid = SQLMAP["LASTID"];
    QComboBox* recept = findChild<QComboBox*>("Рецепт");
    QPlainTextEdit* descr = findChild<QPlainTextEdit*>("Описание");
    QString curRecId;
    QString addComposition;

    //Проверка на пустые поля
    if(!recept || recept->currentText().isEmpty() || TableComposition->rowCount()==0
                                                  || TableComposition->item(0,0)->data(Qt::DisplayRole).toString().isEmpty()
                                                  || TableComposition->item(0,1)->data(Qt::DisplayRole).toString().isEmpty()
                                                  || TableComposition->item(0,2)->data(Qt::DisplayRole).toString().isEmpty()
                                                  || descr && descr->document()->isEmpty())
    {
        QMessageBox* msg= new QMessageBox(QMessageBox::Critical,
                                          "Садомия",
                                          "<b>Не все поля заполнены<b>",
                                          QMessageBox::Cancel);
        msg->show();
        return;
    }

    QMap<int,SQLDataAddProduct> SQLcomposelist;

    //Заполняем таблицу Продукты
    try
    {
        for(int row=0;row<TableComposition->rowCount();row++)
        {
            for(int column=0;column<TableComposition->columnCount();column++)
            {
                //if(column==ColumnNameCompos::name && TableComposition->item(row,column)->backgroundColor()!=Qt::yellow) break;
                SQLcomposelist[row].data[column] = TableComposition->item(row,column)->data(Qt::DisplayRole).toString();

            }
            if(!SQLcomposelist[row].data[0].isEmpty())
            {
                QString isProdExist = "SELECT * FROM Product "
                                      "Where Name='%1' AND Metric='%2'";
                //qDebug() << "Имя продукта:" << SQLcomposelist[row].data[0] << " Метрика:" << SQLcomposelist[row].data[2];
                if(query.exec(isProdExist.arg(SQLcomposelist[row].data[0]).arg(SQLcomposelist[row].data[2])))
                {
                    if(!query.next())
                    {
                        //Если пара продукт-метрика не существует name=0,count,metrics,type,description
                        QString addProd = SQLMAP["ADDPROD"].arg(SQLcomposelist[row].data[ColumnNameCompos::name])
                                                           .arg(SQLcomposelist[row].data[ColumnNameCompos::type])
                                                           .arg(SQLcomposelist[row].data[ColumnNameCompos::metrics])
                                                           .arg(SQLcomposelist[row].data[ColumnNameCompos::description]);
                       // qDebug() << "Запрос добавления продукта: " << addProd;
                        if(query.exec(addProd))
                        {
                            if(query.exec(lastid))
                            {
                                if(query.next())  SQLcomposelist[row].curProdId = query.value(0).toString();
                            }
                        }
                        else throw Error::QueryError(addProd);
                    }
                    else
                    {
                        SQLcomposelist[row].curProdId = query.value(0).toString();
                        QString updateProd = "UPDATE Product SET Type = '%1',Description = '%2' "
                                             "Where ID_PR='%3';";
                        query.exec(updateProd.arg(SQLcomposelist[row].data[ColumnNameCompos::type])
                                             .arg(SQLcomposelist[row].data[ColumnNameCompos::description])
                                             .arg(SQLcomposelist[row].curProdId));
                        //  qDebug() << "Текущий ID продукта:" << SQLcomposelist[row].curProdId;
                    }
                }
                else throw Error::QueryError(isProdExist);
            }
        }

        //Заполняем таблицу Рецепты
        QString isRecAlreadyExist = "SELECT ID_REC,Name FROM Recept "
                                    "Where Name='%1';";
        int MainProdID = findRec("Product","ID_PR","Name",MainProd->currentText()).toInt();
        int RecTypeID = findRec("ReceptType","ID_Type","Type",RecType->currentText(),"SubType",RecSubType->currentText()).toInt();

        QString addRecept         = SQLMAP["ADDRECEPT"].arg(recept->currentText())
                                                       .arg(descr->document()->toPlainText())
                                                       .arg(RecTypeID)
                                                       .arg(RecComment->document()->toPlainText())
                                                       .arg(MainProdID)
                                                       .arg(PostDay->currentText());

        if(query.exec(isRecAlreadyExist.arg(recept->currentText())))
        {
            if(!query.next())
                if(query.exec(addRecept))
                {
                    if(query.exec(lastid))
                        if(query.next())
                            curRecId = query.value(0).toString();
                }
                else
                {
                    throw Error::QueryError(addRecept);
                }
            else
            {
                curRecId = query.value(0).toString();
                QString upDescr = "UPDATE Recept SET Description = '%1', Type = '%2', Comment = '%3', Racion = '%6', "
                                  "ID_MainPR = (SELECT ID_PR FROM Product WHERE Name = '%4') "
                                            "Where ID_REC='%5';";
                if(!query.exec(upDescr.arg(descr->document()->toPlainText())
                                      .arg(RecTypeID)
                                      .arg(RecComment->document()->toPlainText())
                                      .arg(MainProd->currentText())
                                      .arg(curRecId)
                                      .arg(PostDay->currentText())))
                throw Error::QueryError(upDescr.arg(descr->document()->toPlainText())
                                        .arg(RecTypeID)
                                        .arg(RecComment->document()->toPlainText())
                                        .arg(MainProd->currentText())
                                        .arg(curRecId)
                                        .arg(PostDay->currentText()));
            }
        }
        else
        {
           throw Error::QueryError(isRecAlreadyExist.arg(recept->currentText()));
        }

        //***********Заполняем таблицу Состав
        QString delComposition = "DELETE FROM Composition "
                        "WHERE ID_REC = '%1';";

        //сначала удалим все записи данного рецепта
        query.exec(delComposition.arg(curRecId));

        //добавляем состав
        for(int count=0;count<TableComposition->rowCount();count++)
        {

            addComposition = SQLMAP["COMPOSITION"].arg(SQLcomposelist[count].curProdId)
                    .arg(curRecId)
                    .arg(TableComposition->item(count,ColumnNameCompos::count)->data(Qt::DisplayRole).toString());

            if(!query.exec(addComposition)) throw Error::QueryError(addComposition);
        }

        //***********Заполняем таблицу изображений
        if(LabRecImage->pixmap()->cacheKey() != RecInitImg.cacheKey())
        {
            const QPixmap *curImage = LabRecImage->pixmap();
            QByteArray bytes;
            QBuffer buffer(&bytes);
            //сохраняем изображение в двоичном формате
            buffer.open(QIODevice::ReadWrite);
            curImage->save(&buffer,"JPG");
            query.prepare("SELECT * FROM Images "
                          "WHERE ID_REC=:ID_REC");
            query.bindValue(":ID_REC",curRecId);
            //если уже есть сохраненная картинка, то обновим
            if(query.exec() && query.next())
            {
                query.prepare("UPDATE Images SET Image =:Image "
                              "Where ID_REC=:ID_REC;");
                query.bindValue(":ID_REC",curRecId);
            }
            else
            {
                query.prepare("INSERT INTO Images (ID_REC,Image) "
                              "VALUES (:ID_REC,:Image);");
                query.bindValue(":ID_REC",curRecId);
            }
            buffer.seek(0);
            query.bindValue(":Image", buffer.readAll());
            buffer.close();
            if(!query.exec())
                qDebug() << "Попытка сохранить картину провалилась: " << query.lastError().text();
        }
        init(RecName->currentText());
        changedIsSaved();
    }
    catch(Error::QueryError err)
    {
       // qDebug() << "In catch(QueryError)";
        QString msg = "Не удалось выполнить: " + err.data();
        QMessageBox* box= new QMessageBox(QMessageBox::Critical,
                                          "Ошибка выполнения запроса",
                                          msg,
                                          QMessageBox::Cancel);
        box->show();
    }

}

void SQLForm::resizeEvent(QResizeEvent *pe)
{
    ;//
}
void SQLForm::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void SQLForm::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if(urls.isEmpty())
    {
        return;
    }

    QString path = urls.first().toLocalFile();

    if(path.endsWith(".png") || path.endsWith(".jpg") || path.endsWith(".bmp"))
    {
        qDebug() << "Path is: " << path;
        QPixmap img;
        img.load(path);
        img = img.scaled(this->LabRecImage->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        this->LabRecImage->setPixmap(img);
        return;
    }
    if(path.endsWith(".txt") || path.endsWith(".doc"))
    {
        QString text;
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            while(!file.atEnd()) text+=trUtf8(file.read(file.size()));
            this->description->setPlainText(text);
        }
    }
    
}
