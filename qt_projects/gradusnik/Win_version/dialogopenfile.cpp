#include "dialogopenfile.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QString>
#include <QMessageBox>

DialogOpenFile::DialogOpenFile(QWidget *parent): QDialog(parent) {

    butCancel = new QPushButton("cancel");
    butOk = new QPushButton("ok");
    lineEditPath = new QLineEdit("Example: C:\\test.txt");

    //  <set layout>
    QHBoxLayout* butLayout = new QHBoxLayout();
    QVBoxLayout* vlayout = new QVBoxLayout();

    butLayout->addWidget(butOk);
    butLayout->addWidget(butCancel);
    vlayout->addWidget(lineEditPath);
    vlayout->addLayout(butLayout);

    setLayout(vlayout);
    // </set layout>

    // <connect>
    connect(butCancel, SIGNAL(clicked()), this, SLOT(close()) );
    connect(butOk, SIGNAL(clicked()), this, SLOT(sendPath()));
    // </connect>
}
void DialogOpenFile::sendPath() {
    QString path = lineEditPath->text();
    QFile file(path);
    if (!(file.open(QIODevice::ReadOnly | QIODevice::Text) && QFile::exists(path))) {
        QMessageBox *messageBoxError = new QMessageBox();
        messageBoxError->setText("Error! path isn't correct.");
        messageBoxError->exec();
        file.close();
    } else {
        file.close();
        emit sendingPath(path);
        this->close();
    }
}
