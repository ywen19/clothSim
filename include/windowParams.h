/// \file windowParams.h
/// \brief encapsulates a structure for window paramters
/// \includes w,h, and cursor values (relative to screen)
/// \calculation on paramters is in MouseControls
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef WINDOWPARAMS_H_
#define WINDOWPARAMS_H_

#include <ngl/Vec3.h>


struct winParam
{
    int         win_width=1024;
    int         win_height=720;

    bool        cam_rotate=false;
    bool        cam_translate=false;
    bool        alt_flag = false; //indicate mouse control wether the cam or collision sphere
    bool        obj_flag = false;

    int         originXRot=0;
    int         originYRot=0;
    int         originXPos=0;
    int         originYPos=0;

    int         spinXFace=0;
    int         spinYFace=0;

    int         originXPosforSphere=0;
    int         originYPosforSphere=0;
    ngl::Vec3   changeVec2D;
};

constexpr float INCREMENT=0.005f; //cam change with mouse
constexpr float ZOOM=0.5f; //cam change with zoom (wheel); Larger the value, greater the change


#endif // WINDOWPARAMS_H_
