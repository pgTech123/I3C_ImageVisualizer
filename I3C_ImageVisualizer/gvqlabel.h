/*************************************************************************
 * Project:     GV3D File
 *
 * File Name:   GVQLabel.h
 *
 * Author:      Pascal Gendron
 *
 * Version:     0.0.1
 * ************************************************************************/

#ifndef GVQLABEL_H
#define GVQLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QResizeEvent>
#include "gvimage.h"

/*************************************************************************
 * GVQLabel Class:
 *
 * The purpose of this class is to provide a graphic layout to display the
 * image rendered.
 *************************************************************************/

#define DEFAULT_IMAGE_PATH  "./default.i3c"

class GVQLabel : public QLabel
{
    Q_OBJECT

public:
    explicit GVQLabel();
    GVQLabel(const char* p_cPath);
    ~GVQLabel();

    void setPathToImage(const char* p_cPath);

public slots:
    void setAngles(double angleX, double angleY);
    void setAngleX(double angleX);
    void setAngleY(double angleY);

protected:
    void resizeEvent(QResizeEvent *);

private:
    void initializeRenderingWidget(const char* p_cPath);
    void presetWidgetSizeAndAngles();

private slots:
    void refreshScreen();

private:
    /* Qt Grapical Window & Screen Refresh Timer */
    QPixmap *frame;
    QTimer *refreshTimer;

    /* 2D Image (Rendered) */
    int m_intWidth;
    int m_intHeight;
    unsigned char *m_ucharPixels;

    /* 3D Image */
    GVImage *GV;

    /* TEST ANGLE */
    double angleTheta;
    double anglePhi;

    /* Auto-Resize Widget */
    int m_iWidgetWidth;
    int m_iWidgetHeight;
};

#endif // GVQLABEL_H
