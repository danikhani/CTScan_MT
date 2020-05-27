#include "applicationdata.h"

ApplicationData::ApplicationData()
{
    m_pImageData = new short[130*512*512];
    m_pTiefenkarte = new short[512*512];
}

const short* ApplicationData::getImage(){
    return m_pImageData;
}
void ApplicationData::setImage(short* x){
   m_pImageData  = x ;
}

const short* ApplicationData::getDepthMap(){
    return m_pTiefenkarte;
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
    int iNumberBytesRead = dataFile.read((char*)m_pImageData,130*512*512*sizeof(short));
    if (iFileSize != iNumberBytesRead)
    {
        return false;
    }
    // Close the data
    dataFile.close();
    return true;
}

bool ApplicationData::calculateDepthMap(int threshold){
    return true;
}
