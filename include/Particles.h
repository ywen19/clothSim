/// \file Particles.h
/// \brief encapsulates a particle
/// \Store position, previous_position, force etc. for calculation and update
/// \Also include methods to set particle attributes
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef PARTICLES_H_
#define PARTICLES_H_

#include <ngl/Vec3.h>
#include <memory>
#include <ngl/AbstractVAO.h>


class Particles
{
    public:

        Particles();

        ~Particles() = default;

        //--------------------------------------------------------------------------
        /// @brief set particle position
        //--------------------------------------------------------------------------
        void setParticlePos( ngl::Vec3 _position );

        //--------------------------------------------------------------------------
        /// @brief set particle previous position for damping calculation
        //--------------------------------------------------------------------------
        void setParticlePrePos( ngl::Vec3 _position );

        //--------------------------------------------------------------------------
        /// @brief set damping factor for updating position
        //--------------------------------------------------------------------------
        void setKd( float _Kd );

        //--------------------------------------------------------------------------
        /// @brief clear force to all zero for next calculation
        //--------------------------------------------------------------------------
        void setZeroForce();

        //--------------------------------------------------------------------------
        /// @brief get particle position
        //--------------------------------------------------------------------------
        ngl::Vec3 getParticlePos();

        //--------------------------------------------------------------------------
        /// @brief calculate total force ( spring, mass, wind, collision )
        //--------------------------------------------------------------------------
        void getTotalForce( ngl::Vec3 _addForce );

        //--------------------------------------------------------------------------
        /// @brief update particle position with the current force and damping
        //--------------------------------------------------------------------------
        void updatePos();

    private:

        float       m_kd;             // damping factor
        ngl::Vec3   m_Position;       // current position
        ngl::Vec3   m_prePosition;    // previous position
        ngl::Vec3   m_pAcceleration;  // acceleration = F/mass
        ngl::Vec3   m_pForce;         // force applied on particle
};

#endif // PARTICLES_H_
