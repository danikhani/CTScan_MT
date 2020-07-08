#include "applicationdata.h"

ApplicationData::ApplicationData()
{
    imageData3D.pImage = new short[512*512*256];
    imageData3D.width = 512;
    imageData3D.height = 512;
    imageData3D.slices = 256;
    // size of everypixel in mm
    imageData3D.pixelSpacingXY= 1.2695;
    imageData3D.pixelSpacingZ = 2.0000;

    //m_pTiefenkarte = new short[512*512];
}

ApplicationData::~ApplicationData()
{
//    delete[] m_pImageData;
    //delete[] m_pTiefenkarte;
    delete[] imageData3D.pImage;
    //delete[] imageData3D;

}
const image3D ApplicationData::getImage3D(){
    return imageData3D;
}

// return for the arrays.
//const short* ApplicationData::getImage(){
//    return m_pImageData;
//
//}
//const short* ApplicationData::getDepthMap(){
//    return m_pTiefenkarte;
//}


bool ApplicationData::uploadImage(QString path){
    QFile dataFile(path);
    // make it read only
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
    // give an error iff the file is already in use or not available
    if (!bFileOpen)
    {
        return false;
    }
    //now to read the data and save in a slot
    int iFileSize = dataFile.size();

    int iNumberBytesRead = dataFile.read((char*)imageData3D.pImage,imageData3D.width*imageData3D.height*imageData3D.slices*sizeof(short));

    if (iFileSize != iNumberBytesRead)
    {
        return false;
    }
    // Close the data
    dataFile.close();

    return true;
}

/**
bool ApplicationData::calculateDepthMap(int threshold){
    int currenttiefe = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            for(int layer = 129; layer >= 0; layer--){
                int imageDataPosition =layer*512*512 + x + 512*y;;
                if(imageData3D.pImage[imageDataPosition] > threshold){
                    currenttiefe = 129-layer;
                    m_pTiefenkarte[x+ 512*y]=currenttiefe;
                    break;
                }
            }
        }
    }
    bool status = true;

    return status;
}
**/
