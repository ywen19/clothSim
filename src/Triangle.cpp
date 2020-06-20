#include <ngl/NGLStream.h>
#include <ngl/Vec3.h>
#include <iostream>
#include <vector>

#include "Triangle.h"

/// \brief default ctor
Triangle::Triangle():
                     m_NormalV(0.f){;}


//--------------------------------------------------------------------------
/// @brief return normal vector for triangular surface
//--------------------------------------------------------------------------
ngl::Vec3 Triangle::getTriangleN()
{
    return m_NormalV;
}


//--------------------------------------------------------------------------
/// @brief set the three particles for forming the triangle surface and calculate its normal
/// normal of a surface is the cross product of two vectors on it
//--------------------------------------------------------------------------
void Triangle::setTriParticles( Particles *_p1, Particles *_p2, Particles *_p3 )
{
    ngl::Vec3 p1top2( 0.f );
    ngl::Vec3 p1top3( 0.f );

    // two vectors lie on the triangle surface
    p1top2 = _p1->getParticlePos() - _p2->getParticlePos();
    p1top3 = _p1->getParticlePos() - _p3->getParticlePos();
    m_NormalV = p1top2.cross( p1top3 );
}


