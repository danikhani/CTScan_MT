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

    //xy picture sliders and labels
    connect(ui->slider_xy_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingStart(int)));
    connect(ui->slider_xy_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingWidth(int)));
    connect(ui->slider_xy_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedXYCurrentLayer(int)));
    connect(ui->slider_xy_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXYWindowingThreshold(int)));
    //slider for the point z value
    connect(ui->slider_xz_start, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingStart(int)));
    connect(ui->slider_xz_width, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingWidth(int)));
    connect(ui->slider_xz_currentLayer, SIGNAL(valueChanged(int)), this, SLOT(updatedXZCurrentLayer(int)));
    connect(ui->slider_xz_threshold, SIGNAL(valueChanged(int)), this, SLOT(updatedXZWindowingThreshold(int)));
    currentPoint = 2;

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

    if(status == 1){
        updateAllViews();
        emit LOG("Image Loaded");
    }
    else{
        QMessageBox::critical(this, "Cant Load","Try again please");
    }
}
//updateView(int width, int height, int startValue, int windowWidth, image3D tmp_im3D, QImage image);
void ImageLoader::updateAllViews(){
    updateXZView();
    updateXYView();
   }
void ImageLoader::updateXYView(){
    //loading data from application data
    const image3D tmp_imageData3D = m_pData->getImage3D();
    //ui elements
    QImage image(512,512, QImage::Format_RGB32);
    int startSlider = ui->slider_xy_start->value();
    int widthSlider = ui->slider_xy_width ->value();
    int currentLayerSlider = ui->slider_xy_currentLayer->value();
    int thresholdSlider = ui->slider_xy_threshold ->value();
    // values for the algorithem
    int imageDataPosition = 0;
    int iGrauwert;
    int hu_value;
    int number_HU_OutOfRange = 0;
    int number_windowing_OutOfRange = 0;
    for(int y = 0 ; y < tmp_imageData3D.width; y++){
        for(int x = 0 ; x < tmp_imageData3D.height ; x++){
             imageDataPosition = x * tmp_imageData3D.width + y + tmp_imageData3D.width*tmp_imageData3D.height*currentLayerSlider ;
             hu_value = tmp_imageData3D.pImage[imageDataPosition];
             int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
             //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
             if(windowingError == -1){
                 number_HU_OutOfRange++;
             }
             if(windowingError == -2){
                 number_windowing_OutOfRange++;
             }
             if(hu_value <= thresholdSlider){
                image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
             }else{
                 image.setPixel(y, x, qRgb(255,0,0));
             }
        }
    }
    ui->label_xy_image->setPixmap(QPixmap::fromImage(image));

    QString outOfRangeNumber;
    outOfRangeNumber.setNum(number_HU_OutOfRange);
    QString messagetext = "times was/were HU_Value out of Range";
    outOfRangeNumber += messagetext;
    //emit LOG(outOfRangeNumber);
}

void ImageLoader::updateXZView(){
    //loading data from application data
    const image3D tmp_imageData3D = m_pData->getImage3D();
    //ui elements
    QImage image(512,512, QImage::Format_RGB32);
    int startSlider = ui->slider_xz_start->value();
    int widthSlider = ui->slider_xz_width ->value();
    int currentLayerSlider = ui->slider_xz_currentLayer->value();
    int thresholdSlider = ui->slider_xz_threshold ->value();
    // values for the algorithem
    int imageDataPosition = 0;
    int iGrauwert;
    int hu_value;
    int number_HU_OutOfRange = 0;
    int number_windowing_OutOfRange = 0;
    for(int y = 0 ; y < tmp_imageData3D.width; y++){
        for(int x = 0 ; x < tmp_imageData3D.slices ; x++){
             imageDataPosition = currentLayerSlider * tmp_imageData3D.width + y + tmp_imageData3D.width*tmp_imageData3D.height*x ;
             hu_value = tmp_imageData3D.pImage[imageDataPosition];
             int windowingError = MyLib::windowing(hu_value,startSlider,widthSlider,iGrauwert);
             //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
             if(windowingError == -1){
                 number_HU_OutOfRange++;
             }
             if(windowingError == -2){
                 number_windowing_OutOfRange++;
             }
             if(hu_value <= thresholdSlider){
                image.setPixel(y, x,qRgb(iGrauwert, iGrauwert, iGrauwert));
             }else{
                 image.setPixel(y, x, qRgb(255,0,0));
             }
        }
    }
    ui->label_xz_image->setPixmap(QPixmap::fromImage(image));

    QString outOfRangeNumber;
    outOfRangeNumber.setNum(number_HU_OutOfRange);
    QString messagetext = "times was/were HU_Value out of Range";
    outOfRangeNumber += messagetext;
    //emit LOG(outOfRangeNumber);

}
//updateView(int width, int height, int startValue, int windowWidth, image3D tmp_im3D, QImage image);

