#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_bRotationAuto = false;

    m_dAngleX = 0;
    m_dAngleY = 0;

    AutorotateTimer = new QTimer(this);
    AutorotateTimer->start(30);

    renderingWidget = new GVQLabel(DEFAULT_FILE_PATH);
    ui->gridLayout->addWidget(renderingWidget,2,2);

    ui->centralwidget->setLayout(ui->gridLayout);

    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(setAngleX(int)));
    connect(ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(setAngleY(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent*)
{
    qApp->exit();
}

void MainWindow::setAngleX(int value)
{
    renderingWidget->setAngleX((double)value/30);
    m_dAngleX = value/30;
}

void MainWindow::setAngleY(int value)
{
    renderingWidget->setAngleY((double)value/30);
    m_dAngleY = value/30;
}

void MainWindow::on_pushButtonStart_clicked()
{
    if(m_bRotationAuto){
        m_bRotationAuto = false;
        ui->pushButtonStart->setText("Start");
        disconnect(AutorotateTimer,SIGNAL(timeout()), this, SLOT(incrementRotation()));
    }
    else{
        m_bRotationAuto = true;
        ui->pushButtonStart->setText("Stop");
        connect(AutorotateTimer,SIGNAL(timeout()), this, SLOT(incrementRotation()));
    }

}

void MainWindow::incrementRotation()
{
    m_dAngleX += (double)ui->horizontalSlider->value()/200;
    renderingWidget->setAngleX(m_dAngleX);

    m_dAngleY += (double)ui->verticalSlider->value()/200;
    renderingWidget->setAngleY(m_dAngleY);
}

void MainWindow::on_actionOpen_triggered()
{
    QString pathToImage = QFileDialog::getOpenFileName(this, "Load 3D Image",
                                              QString(), "3D Image File(*.i3c)");
    renderingWidget->setPathToImage(pathToImage.toStdString().c_str());
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionAbout_I3C_Visualizer_triggered()
{
    aboutUsWindow.show();
}
