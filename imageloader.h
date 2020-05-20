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
    short* m_pImageData_130;
    int updatedStart;
    int updatedWidth;
    int currentLayer;
    int updatedThreshold;
    short* m_pTiefenkarte;

private slots:
    int windowing( int HU_value, int startValue, int windowWidth);
    void updatedWindowingStart(int value);
    void updatedWindowingWidth(int value);
    void ReadFile();
    void update3DView();
    void updatedCurrentLayer(int value);
    void updatedWindowingThreshold(int value);
    void updatedTiefenKarte();
    void update3DReflection();
};


#endif // WIDGET_H
