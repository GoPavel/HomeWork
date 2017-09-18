#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include "dialogopenfile.h"

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QChartView* chartView;

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void readFileAndDrawChart(QString path);
    void openDialogOpenFile();
};

#endif // MAINWINDOW_H
