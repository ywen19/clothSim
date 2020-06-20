/// \file Triangle.h
/// \brief encapsulates a triangle ( form from 3 partricles )
/// \normals from it is a vec3
/// \the normal is used for calculating wind force: F=N*(d*Vwind), d=N/|N|
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <ngl/Vec3.h>
#include <iostream>
#include <vector>

#include "Particles.h"


class Triangle
{
    public:

        Triangle();
        virtual ~Triangle() = default;

        //--------------------------------------------------------------------------
        /// @brief return normal vector for triangular surface
        //--------------------------------------------------------------------------
        ngl::Vec3 getTriangleN();

        //--------------------------------------------------------------------------
        /// @brief set the three particles for forming the triangle surface and calculate its normal
        //--------------------------------------------------------------------------
        void setTriParticles( Particles *_p1, Particles *_p2, Particles *_p3 );

    private:

        ngl::Vec3   m_NormalV;
};



#endif // TRIANGLE_H_
