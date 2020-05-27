#include "imageloader.h"
#include "ui_imageloader.h"
#include <cmath>

ImageLoader::ImageLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //give the button
    connect(ui->pushButton_layered, SIGNAL(clicked()), this, SLOT(ReadFile()));
    connect(ui->pushButton_tiefenKarte, SIGNAL(clicked()), this, SLOT(updatedTiefenKarte()));
    connect(ui->pushButton_reflect, SIGNAL(clicked()), this, SLOT(update3DReflection()));

    //update slider numbers
    connect(ui->slider_start, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingStart(int)));
    connect(ui->slider_width, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingWidth(int)));
    connect(ui->slider_current_layer, SIGNAL(valueChanged(int)), this, SLOT(updatedCurrentLayer(int)));
    connect(ui->slider_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingThreshold(int)));


}

ImageLoader::~ImageLoader()
{
    delete ui;
}
void ImageLoader::setData(ApplicationData *pData){
    this->m_pData = pData;
}

void ImageLoader::ReadFile(){
    // To open a raw file from a dialog
    //QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    QString imagePath = "C:/Users/dania/OneDrive/Dokumente/Dokuments/RWTH/Master/SS20/Softwareentwicklung in der Medizintechnik/Bilder/Kopf_CT_130.raw";
    bool status = m_pData->uploadImage(imagePath);
    if (!status){
        QMessageBox::critical(this, "Bro","Cannot open it");
    }
    //read from the array and make the picture
    update3DView();
}
void ImageLoader::update3DView()
{
    const short* pImage = m_pData->getImage();
    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            imageDataPosition = ui->slider_current_layer->value()*512*512 + x + 512*y;
            int iGrauWert = MyLib::windowing(pImage[imageDataPosition],ui->slider_start->value(),ui->slider_width ->value());
            if(pImage[imageDataPosition] <= ui->slider_threshold ->value()){
                image.setPixel(x , y, qRgb(iGrauWert,iGrauWert,iGrauWert));
            }
            else{
                image.setPixel(x , y, qRgb(255,0,0));
            }
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
/*int ImageLoader::windowing(int Hu_value, int startValue, int windowWidth){
    int iGrauwert;
    //fensterung berechnen
    if(Hu_value<startValue){
        iGrauwert = 0;
    }
    else if(Hu_value> (startValue+windowWidth)){
        iGrauwert = 255;
    }
    else{
        iGrauwert =  255.0/windowWidth * (Hu_value-startValue);
    }
    return iGrauwert;
}
*/
void ImageLoader::updatedWindowingStart(int value)
{
    ui ->label_start->setText("Start:" + QString::number(value));
    update3DView();
}
void ImageLoader::updatedWindowingWidth(int value)
{
    ui ->label_width->setText("Width:" + QString::number(value));
    update3DView();
}
void ImageLoader::updatedCurrentLayer(int value)
{
    ui ->label_current_layer->setText("Layer" + QString::number(value));
    update3DView();
}
void ImageLoader::updatedWindowingThreshold(int value)
{
    ui ->label_threshold->setText("Threshold:" + QString::number(value));
    update3DView();
}

//for updating the tiefenkarte.
/*
void ImageLoader::updatedTiefenKarte(){
    QImage image(512,512, QImage::Format_RGB32);
    const short* pImage = m_pData->getImage();
    int currenttiefe = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            for(int layer = 129; layer >= 0; layer--){
                int imageDataPosition =layer*512*512 + x + 512*y;;
                if(pImage[imageDataPosition] > ui->slider_threshold ->value()){
                    currenttiefe = 129-layer;
                    m_pTiefenkarte[x+ 512*y]=currenttiefe;
                    break;
                }
            }

            image.setPixel(x , y, qRgb(currenttiefe,currenttiefe,currenttiefe));

        }
    }
    ui ->label_tiefenKarte->setText("Tiefe:" + QString::number(currenttiefe));
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));

}
//for showing the 3D-Picture
void ImageLoader::update3DReflection(){
    const short* pImage = m_pData->getImage();
    updatedTiefenKarte();
    int sy = 2;
    int sx = 2;
    QImage image(512,512, QImage::Format_RGB32);

    for(int y = 1 ; y < 511; y++){
        for(int x = 1 ; x < 511 ; x++){
            double tx =pImage[x-1 + 512*y] - m_pTiefenkarte[x+1 + 512*y];
            double ty =pImage[x + 512*(y-1)] - m_pTiefenkarte[x + 512*(y+1)];
            int iRefl = 255.0*(sx*sy)/std::pow(std::pow(sy*tx,2) + std::pow(sx*ty,2) + std::pow(sy*sx,2),0.5);
            image.setPixel(x , y, qRgb(iRefl,iRefl,iRefl));
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
*/
void ImageLoader::updatedTiefenKarte(){}
void ImageLoader::update3DReflection(){}
