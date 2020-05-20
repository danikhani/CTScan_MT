#include "applicationdata.h"

ApplicationData::ApplicationData()
{
    m_pImageData = new short[130*512*512];
    m_pTiefenkarte = new short[512*512];
}

const short* ApplicationData::getImage(){
    return m_pImageData;
}

const short* ApplicationData::getDepthMap(){
    return m_pTiefenkarte;
}

bool uploadImage(QString path){
    return true;
}

bool calculateDepthMap(int threshold){
    return true;
}
