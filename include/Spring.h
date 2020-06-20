/// \file Spring.h
/// \brief encapsulates a spring force (between two particles) / ngl::vec3 object
/// \mass-spring damper model is used in this cloth simulation system
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef SPRING_H_
#define SPRING_H_

#include <ngl/Vec3.h>
#include "Particles.h"


class Spring
{

    public:

        Spring();
        ~Spring() = default;

        //--------------------------------------------------------------------------
        /// @brief set rest length between two given particles
        //--------------------------------------------------------------------------
        void setRestlen( float _restlen );

        //--------------------------------------------------------------------------
        /// @brief set spring constant
        //--------------------------------------------------------------------------
        void setKs( float _ksValue );

        //--------------------------------------------------------------------------
        /// @brief get spring force for adding to given particles
        //--------------------------------------------------------------------------
        ngl::Vec3 getSpringF();

        //--------------------------------------------------------------------------
        /// @brief set spring force to all zero for the next calculation
        //--------------------------------------------------------------------------
        void setZeroSpringF();

        //--------------------------------------------------------------------------
        /// @brief calculate spring force using mass-spring model
        //--------------------------------------------------------------------------
        void calSpringForce( Particles *point1, Particles *point2 );

    private:

        ngl::Vec3   m_force;    // spring force value
        float       m_restlen;  // rest length between two particles
        float       m_ks;       // Spring constant

};




#endif // SPRING_H_
