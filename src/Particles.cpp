#include <ngl/NGLStream.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>

#include "Particles.h"
#include <iostream>


/// \brief default ctor
Particles::Particles():
                       m_kd(.6f),
                       m_Position(.0f),
                       m_prePosition(.0f),
                       m_pForce(.0f){;}


//--------------------------------------------------------------------------
/// @brief set particle position
//--------------------------------------------------------------------------
void Particles::setParticlePos( ngl::Vec3 _position )
{
    m_Position = _position;
}


//--------------------------------------------------------------------------
/// @brief set particle previous position for damping calculation
//--------------------------------------------------------------------------
void Particles::setParticlePrePos( ngl::Vec3 _position )
{
    m_prePosition = _position;
}


//--------------------------------------------------------------------------
/// @brief set damping factor for updating position
//--------------------------------------------------------------------------
void Particles::setKd( float _Kd )
{
    m_kd = _Kd;
}


//--------------------------------------------------------------------------
/// @brief clear force to all zero for next calculation
//--------------------------------------------------------------------------
void Particles::setZeroForce()
{
    m_pForce.null();
}


//--------------------------------------------------------------------------
/// @brief get particle position
//--------------------------------------------------------------------------
ngl::Vec3 Particles::getParticlePos()
{
    return m_Position;
}


//--------------------------------------------------------------------------
/// @brief calculate total force ( spring, mass, wind, collision )
//--------------------------------------------------------------------------
void Particles::getTotalForce( ngl::Vec3 _addForce )
{
    m_pForce += _addForce;
}


//--------------------------------------------------------------------------
/// @brief update particle position with the current force and damping
/// a=F/mass; pos += acceleration as (acceleration as timestep is 1)
/// clear total force to zero for next calculation
//--------------------------------------------------------------------------
void Particles::updatePos()
{
    ngl::Vec3 temp_pos;
    temp_pos = m_Position;

    // normalize the force; no test condition for zero length for normalize in library
    if ( m_pForce.lengthSquared() ) { m_pForce.normalize(); }

    // 0.03f is a const for scaling timestep(actually speed down)
    m_pAcceleration = m_pForce*0.03f;
    // damping and update
    m_Position += ( m_Position - m_prePosition )*( 1.f - m_kd) + m_pAcceleration;
    m_Position += m_pAcceleration;
    m_prePosition = temp_pos;

    // clear force to zero for future calculation
    m_pForce.null();
}



