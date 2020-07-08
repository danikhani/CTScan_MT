#include "mylib.h"

MyLib::MyLib()
{}

int MyLib::windowing(int HU_value, int startValue, int windowWidth, int& iGrauwert){
   //0 if ok.-1 if HU_value is out of range. -2 if windowing parameters are out of range
   if(HU_value > 3072 || HU_value < -1024){
       return -1;
   }
   else if(windowWidth < 1 || windowWidth > 4096){
       return -2;
   }
   else{
        //fensterung berechnen
        if(HU_value<startValue){
            iGrauwert = 0;
        }
        else if(HU_value> (startValue+windowWidth)){
            iGrauwert = 255;
        }
        else{
            iGrauwert =  255.0/windowWidth * (HU_value-startValue);
        }
        return 0;
    }
}

 //TODO: scalierung muss berücksichtigig werden,
int MyLib::getSlice(const image3D& image, const Reconstruction& param, image2D& im2D){
    Eigen::Vector3d normXdir = param.xdir.normalized();
    Eigen::Vector3d normYdir = param.ydir.normalized();
    Eigen::Vector3d tempVector;
    int iHuVal;

    for (double i = 0.0; i < im2D.width; i++) {
        for (double j = 0.0; j < im2D.height; j++){
            //normalize
            tempVector = param.pos + i*normXdir + j*normYdir;
            //TODO: scalierung muss berücksichtigig werden,
            int x = round(tempVector.x());
            int y = round(tempVector.y());
            int z = round(tempVector.z());

            int iIndex = x+ y*image.height  + z*image.height*image.slices;
            if ((x>=0 && x<image.width) && (y>=0 && y<image.height) && (z>=0 && z<image.slices)){
                 iHuVal = image.pImage[iIndex];
            }
            else
            {
                iHuVal = 0;
            }
            int iIdx = (int)i+ (int)j*im2D.height;
            im2D.pImage[iIdx] = iHuVal;
        }
    }
    return 1;
}


