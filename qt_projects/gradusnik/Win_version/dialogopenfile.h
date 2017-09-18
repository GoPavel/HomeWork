#ifndef DIALOGOPENFILE_H
#define DIALOGOPENFILE_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

class DialogOpenFile: public QDialog {
    Q_OBJECT

public:
    explicit DialogOpenFile(QWidget *parent = 0);

private:
    QPushButton *butCancel, *butOk;
    QLineEdit *lineEditPath;
public slots:
    void sendPath();
signals:
    void  sendingPath(QString);
};

#endif // DIALOGOPENFILE_H
