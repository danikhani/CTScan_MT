#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QWidget>
#include <cmath>
//to add files from desktop
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "applicationdata.h"
#include "mylib.h"
#include <QMouseEvent>
#include <QDebug>
#include <QInputDialog>
#include <Eigen/Geometry>

namespace Ui {
class ImageLoader;
}

class ImageLoader : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLoader(QWidget *parent = nullptr);
    ~ImageLoader();
    void setData(ApplicationData *pData);
signals:
    void LOG_State(QString str);
    void LOG_Instructions(QString str);

private:
    Ui::ImageLoader *ui;
    ApplicationData *m_pData;

    int updatedStart;
    int updatedWidth;
    int currentLayer;
    int updatedThreshold;
    Eigen::Vector3d localPoint_1;
    Eigen::Vector3d localPoint_2;
    Reconstruction param;
    image2D *reco_im2D;
    double scale;
    bool showSlice;
    bool imageLoaded;
    logMessages englishStrings;



private slots:
    void ReadFile();
    void updateAllViews();
    void updateXYView();
    void updateXZView();
    void updateSliceView();

    void drawXYPixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange);
    void drawNormalVectorXY(QImage &image);
    void visualizeSliceXY(image3D tmp_imageData3D,QImage &image);

    void drawXZPixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange);
    void drawNormalVectorXZ(QImage &image);
    void drawVerticalXZLine(QImage &image, Eigen::Vector3d point, int depth);
    void visualizeSliceXZ(image3D tmp_imageData3D,QImage &image);

    void drawSlicePixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange);
    void drawBoringCircle(QImage &image);
    void reconstructSlice();

    void mousePressEvent(QMouseEvent *event);
    void showTheSlice();
    void updatePointCoordinates();


    // show values of xy sliders
    void updatedXYWindowingStart(int value);
    void updatedXYWindowingWidth(int value);
    void updatedXYCurrentLayer(int value);
    void updatedXYWindowingThreshold(int value);

    // show values of xz sliders
    void updatedXZWindowingStart(int value);
    void updatedXZWindowingWidth(int value);
    void updatedXZCurrentLayer(int value);
    void updatedXZWindowingThreshold(int value);

    //show values of slice slider
    void updatedSliceWindowingStart(int value);
    void updatedSliceWindowingWidth(int value);
    void updatedSliceCurrentLayer(int value);
    void updatedSliceWindowingThreshold(int value);
    void updatedSliceRotGrade(int value);
    void updatedSliceScale(int value);
};

#endif // IMAGELOADER_H
