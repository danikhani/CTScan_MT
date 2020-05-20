#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <QString>


class ApplicationData
{
public:
    ApplicationData();
    bool uploadImage(QString path);
    bool calculateDepthMap(int threshold);

    const short* getImage();
    const short* getDepthMap();

private:
    short* m_pTiefenkarte;
    short* m_pImageData;
};

#endif // APPLICATIONDATA_H
