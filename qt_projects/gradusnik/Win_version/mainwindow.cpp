#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QtCharts/QLineSeries>
#include <QtGlobal>
#define MAX_REAL qreal(-200000)

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // <members>
        chartView = new QChartView();
    // </memvers>

    // <menu and actions>
        QMenu* menuEdit = menuBar()->addMenu("Edit");

        QAction* printFile = new QAction(tr("Print chart"), this);

        menuEdit->addAction(printFile);
    // </menu and actions>

    // <connect>
        connect(printFile, SIGNAL(triggered()), this, SLOT(openDialogOpenFile()));
    // </connect>

    setCentralWidget(chartView);
}

void MainWindow::openDialogOpenFile() {
    DialogOpenFile* dialog = new DialogOpenFile();
    connect(dialog, SIGNAL(sendingPath(QString)), this, SLOT(readFileAndDrawChart(QString)));
    dialog->exec();
}

void MainWindow::readFileAndDrawChart(QString path) {
    QFile* file = new QFile(path);
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(file);
    QLineSeries* values = new QLineSeries();
    for (qreal x, x_old = MAX_REAL, y; !stream.atEnd(); ) {
        stream >> x >> y;
        if (x_old < x) {
            values->append(x, y);
            x_old = x;
        }
    }
    //<setting chart>
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(values);
    chart->setTitle("<b>Chart: </b>" + path);
    chart->createDefaultAxes();
    //</setting chart>

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    file->close();
}
