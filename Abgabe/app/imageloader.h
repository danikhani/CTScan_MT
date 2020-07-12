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
    image2D *reco_im2D;
    Reconstruction param;
    double scale;
    bool imageLoaded;
    bool showSlice;


private slots:
    /**
     * @brief ReadFile
     * This brings a GUI page to load the RAW picture
     */
    void ReadFile();
    /**
     * @brief updateAllViews
     * @brief updateXYView
     * @brief updateXZView
     * @brief updateSliceView
     * These Methods update the GUI-Label with the desired pixels.
     */
    void updateAllViews();
    void updateXYView();
    void updateXZView();
    void updateSliceView();


    //Methods responsible for the XY-Gui Label:
    /**
     * @brief drawXYPixels
     * It draws the pixels read from the raw file.
     * @param tmp_imageData3D is the saved array with the raw data files
     * @param image reconstructed gui-label which is responsible to draw the pixels
     * @param numberHUOutOfRange reconstructed number of HU out of range error
     * @param numberWindowingOutOfRange reconstructed number of windowing out of range error
     */
    void drawXYPixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange);
    /**
     * @brief drawNormalVectorXY
     * It draws the drill trajektorie
     * @param image reconstructed gui-label which is responsible to draw the pixels
     */
    void drawNormalVectorXY(QImage &image);
    /**
     * @brief visualizeSliceXY
     * It draws the plane orthogonal to the drill trajektorie
     * @param tmp_imageData3D is the saved array with the raw data files
     * @param image reconstructed gui-label which is responsible to draw the pixels
     */
    void visualizeSliceXY(image3D tmp_imageData3D,QImage &image);


    //Methods responsible for the XZ-Gui Label:
    /**
     * @brief drawXZPixels
     * It draws the pixels read from the raw file.
     * @param tmp_imageData3D is the saved array with the raw data files
     * @param image is the gui-label which is responsible to draw the pixels
     * @param numberHUOutOfRange reconstructed number of HU out of range error
     * @param numberWindowingOutOfRange reconstructed number of windowing out of range error
     */
    void drawXZPixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange);
    /**
     * @brief drawNormalVectorXZ
     * It draws the drill trajektorie
     * @param image reconstructed gui-label which is responsible to draw the pixels
     */
    void drawNormalVectorXZ(QImage &image);
    /**
     * @brief drawVerticalXZLine
     * It draws the vertical lines in XZ label
     * @param image reconstructed gui-label which is responsible to draw the pixels
     * @param point is the vector with the x-position of to be drawed lines
     * @param depth is the horizontal length of the XZ label.
     */
    void drawVerticalXZLine(QImage &image, Eigen::Vector3d point, int depth);
    /**
     * @brief visualizeSliceXZ
     * It draws the plane orthogonal to the drill trajektorie
     * @param tmp_imageData3D is the saved array with the raw data files
     * @param image reconstructed gui-label which is responsible to draw the pixels
     */
    void visualizeSliceXZ(image3D tmp_imageData3D,QImage &image);

    /**
     * @brief drawSlicePixels
     * It draws the pixels read from the raw file.
     * @param tmp_imageData3D is the saved array with the raw data files
     * @param image reconstructed gui-label which is responsible to draw the pixels
     * @param numberHUOutOfRange reconstructed number of HU out of range error
     * @param numberWindowingOutOfRange reconstructed number of windowing out of range error
     */
    void drawSlicePixels(image3D tmp_imageData3D, QImage &image,int &numberHUOutOfRange, int &numberWindowingOutOfRange);
    /**
     * @brief drawBoringCircle
     * It draws the drilling circle on the slice label
     * @param image reconstructed gui-label which is responsible to draw the pixels
     */
    void drawBoringCircle(QImage &image);
    /**
     * @brief makeSliceData
     * This calculates the data which will be shown on the slice label.
     */
    void makeSliceData();

    //Methods responsible for the XZ-Gui Label:
    /**
     * @brief mousePressEvent
     * is the overriten method from the QT.
     * It gives the coordinates of the clicked pixel on every label
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);
    /**
     * @brief showTheSlice
     * this called when the user clicks on the button 'show slices'.
     * it trues bool showSlice. Which later allows to show the orthogonal slices on xy and xz label.
     */
    void showTheSlice();
    /**
     * @brief updatePointCoordinates
     * this called when the user clicks on the button 'show points'.
     * it allows the user to give manually the desired coordinates of the points.
     */
    void updatePointCoordinates();

    //Methods responsible for updating the Gui text elements from slider:
    // show values of xy sliders
    /**
     * @brief updatedXYWindowingStart
     * @brief updatedXYWindowingWidth
     * @brief updatedXYCurrentLayer
     * @brief updatedXYWindowingThreshold
     * These methods update the labels related to a slider when user change the value of the slider.
     * they also update the related GUI-Label for showing the new pixels
     * @param value is the value read from the slider.
     */
    void updatedXYWindowingStart(int value);
    void updatedXYWindowingWidth(int value);
    void updatedXYCurrentLayer(int value);
    void updatedXYWindowingThreshold(int value);

    // show values of xz sliders
    /**
     * @brief updatedXZWindowingStart
     * @brief updatedXZWindowingWidth
     * @brief updatedXZCurrentLayer
     * @brief updatedXZWindowingThreshold
     * These methods update the labels related to a slider when user change the value of the slider.
     * they also update the related GUI-Label for showing the new pixels
     * @param value is the value read from the slider.
     */
    void updatedXZWindowingStart(int value);
    void updatedXZWindowingWidth(int value);
    void updatedXZCurrentLayer(int value);
    void updatedXZWindowingThreshold(int value);

    //show values of slice slider
    /**
     * @brief updatedSliceWindowingStart
     * @brief updatedSliceWindowingWidth
     * @brief updatedSliceCurrentLayer
     * @brief updatedSliceWindowingThreshold
     * @brief updatedSliceRotGrade
     * @brief updatedSliceScale
     * These methods update the labels related to a slider when user change the value of the slider.
     * they also update the related GUI-Label for showing the new pixels
     * @param value is the value read from the slider.
     */
    void updatedSliceWindowingStart(int value);
    void updatedSliceWindowingWidth(int value);
    void updatedSliceCurrentLayer(int value);
    void updatedSliceWindowingThreshold(int value);
    void updatedSliceRotGrade(int value);
    void updatedSliceScale(int value);
};

#endif // IMAGELOADER_H
