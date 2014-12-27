/*************************************************************************
 * Project:     GV3D File
 *
 * File Name:   GVImage.cpp
 *
 * Author:      Pascal Gendron
 *
 * Version:     0.0.1
 * ************************************************************************/

#include "gvimage.h"

GVImage::GVImage()
{
    initializeImage();
}

GVImage::GVImage(const char* p_cFilename)
{
    initializeImage();
    openFile(p_cFilename);
}

GVImage::~GVImage()
{
    if(m_bImageStored == true){
        deleteCurrentImage();
    }
}

void GVImage::initializeImage()
{
    m_bImageStored =false;
    m_dTheta = 0;
    m_dPhi = 0;
}

int GVImage::openFile(const char* p_cFilename)
{
    fstream file;

    file.open(p_cFilename);

    if(file.is_open()){
        int iError = readImageFile(&file);
        file.close();
        return iError;
    }
    else{
        return UNABLE_TO_OPEN_FILE;
    }
}

int GVImage::readImageFile(fstream *file)
{
    /* Delete previous image data */
    if(m_bImageStored == true){
        deleteCurrentImage();
    }

    int iError;

    /* Read Cube Side Lenght */
    int iSideLenghtUnverified;
    *file >> iSideLenghtUnverified;
    iError = verifyImageSideLenght(iSideLenghtUnverified);
    if(iError != NO_ERRORS){
        return iError;
    }
    //cout << "Side Lenght: " << m_iSideLenght << endl;     /* Debug */

    /* Preparing to Read Image*/
    setImageProperties();
    setImageCenterPoint();
    transform.setUnrotatedCornersCorners(m_iCenterPointX, m_iCenterPointY, m_iSideLenght);
    setNumberOfLevels();

    /* Read Data */
    readNumOfMaps(file);
    iError = readCubes(file);
    if(iError != NO_ERRORS){
        return iError;
    }

    return NO_ERRORS;
}

int GVImage::verifyImageSideLenght(int iSideLenght)
{
    if(iSideLenght <= 1){
        m_iSideLenght = 0;
        return INVALID_CUBE_SIZE;
    }
    else if(!isBase2(iSideLenght)){
        m_iSideLenght = 0;
        return SIZE_NOT_BASE_2;
    }
    else{
        m_iSideLenght = iSideLenght;
        return NO_ERRORS;
    }
}

void GVImage::setImageProperties()
{
    /* Viewport Defined as 2x cube lenght */
    m_p_iImageWidth = new int(2*m_iSideLenght);
    m_p_iImageHeight = new int(2*m_iSideLenght);

    m_p_ucImageData = new unsigned char[(*m_p_iImageWidth) * (*m_p_iImageHeight) * NUMBER_OF_CHANNELS];
    m_p_bPixelFilled = new bool[(*m_p_iImageWidth) * (*m_p_iImageHeight)];
}

void GVImage::setImageCenterPoint()
{
    m_iCenterPointX = (*m_p_iImageWidth)/2;
    m_iCenterPointY = (*m_p_iImageHeight)/2;
}

void GVImage::setNumberOfLevels()
{
    m_iNumberOfLevels = firstHighBit(m_iSideLenght);

    /* Debug */
    //cout << "Number of Levels: " << m_iNumberOfLevels << endl;

    m_iArrCubeAtLevel = new int[m_iNumberOfLevels];
}

void GVImage::readNumOfMaps(fstream *file)
{
    int iBuffer = 0;
    m_iNumberOfCubes = 0;

    /* Store cubes at each level and count the total number of cubes */
    for(int i = 0; i < m_iNumberOfLevels; i++)
    {
        *file >> iBuffer;
        m_iArrCubeAtLevel[i] = iBuffer;
        m_iNumberOfCubes += iBuffer;
    }

    /* Debug */
    //cout << "Number of Cubes: " << m_iNumberOfCubes << endl;

    /* One of the cube is 'this' so we remove 1 */
    m_iNumberOfCubes--;

    /* Create Cube Pointer Array */
    m_p_GVImageArray = new GVIndexCube*[m_iNumberOfCubes];
}

int GVImage::readCubes(fstream *file)
{
    int iError = readPixelCubes(file);
    if(iError != NO_ERRORS){
        return iError;
    }
    iError = readIndexCubes(file);
    if(iError != NO_ERRORS){
        return iError;
    }

    return NO_ERRORS;
}

