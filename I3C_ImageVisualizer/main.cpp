/***************************************************************************
 * GV3D File Reader
 *
 * Prototype testing if it would be thinkable to render 3D images.
 * The hypothesis is that if you store 3D model as a map of pixels
 * (in the 3 dimentions)it would be faster to render and would give
 * more realistic pictures than those obtained by rendering 3D model
 * stored as polygons.
 *
 * The idea is first to render images to develop the rendering algoritm
 * techniques and then extend it to 3D video files.
 *
 * This project focusses on rendering a 3D image.
 *
 *
 * Author:      Pascal Gendron
 *
 * Purpose:     Develop and test 3D images rendering process
 *
 * Reference:   Use the Qt library (QApplication)
 *
 * File format: The program is design to read .I3C files. The description
 *              of the file will soon be added into the project directory.
 *
 * Version:     0.0.1
 *
 **************************************************************************/

#include <QApplication>
#include <QIcon>
#include "mainwindow.h"

/* Program Parameters */
#define ICON_PATH           "./../GVisionLogo.ico"
#define APPLICATION_NAME    "GV3DFile"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow *window = new MainWindow();
    QIcon *icon = new QIcon(ICON_PATH);

    /* Set Window Parameters */
    window->setGeometry(100,100,720,405);
    window->setWindowTitle(APPLICATION_NAME);
    window->setWindowIcon(*icon);

    /* Display Window */
    window->show();

    return app.exec();
}
