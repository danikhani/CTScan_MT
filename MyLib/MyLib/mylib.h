#ifndef MYLIB_H
#define MYLIB_H

#include "MyLib_global.h"
//#include "Eigen/Core"

class MYLIB_EXPORT MyLib
{
public:
    MyLib();
    static int windowing(int HU_value, int startValue, int windowWidth);
};

#endif // MYLIB_H
