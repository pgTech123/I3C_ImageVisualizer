#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QTimer>
#include "gvqlabel.h"
#include "aboutus.h"

/* Default 3D Image Path */
#define DEFAULT_FILE_PATH           "./../Default.i3c"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* Events */
    void closeEvent(QCloseEvent* event);

private slots:
    void incrementRotation();

    void setAngleX(int value);
    void setAngleY(int value);
    void on_pushButtonStart_clicked();

    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_I3C_Visualizer_triggered();

private:
    Ui::MainWindow *ui;
    GVQLabel *renderingWidget;
    QTimer *AutorotateTimer;

    AboutUs aboutUsWindow;

    bool m_bRotationAuto;

    double m_dAngleX;
    double m_dAngleY;
};

#endif // MAINWINDOW_H