int GVImage::readPixelCubes(fstream *file)
{
    unsigned char ucMap = 0;
    int iBufRedArr[8];
    int iBufGreenArr[8];
    int iBufBlueArr[8];
    int iCubeBeingWritten = 0;
    int iNumOfPixels;
    int iError;

    /* Read Pixel Cubes */
    for(int i = 0; i < m_iArrCubeAtLevel[0]; i++)
    {
        /* Create Cube */
        m_p_GVImageArray[iCubeBeingWritten] = new GVIndexCube(&*m_p_iImageWidth,
                                                                  &*m_p_iImageHeight,
                                                                  &*m_p_ucImageData,
                                                                  &*m_p_bPixelFilled);
        iError = readMap(file, &ucMap, &iNumOfPixels);
        if(iError != NO_ERRORS){
            return iError;
        }

        for(int j = 0; j < iNumOfPixels; j++)
        {
            //PIXEL READING
            *file >> iBufRedArr[j];
            *file >> iBufGreenArr[j];
            *file >> iBufBlueArr[j];
        }

        if(m_iNumberOfLevels == 1){
            this->addPixelsCube(ucMap,
                                iBufRedArr,
                                iBufGreenArr,
                                iBufBlueArr);
        }
        else{
            m_p_GVImageArray[iCubeBeingWritten]->addPixelsCube(ucMap,
                                                                iBufRedArr,
                                                                iBufGreenArr,
                                                                iBufBlueArr);
        }

        iCubeBeingWritten ++;
    }
    return NO_ERRORS;
}

int GVImage::readIndexCubes(fstream *file)
{
    unsigned char ucMap = 0;
    int iNumOfChild = 0;
    int iAddressCubesCursorOffset = 0;
    int iCubeBeingWritten = m_iArrCubeAtLevel[0];
    int iError;

    for(int level = 1; level < m_iNumberOfLevels; level++)
    {
        for(int i = 0; i < m_iArrCubeAtLevel[level]; i++)
        {
            iError = readMap(file, &ucMap, &iNumOfChild);
            if(iError != NO_ERRORS){
                return iError;
            }

            /* Set cube with child addresses */
            if(m_iNumberOfLevels == level+1){
                //cout << "Master Cube" << endl;
                this->addReferenceCube(ucMap, &m_p_GVImageArray[iAddressCubesCursorOffset]);
            }
            else{
                m_p_GVImageArray[iCubeBeingWritten] = new GVIndexCube(&*m_p_iImageWidth,
                                                                          &*m_p_iImageHeight,
                                                                          &*m_p_ucImageData,
                                                                          &*m_p_bPixelFilled);

                m_p_GVImageArray[iCubeBeingWritten]->addReferenceCube(ucMap,
                                                                      &m_p_GVImageArray[iAddressCubesCursorOffset]);
            }

            /* Update Offset */
            iAddressCubesCursorOffset += iNumOfChild;
            iCubeBeingWritten++;
        }
    }

    return NO_ERRORS;
}

int GVImage::readMap(fstream *file, unsigned char* ucMap, int* iNumOfPix)
{
    int iBufMap;
    *file >> iBufMap;
    *iNumOfPix = numberHighBits(iBufMap);
    if(*iNumOfPix > 8){
        return FILE_CORRUPTED;
    }
    *ucMap = (unsigned char)iBufMap;

    /* Debug */
    //cout << "Number of Pixels in Cube: " << *iNumOfPix << endl;
    //cout << "Map: " << iBufMap << endl;

    return NO_ERRORS;
}

void GVImage::deleteCurrentImage()
{
    delete m_p_iImageWidth;
    delete m_p_iImageHeight;
    delete[] m_p_ucImageData;
    delete[] m_p_bPixelFilled;

    for(int i = 0; i< m_iNumberOfCubes ; i++)
    {
        delete m_p_GVImageArray[i];
    }
    delete[] m_p_GVImageArray;

}

int GVImage::getWidth()
{
    return *m_p_iImageWidth;
}

int GVImage::getHeight()
{
    return *m_p_iImageHeight;
}

unsigned char* GVImage::getData()
{
    return m_p_ucImageData;
}

void GVImage::setRotation(double dTheta, double dPhi)
{
    m_dTheta = dTheta;
    m_dPhi = dPhi;
}

void GVImage::generateImage()
{
    /* Initialize every pixels as empty */
    for(int i = 0; i < (*m_p_iImageWidth)*(*m_p_iImageHeight); i++)
    {
        m_p_bPixelFilled[i] = false;
    }

    /* Compute cube corners projected on the frame */
    for(int i = 0; i < 8; i++)
    {
        // cout << "PosX: " << i << "  " << m_iUnrotatedCornerX[i] << endl;
        // cout << "PosY: " << i << "  " << m_iUnrotatedCornerY[i] << endl;
        transform.setAngles(m_dTheta, m_dPhi);
        transform.computeRotation(m_dScreenRotatedCornerX, m_dScreenRotatedCornerY, m_dDstFromScreenRotated);
    }

    /* Sort points on Z axis by distance */
    sort(m_dDstFromScreenRotated ,m_dCornerSortedByDst);

    /* Rendering */
    ApplyRotation_and_Render(m_dScreenRotatedCornerX,
                             m_dScreenRotatedCornerY,
                             m_dCornerSortedByDst,
                             (double)m_iCenterPointX,
                             (double)m_iCenterPointY);

    /* Fill every pixels left empty with black */
    for(int i = 0; i < (*m_p_iImageWidth)*(*m_p_iImageHeight); i++)
    {
        if(m_p_bPixelFilled[i] == false)
        {
            m_p_ucImageData[3*i] = 0;
            m_p_ucImageData[(3*i) + 1] = 0;
            m_p_ucImageData[(3*i) + 2] = 0;
        }
    }
}


