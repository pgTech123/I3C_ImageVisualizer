/*************************************************************************
 * Project:     GV3D File
 *
 * File Name:   GVQLabel.cpp
 *
 * Author:      Pascal Gendron
 *
 * Version:     0.0.1
 * ************************************************************************/

#include "gvqlabel.h"

GVQLabel::GVQLabel()
{
    presetWidgetSizeAndAngles();

    /* The Default Image Loaded is at that Location */
    initializeRenderingWidget(DEFAULT_IMAGE_PATH);
}

GVQLabel::GVQLabel(const char* p_cPath)
{
    presetWidgetSizeAndAngles();
    /* Load the Image from the Specified Location */
    initializeRenderingWidget(p_cPath);
}

GVQLabel::~GVQLabel()
{
    if(GV != NULL){
        delete GV;
    }
}

void GVQLabel::setPathToImage(const char *p_cPath)
{
    /* Delete Previous Image */
    if(GV != NULL){
        disconnect(refreshTimer,SIGNAL(timeout()),this,SLOT(refreshScreen()));
        delete GV;
    }

    /* Creation of the 3D image */
    GV = new GVImage();
    if(GV->openFile(p_cPath) == NO_ERRORS){
        /* Retrieve Size and Pointer to Data from 3D Model */
        m_intHeight = GV->getHeight();
        m_intWidth = GV->getWidth();
        m_ucharPixels = GV->getData();

        connect(refreshTimer,SIGNAL(timeout()),this,SLOT(refreshScreen()));
    }
    else{
        delete GV;
        GV = NULL;

        /* Warning Message */
    }

}

void GVQLabel::presetWidgetSizeAndAngles()
{
    /* x0.8 to be able to resize down */
    m_iWidgetWidth = this->width()*0.8;
    m_iWidgetHeight = this->height()*0.8;

    angleTheta = 0;
    anglePhi = 0;

    GV = NULL;
}

void GVQLabel::initializeRenderingWidget(const char* p_cPath)
{
    /* Refresh Image Callback */
    refreshTimer = new QTimer(this);
    refreshTimer->start(60);

    setPathToImage(p_cPath);

    /* Frame Initialization */
    frame = new QPixmap();
    frame->fill(Qt::black);

    /* Display Initial Frame */
    QLabel::setPixmap(*frame);
}

void GVQLabel::resizeEvent(QResizeEvent *event)
{
    /* x0.8 to be able to resize down */
    m_iWidgetWidth = event->size().width()*0.8;
    m_iWidgetHeight = event->size().height()*0.8;
}

void GVQLabel::setAngles(double angleX, double angleY)
{
    angleTheta = angleX;
    anglePhi = angleY;
}

void GVQLabel::setAngleX(double angleX)
{
    angleTheta = angleX;
}

void GVQLabel::setAngleY(double angleY)
{
    anglePhi = angleY;
}

void GVQLabel::refreshScreen()
{
    /* Image Preparation and Rendering */
    GV->setRotation(angleTheta, anglePhi);
    GV->generateImage();

    /* Fit Pixel Map for the Window Resolution */
    QImage img(m_ucharPixels, m_intWidth, m_intHeight, 3*m_intWidth, QImage::Format_RGB888); // 2 pixels width, 2 pixels height, 6 bytes per line, RGB888 format
    QImage scaled = img.scaled(m_iWidgetWidth, m_iWidgetHeight,Qt::KeepAspectRatio); // Scale image to show results better
    QPixmap pix = QPixmap::fromImage(scaled); // Create pixmap from image

    /* Set the this Class as the Pixel Map of the Rendered Image */
    QLabel::setPixmap(pix);
}
