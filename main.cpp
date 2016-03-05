#include <QtPlugin>
#include <QApplication>
#include "global.h"
#include "u_error.h"
#include "sqlform.h"

//Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

typedef QMap<QString,QString> TSQLMAP;
TSQLMAP SQLMAP;
QStringList listAvailableMetrics,
            listAvailableNames,
            listAvailableTypes,
            listAvailableRecepts,
            listAvailTypeRec,
            listAvailRacions,
            listAvailSubTypeRec;

QVector<QStringList> listforcurProdMetrics;

void SQLinit();

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = QFileDialog::getOpenFileName(0,"Выбрать файл БД","","*.sqlite");
    //QString path ="RussianCook.sqlite";
    db.setDatabaseName(path);
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    QStringList tables = db.tables();
    foreach (QString str, tables) {
        qDebug() << "Table is: " << str;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!createConnection()) {
        return -1;
    }
    SQLinit();
    SQLForm w;
    w.show();

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    return a.exec();
}

void SQLinit()
{
    SQLMAP["CMPITEM"] = "SELECT Name,Metric "
                        "FROM Product "
                        "WHERE Name = '%1';";

    SQLMAP["LASTID"] =  "SELECT last_insert_rowid() ";

    SQLMAP["ListMetrics"] = "SELECT DISTINCT Metric"
                            " FROM Product;";

    SQLMAP["GetCompOnRec"] = "SELECT Product.Name, Composition.Count, Product.Metric, Product.Type, Product.Description "
                            "FROM Composition, Product "
                            "Where (Composition.ID_PR = Product.ID_PR) AND Composition.ID_REC = '%1';";
    SQLMAP["GetlistMetrForProd"] = "SELECT Metric "
                                   "FROM Product "
                                   "Where Name = '%1';";

    SQLMAP["ADDRECEPT"] = "INSERT INTO Recept (Name,Description,Type,Comment,ID_MainPR,Racion) "
                        "VALUES('%1','%2','%3','%4','%5','%6');";

    SQLMAP["COMPOSITION"] = "INSERT INTO Composition (ID_PR,ID_REC,Count) "
                            "VALUES('%1','%2','%3');";

    SQLMAP["ADDPROD"] = "INSERT INTO Product (Name,Type,Metric,Description) "
                        "VALUES('%1','%2','%3','%4');";
}
