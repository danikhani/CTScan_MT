#include "mylib.h"

MyLib::MyLib()
{}

int MyLib::windowing(int HU_value, int startValue, int windowWidth, int& iGrauwert){
   int errorStatus = 0;
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
    return errorStatus;

}
