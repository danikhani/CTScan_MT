#include "imageloader.h"
#include "ui_imageloader.h"
#include <cmath>

ImageLoader::ImageLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //give the button
    connect(ui->pushButton_8bit, SIGNAL(clicked()), this, SLOT(ReadFile()));
    //give the button 12 bit
    connect(ui->pushButton_12bit, SIGNAL(clicked()), this, SLOT(ReadFile_12bit()));
    connect(ui->pushButton_layered, SIGNAL(clicked()), this, SLOT(ReadFile_layered()));

    connect(ui->pushButton_tiefenKarte, SIGNAL(clicked()), this, SLOT(ifTiefenKarte()));
    connect(ui->pushButton_reflect, SIGNAL(clicked()), this, SLOT(update3DReflection()));

    //update slider numbers
    connect(ui->slider_start, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingStart(int)));
    connect(ui->slider_width, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingWidth(int)));
    connect(ui->slider_current_layer, SIGNAL(valueChanged(int)), this, SLOT(updatedCurrentLayer(int)));
    connect(ui->slider_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedWindowingThreshold(int)));

    //Speicher der Größe 512*512 reservieren
    m_pImageData = new short[512*512];
    m_pImageData_130 = new short[130*512*512];
    m_pTiefenkarte = new short[512*512];
}

ImageLoader::~ImageLoader()
{
    delete ui;
    delete[] m_pImageData;
    delete[] m_pImageData_130;
    delete[] m_pTiefenkarte;
}
void ImageLoader::MalePixel()
{
    QImage image(512,512, QImage::Format_RGB32);
    image.setPixel(50,100,qRgb(255,0,0));
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
    //ui->pushButton_Pixel->setText("Funktioniert");
}
void ImageLoader::ReadFile()
{
    // To open a raw file from a dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    QFile dataFile(imagePath);
    // make it read only
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
    // give an error iff the file is already in use or not available
    if (!bFileOpen)
    {
        QMessageBox::critical(this, "Bro","Cannot open it");
        return;
    }
    //now to read the data and save in a slot
    //dataFile.read(imageData,512*512);
    // How many data are read
    int iFileSize = dataFile.size();
    int iNumberBytesRead = dataFile.read(imageData,512*512);

    if (iFileSize != iNumberBytesRead)
    {
        QMessageBox::critical(this, "Bro","The Pixel numerbs are not equal");
        return;
    }
    // Close the data
    dataFile.close();

    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            int iGrauWert = imageData[imageDataPosition];
            image.setPixel(x,y,qRgb(iGrauWert,iGrauWert,iGrauWert));
            imageDataPosition++;
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
void ImageLoader::ReadFile_12bit(){
    loaded3D = false;
    // To open a raw file from a dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    QFile dataFile(imagePath);
    // make it read only
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
    // give an error iff the file is already in use or not available
    if (!bFileOpen)
    {
        QMessageBox::critical(this, "Bro","Cannot open it");
        return;
    }
    //now to read the data and save in a slot
    int iFileSize = dataFile.size();
    int iNumberBytesRead = dataFile.read((char*)m_pImageData,512*512*sizeof(short));

    if (iFileSize != iNumberBytesRead)
    {
        QMessageBox::critical(this, "Bro","The Pixel numerbs are not equal");
        return;
    }
    // Close the data
    dataFile.close();

    //read from the array and make the picture
    updateView();
}
void ImageLoader::ReadFile_layered(){
    loaded3D = true;
    // To open a raw file from a dialog
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    QFile dataFile(imagePath);
    // make it read only
    bool bFileOpen = dataFile.open(QIODevice::ReadOnly);
    // give an error iff the file is already in use or not available
    if (!bFileOpen)
    {
        QMessageBox::critical(this, "Bro","Cannot open it");
        return;
    }
    //now to read the data and save in a slot
    int iFileSize = dataFile.size();
    int iNumberBytesRead = dataFile.read((char*)m_pImageData_130,130*512*512*sizeof(short));

    if (iFileSize != iNumberBytesRead)
    {
        QMessageBox::critical(this, "Bro","The Pixel numerbs are not equal");
        return;
    }
    // Close the data
    dataFile.close();

    //read from the array and make the picture
    updateView();
}

int ImageLoader::windowing(int Hu_value, int startValue, int windowWidth){
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
void ImageLoader::updatedWindowingStart(int value)
{
    ui ->label_start->setText("Start:" + QString::number(value));
    updateView();
}
void ImageLoader::updatedWindowingWidth(int value)
{
    ui ->label_width->setText("Width:" + QString::number(value));
    updateView();
}
void ImageLoader::updateView(){
    if (loaded3D){
        update3DView();
        tiefenKarteAllowed = true;
    }
    else{
        update2DView();
        tiefenKarteAllowed = false;
    }
}

void ImageLoader::update2DView()
{
    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            imageDataPosition = x + 512*y;
            int iGrauWert = windowing(m_pImageData[imageDataPosition],ui->slider_start->value(),ui->slider_width->value());
            if(m_pImageData[imageDataPosition] <= ui->slider_threshold ->value()){
                image.setPixel(x , y, qRgb(iGrauWert,iGrauWert,iGrauWert));
            }
            else{
                image.setPixel(x , y, qRgb(255,0,0));
            }
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}

void ImageLoader::update3DView()
{
    //read from the array and make the picture
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            imageDataPosition = ui->slider_current_layer->value()*512*512 + x + 512*y;
            int iGrauWert = windowing(m_pImageData_130[imageDataPosition],ui->slider_start->value(),ui->slider_width ->value());
            if(m_pImageData_130[imageDataPosition] <= ui->slider_threshold ->value()){
                image.setPixel(x , y, qRgb(iGrauWert,iGrauWert,iGrauWert));
            }
            else{
                image.setPixel(x , y, qRgb(255,0,0));
            }
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}

void ImageLoader::updatedCurrentLayer(int value)
{
    ui ->label_current_layer->setText("Layer" + QString::number(value));
    updateView();
}
void ImageLoader::updatedWindowingThreshold(int value)
{
    ui ->label_threshold->setText("Threshold:" + QString::number(value));
    updateView();
}
//this allows us to use tiefenkarte while using a 3d file.
void ImageLoader::ifTiefenKarte(){
    if (tiefenKarteAllowed){
         updatedTiefenKarte();
    }
}
//for updating the tiefenkarte.
void ImageLoader::updatedTiefenKarte(){
    QImage image(512,512, QImage::Format_RGB32);
    int currenttiefe = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            for(int layer = 129; layer >= 0; layer--){
                int imageDataPosition =layer*512*512 + x + 512*y;;
                if(m_pImageData_130[imageDataPosition] > ui->slider_threshold ->value()){
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
    updatedTiefenKarte();
    int sy = 2;
    int sx = 2;
    QImage image(512,512, QImage::Format_RGB32);

    for(int y = 1 ; y < 511; y++){
        for(int x = 1 ; x < 511 ; x++){
            double tx =m_pTiefenkarte[x-1 + 512*y] - m_pTiefenkarte[x+1 + 512*y];
            double ty =m_pTiefenkarte[x + 512*(y-1)] - m_pTiefenkarte[x + 512*(y+1)];
            int iRefl = 255.0*(sx*sy)/std::pow(std::pow(sy*tx,2) + std::pow(sx*ty,2) + std::pow(sy*sx,2),0.5);
            image.setPixel(x , y, qRgb(iRefl,iRefl,iRefl));
        }
    }
    int a = std::pow(2,2);
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}
