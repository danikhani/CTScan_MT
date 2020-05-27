#include "applicationdata.h"

ApplicationData::ApplicationData()
{
    m_pImageData = new short[130*512*512];
    m_pTiefenkarte = new short[512*512];
}
// return for the arrays.
const short* ApplicationData::getImage(){
    return m_pImageData;

}
const short* ApplicationData:: getDepthMap(){
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
    int currenttiefe = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            for(int layer = 129; layer >= 0; layer--){
                int imageDataPosition =layer*512*512 + x + 512*y;;
                if(m_pImageData[imageDataPosition] > threshold){
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
