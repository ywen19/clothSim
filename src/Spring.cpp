#include "Spring.h"

/// \brief default ctor
Spring::Spring():
                 m_force(0.f),
                 m_restlen(0.f),
                 m_ks(0.f){;}


//--------------------------------------------------------------------------
/// @brief set rest length between two given particles
/// this length differs between structural, bend and shear spring
//--------------------------------------------------------------------------
void Spring::setRestlen( float _restlen )
{
    m_restlen = _restlen;
}


//--------------------------------------------------------------------------
/// @brief set spring constant; too high value of ks makes the cloth too thight
//--------------------------------------------------------------------------
void Spring::setKs( float _ksValue )
{
    m_ks = _ksValue;
}


//--------------------------------------------------------------------------
/// @brief get spring force for adding to given particles
//--------------------------------------------------------------------------
ngl::Vec3 Spring::getSpringF()
{
    return m_force;
}


//--------------------------------------------------------------------------
/// @brief set spring force to all zero for the next calculation
//--------------------------------------------------------------------------
void Spring::setZeroSpringF()
{
    m_force.null();
}


//--------------------------------------------------------------------------
/// @brief calculate spring force using mass-spring model ( Hooke's Law )
/// distdif = currentlen-restlen
/// we use accuracy to filter out unwanted force due to floats inaccuracy
//--------------------------------------------------------------------------
void Spring::calSpringForce( Particles *point1, Particles *point2 )
{
    float accuracy = 0.1f;
    ngl::Vec3 distVec;
    distVec = point1->getParticlePos() - point2->getParticlePos();
    float currentlen = distVec.length();
    float distdiff = currentlen-m_restlen;

    if( distVec.lengthSquared() ) { distVec.normalize(); } // get the direction vector

    if( (distdiff>0.f&&distdiff<accuracy) || (distdiff<0.f&&distdiff>(-1.f)*accuracy) ) { distdiff = 0.f; }

    m_force = (-1.f) * m_ks * distVec * distdiff; // spring force on point1
}










