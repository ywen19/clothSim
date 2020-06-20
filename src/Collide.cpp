#include "Collide.h"


/// \brief default ctor
Collide::Collide():
                   m_distbetween(0.f),
                   m_collideDist(0.f),
                   m_colliderPos(0.f),
                   m_partPos(0.f),
                   m_collideF(0.f){;}


//--------------------------------------------------------------------------
/// @brief set collision object position from world scene
//--------------------------------------------------------------------------
void Collide::setColliderPos( ngl::Vec3 io_colliderPos )
{
    m_colliderPos = io_colliderPos;
}


//--------------------------------------------------------------------------
/// @brief return collision force so can be added to particle
//--------------------------------------------------------------------------
ngl::Vec3 Collide::getCollideF()
{
    return m_collideF;
}


//--------------------------------------------------------------------------
/// @brief clear collision force to zero for future calculation
//--------------------------------------------------------------------------
void Collide::clearCollideF()
{
    m_collideF.null();
}


//--------------------------------------------------------------------------
/// @brief get collision force between particle and ground plane
/// dist between plane and particle is the y axis of the particle
/// when inbetween distance is smaller than avoidance radius, avoidance happens
//--------------------------------------------------------------------------
void Collide::planarCollide( Particles *point1, float _collidedist )
{
    ngl::Vec3 dirVector;
    m_collideDist = _collidedist;
    m_partPos = point1->getParticlePos();

    dirVector.set(0.f, -m_partPos.m_y, 0.f);

    m_distbetween = m_partPos.m_y;

    if( m_distbetween<m_collideDist )
    {
        m_collideF = ( m_collideDist-m_distbetween )/m_collideDist * dirVector;
    }
    if( m_distbetween>=m_collideDist )
    {
        m_collideF.set( 0.f );
    }
}


//--------------------------------------------------------------------------
/// @brief get collision force between particle and sphere
/// dist inbetween is between particle and sphere center
/// avoidance radius = sphere radius ( = 1.5f ) + margin
//--------------------------------------------------------------------------
void Collide::sphereCollide( Particles *point1, float _collidedist )
{
    m_partPos = point1->getParticlePos();
    ngl::Vec3 dirVector = m_partPos - m_colliderPos;
    m_distbetween = dirVector.length();
    float collide_fac_dist = m_distbetween-1.5f;
    float margin = _collidedist-1.5f;

    if ( dirVector.lengthSquared() ) { dirVector.normalize(); }

    if( collide_fac_dist>0.f && collide_fac_dist<=margin )
    {
        m_collideF = ( _collidedist-m_distbetween ) * dirVector;
    }



}
