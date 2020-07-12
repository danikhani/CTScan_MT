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
}

ApplicationData::~ApplicationData()
{

    delete[] imageData3D.pImage;

}
const image3D ApplicationData::getImage3D(){
    return imageData3D;
}

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
