#ifndef APPLICATIONDATA_H
#define APPLICATIONDATA_H
#include <cmath>
#include <QString>
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "Eigen/Core"
#include "mylib.h"


class ApplicationData
{
public:
    ApplicationData();
    ~ApplicationData();
    bool uploadImage(QString path);
    bool calculateDepthMap(int threshold);
    const short *getImage();
    const short *getDepthMap();
    const image3D getImage3D();

private:
    //short *m_pImageData;
    short *m_pTiefenkarte;
    //image3D *imageData3D;
    image3D imageData3D;
};

#endif // APPLICATIONDATA_H
