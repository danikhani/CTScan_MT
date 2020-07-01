#include "imageloader.h"
#include "ui_imageloader.h"

ImageLoader::ImageLoader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageLoader)
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
    QString imagePath = QFileDialog::getOpenFileName(this, "open image", "./", "RAW Image Files (*.raw)");
    // give files to m-pdata
    bool status = m_pData->uploadImage(imagePath);
    //update 3Dview
    if(status){
        update3DView();
        emit LOG("Das ist ein Beispieltext");
    }
    else{
        QMessageBox::critical(this, "Cant Load","Try again please");
    }
}

void ImageLoader::update3DView()
{
    //read from the array and make the picture
    const short* pImage = m_pData->getImage();
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    int iGrauwert;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            imageDataPosition = ui->slider_current_layer->value()*512*512 + x + 512*y;
            int windowingError = MyLib::windowing(pImage[imageDataPosition],ui->slider_start->value(),ui->slider_width ->value(),iGrauwert);
            if(pImage[imageDataPosition] <= ui->slider_threshold ->value()){
                image.setPixel(x , y, qRgb(iGrauwert,iGrauwert,iGrauwert));
            }
            else{
                image.setPixel(x , y, qRgb(255,0,0));
            }
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}

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
void ImageLoader::updatedTiefenKarte(){
    //load data to threshhold
    bool status = m_pData->calculateDepthMap(ui->slider_threshold ->value());
    // check if there is a problem.
    const short* pTiefenkarte = m_pData->getDepthMap();
    if(!status){
        QMessageBox::critical(this, "Cant Load","Try again please");
    }

    QImage image(512,512, QImage::Format_RGB32);
    int currenttiefe = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            currenttiefe = pTiefenkarte[x+ 512*y];
            image.setPixel(x , y, qRgb(currenttiefe,currenttiefe,currenttiefe));

        }
    }

    //to set the current layer number string
    int currentlayer = 0;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            for(int layer = 129; layer >= 0; layer--){
                int imageDataPosition =layer*512*512 + x + 512*y;;
                if(m_pData->getImage()[imageDataPosition] > ui->slider_threshold ->value()){
                    currentlayer = 129-layer;
            }
            }
        }
    }
    ui ->label_tiefenKarte->setText("Tiefe:" + QString::number(currentlayer));

    // show the picture
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));

}

//void ImageLoader::update3DReflection(){}
//for showing the 3D-Picture

void ImageLoader::update3DReflection(){
    updatedTiefenKarte();
    const short* pTiefenkarte = m_pData->getDepthMap();
    int sy = 2;
    int sx = 2;
    QImage image(512,512, QImage::Format_RGB32);

    for(int y = 1 ; y < 511; y++){
        for(int x = 1 ; x < 511 ; x++){
            double tx =pTiefenkarte[x-1 + 512*y] - pTiefenkarte[x+1 + 512*y];
            double ty =pTiefenkarte[x + 512*(y-1)] - pTiefenkarte[x + 512*(y+1)];
            int iRefl = 255.0*(sx*sy)/std::pow(std::pow(sy*tx,2) + std::pow(sx*ty,2) + std::pow(sy*sx,2),0.5);
            image.setPixel(x , y, qRgb(iRefl,iRefl,iRefl));
        }
    }
    ui->label_image_2->setPixmap(QPixmap::fromImage(image));
}