void ImageLoader::update3DView()
{
    //read from the array and make the picture
    const image3D tmp_imageData3D = m_pData->getImage3D();
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    int iGrauwert;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            imageDataPosition = ui->slider_xy_currentLayer->value()*512*512 + x + 512*y;
            //tryvoxel[x][y][ui->slider_current_layer->value()] = tmp_imageData3D.pImage[imageDataPosition];

            int windowingError = MyLib::windowing(tmp_imageData3D.pImage[imageDataPosition],ui->slider_xy_start->value(),ui->slider_xy_width ->value(),iGrauwert);
            if(tmp_imageData3D.pImage[imageDataPosition] <= ui->slider_xy_threshold ->value()){
                image.setPixel(x , y,qRgb(iGrauwert, iGrauwert, iGrauwert));
            }
            else{
                image.setPixel(x , y, qRgb(255,0,0));
            }
        }
    }

    //qDebug( "C Style Debug Message" );
    ui->label_xy_image->setPixmap(QPixmap::fromImage(image));
}

void ImageLoader::updatedXYWindowingStart(int value)
{
    ui ->label_xy_start->setText("Start:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXYWindowingWidth(int value)
{
    ui ->label_xy_width->setText("Width:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXYCurrentLayer(int value)
{
    ui ->label_xy_currentLayer->setText("Layer" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXYWindowingThreshold(int value)
{
    ui ->label_xy_threshold->setText("Threshold:" + QString::number(value));
    updateAllViews();
}
//update xz slider values
void ImageLoader::updatedXZWindowingStart(int value)
{
    ui ->label_xz_start->setText("Start:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXZWindowingWidth(int value)
{
    ui ->label_xz_width->setText("Width:" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXZCurrentLayer(int value)
{
    ui ->label_xz_currentLayer->setText("Layer" + QString::number(value));
    updateAllViews();
}
void ImageLoader::updatedXZWindowingThreshold(int value)
{
    ui ->label_xz_threshold->setText("Threshold:" + QString::number(value));
    updateAllViews();
}


void ImageLoader::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    QPoint globalPos;
    globalPos = event->pos();
    QPoint localPos;
    localPos = ui->label_xy_image->mapFromParent(globalPos);
    if (ui->label_xy_image->rect().contains(localPos)){
        // check for risks. for choosing a point
        if(currentPoint ==2){
            localPoint_XY_1.x() = localPos.x();
            localPoint_XY_1.y() = localPos.y();
            //localPoint1.z = ui->slider_point1_z->value();
            currentPoint = 1;
            drawLine();
        }
        else if(currentPoint == 1){
            localPoint_XY_2.x() = localPos.x();
            localPoint_XY_2.y() = localPos.y();
            //localPoint2.z = ui->slider_point2_z->value();;
            currentPoint = 2;
            drawLine();
        }
    }
}

void ImageLoader::drawLine()
{

    /**
    float tanLine = (firstPointXY.y - secPointXY.y)/(firstPointXY.x - secPointXY.x);
    for (float i = 0; i < 1000; i++) {
        float dx = firstPointXY.x + (secPointXY.x - firstPointXY.x)/1000.0*i; // interpolate betwenn the two points
        float dy = (dx-firstPointXY.x)* tanLine + firstPointXY.y;
        int x = (int)dx;
        int y = (int)dy;
        imageXY.setPixel(x,y,qRgb(255, 255, 0));
    }
    **/

    //read from the array and make the picture
    const image3D tmp_imageData3D = m_pData->getImage3D();
    QImage image(512,512, QImage::Format_RGB32);
    int imageDataPosition = 0;
    int iGrauwert;
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            imageDataPosition = ui->slider_xy_currentLayer->value()*512*512 + x + 512*y;
            int windowingError = MyLib::windowing(tmp_imageData3D.pImage[imageDataPosition],ui->slider_xy_start->value(),ui->slider_xy_width ->value(),iGrauwert);
            if(tmp_imageData3D.pImage[imageDataPosition] <= ui->slider_xy_threshold ->value()){
                image.setPixel(x , y, qRgb(iGrauwert,iGrauwert,iGrauwert));
            }
            else{
                image.setPixel(x , y, qRgb(255,0,0));
            }
            if(y == ((localPoint_XY_2.y() - localPoint_XY_1.y())/(localPoint_XY_2.x() - localPoint_XY_1.x())*(x-localPoint_XY_1.x())+localPoint_XY_1.y())){
                image.setPixel(x , y, qRgb(255,0,0));
            }
        }
    }
    ui->label_xy_image->setPixmap(QPixmap::fromImage(image));
}
/**
drawline function:
get localPoint1.x
**/
void ImageLoader::updatedPoint1Z(int value)
{
    ui ->label_xy_currentLayer->setText("Layer" + QString::number(value));
    update3DView();
}
void ImageLoader::updatedPoint2Z(int value)
{
    ui ->label_xy_currentLayer->setText("Layer" + QString::number(value));
    update3DView();
}


/**
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
    const image3D tmp_imageData3D = m_pData->getImage3D();
    for(int y = 0 ; y < 512; y++){
        for(int x = 0 ; x < 512 ; x++){
            for(int layer = 129; layer >= 0; layer--){
                int imageDataPosition =layer*512*512 + x + 512*y;;
                if(tmp_imageData3D.pImage[imageDataPosition] > ui->slider_threshold ->value()){
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
**/


/**

void ImageLoader::on_Button_FindMarker_clicked()
{
    EIDM_CTScan scan;
    scan.data = m_pImage;
    scan.width = 512;
    scan.height = 512;
    scan.layers = 130;
    scan.size = 512 * 512 * 130 * sizeof(short);
    scan.vox_scale_xy = 0.1;
    scan.vox_scale_z = 0.1;

    uint8_t* check_arr;
    check_arr = new uint8_t[512 * 512 * 130];
    EIDM_ProgStatus prog;

    EIDM_ProgCallback Callback;

    EIDM_UserData UserData;

    EIDM_RegionList list;

    int z = EIDM_find_markers(&scan, nullptr, nullptr, nullptr, nullptr);
}


void ImageLoader::Region_Growing(Point localPoint){

    EIDM_CTScan scan;
    scan.data = m_pImage;
    scan.width = 512;
    scan.height = 512;
    scan.layers = 130;
    scan.size = 512 * 512 * 130 * sizeof(short);
    scan.vox_scale_xy = 0.1;
    scan.vox_scale_z = 0.1;

    EIDM_Vox3 seed;
    seed.x = localPoint.x;
    seed.y = localPoint.y;
    seed.z = localPoint.z;
    uint8_t* check_arr;
    check_arr = new uint8_t[512 * 512 * 130];
    EIDM_ProgStatus prog;

    EIDM_ProgCallback Callback;

    EIDM_UserData UserData;

    EIDM_RegionList list;

    int t = EIDM_region_grow(&scan, &seed, Untere_Grenze, Obere_Grenze, check_arr, nullptr, nullptr);

    //int r = EIDM_region_list_from_mat(&scan, check_arr, *list);

    QImage im(512, 512, QImage::Format_RGB32);
    int test = check_arr[200+200*512+512*512*60] ;
    for (int i = 0; i < 512; i++)	{
        for (int j = 0; j < 512; j = j + 1)		{
                if (check_arr[j * 512 + i + 512 * 512 * layerNr] == 1){
                    im.setPixel(i, j, qRgb(0, 255, 0));
                }
        }
    }
    ui->labelImage->setPixmap(QPixmap::fromImage(im));

}

**/

