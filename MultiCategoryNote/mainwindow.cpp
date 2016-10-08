#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("objects.db");
    if (!db.open())
    {
        qDebug("can not open objects table");
    }

    QSqlQuery q;
    if (q.exec("select * from objects where guid=\"{00000000-1234-5678-0000-000000000000}\"") == false)
    {
        qDebug("select");
    }

    if (q.next())
    {
        QString uuid = q.value(0).toString();
        QString content = q.value(1).toString();

        qDebug() << uuid << "\t" << content << endl;

        ui->noteTreeWidget->clear();
        ui->noteTreeWidget->setColumnCount(1);


        // 1. 创建 QJsonParseError 对象，用来获取解析结果
        QJsonParseError error;
        // 2. 使用静态函数获取 QJsonDocument 对象
        QJsonDocument jsonDocument = QJsonDocument::fromJson(content.toUtf8(), &error);
        // 3. 根据解析结果进行处理
        if (error.error == QJsonParseError::NoError) {
            if (!(jsonDocument.isNull() || jsonDocument.isEmpty())) {
                if (jsonDocument.isObject()) {
                    QJsonObject json = jsonDocument.object();
                    if( json.value(QString("type")).toString() == QString("tree")) {
                      QString title = json.value("name").toString();

                      // add a note item to the tree
                      QTreeWidgetItem *rootItem = new QTreeWidgetItem();
                      rootItem->setText(0, title);
                      ui->noteTreeWidget->addTopLevelItem(rootItem);
                      QJsonArray items=json.value("items").toArray();
                      for(int i=0;i<items.count();i++)
                      {
                        QTreeWidgetItem *subItem = new QTreeWidgetItem();
                        QJsonObject o = items[i].toObject();
                        QString t = o.value("name").toString();
                        subItem->setText(0,t);
                        rootItem->addChild(subItem);
                      }
                    }
                } else if (jsonDocument.isArray()) {
                    // ...
                }
            }
        } else {
            // 检查错误类型
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
