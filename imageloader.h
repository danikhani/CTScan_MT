#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//to add files from desktop
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class Widget;
}

class ImageLoader : public QWidget
{
    Q_OBJECT

public:
    explicit ImageLoader(QWidget *parent = 0);
    ~ImageLoader();

private:
    Ui::Widget *ui;
    char imageData[512*512];
    short* m_pImageData;
    short* m_pImageData_130;
    int updatedStart;
    int updatedWidth;
    int currentLayer;

private slots:
    int windowing( int HU_value, int startValue, int windowWidth);
    void MalePixel();
    void ReadFile();
    void ReadFile_12bit();
    //void SetPicture();
    void updatedWindowingStart(int value);
    void updatedWindowingWidth(int value);
    void update2DView();

    void ReadFile_layered();
    void update3DView();
    void updatedCurrentLayer(int value);
};


#endif // WIDGET_H
