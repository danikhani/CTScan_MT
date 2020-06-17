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


class ApplicationData
{
public:
    ApplicationData();
    ~ApplicationData();
    bool uploadImage(QString path);
    bool calculateDepthMap(int threshold);
    const short* getImage();
    const short* getDepthMap();

private:

    short* m_pImageData;
    short* m_pTiefenkarte;

    //void update3DView();
    //void updatedTiefenKarte();
    //void update3DReflection();
};

#endif // APPLICATIONDATA_H
