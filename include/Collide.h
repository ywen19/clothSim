/// \file Collide.h
/// \brief encapsulates a collision force / ngl::vec3 object
/// \two different collisions include: with gound plane and sphere
/// \collide dist is the avoidance radius
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef COLLIDE_H_
#define COLLIDE_H_

#include <ngl/Vec3.h>
#include "Particles.h"


class  Collide
{
    public:

        Collide();
        ~Collide() = default;

        //--------------------------------------------------------------------------
        /// @brief set collision object position
        //--------------------------------------------------------------------------
        void setColliderPos( ngl::Vec3 io_colliderPos );

        //--------------------------------------------------------------------------
        /// @brief get collision force for adding into particle forces
        //--------------------------------------------------------------------------
        ngl::Vec3 getCollideF();

        //--------------------------------------------------------------------------
        /// @brief clear collision force to zero for future calculation
        //--------------------------------------------------------------------------
        void clearCollideF();

        //--------------------------------------------------------------------------
        /// @brief cloth-plane collision algorithm
        //--------------------------------------------------------------------------
        void planarCollide( Particles *point1, float _collidedist );

        //--------------------------------------------------------------------------
        /// @brief cloth-sphere collision algorithm
        //--------------------------------------------------------------------------
        void sphereCollide( Particles *point1, float _collidedist );

     private:

        float       m_distbetween;  //dist between particle and collision object
        float       m_collideDist;  //collision avoidance dist
        ngl::Vec3   m_colliderPos;  //collision object pos
        ngl::Vec3   m_partPos;      //particle(being detected) pos
        ngl::Vec3   m_collideF;     //collsion force
};


#endif // COLLIDE_H_
