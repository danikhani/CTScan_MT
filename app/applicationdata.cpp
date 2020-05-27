#include "applicationdata.h"

ApplicationData::ApplicationData()
{
    m_pImageData = new short[130*512*512];
    m_pTiefenkarte = new short[512*512];
}


bool ApplicationData::uploadImage(QString path){
    bool status = true;

    return status;
}

bool ApplicationData::calculateDepthMap(int threshold){
    bool status = true;

    return status;
}
const short* ApplicationData::getImage(){
    return m_pImageData;

}
const short* ApplicationData:: getDepthMap(){
    return m_pTiefenkarte;
}
