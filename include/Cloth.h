/// \file Cloth.h
/// \brief encapsulates a cloth grid / Vector object
/// \Particles are stored in an one-dimension vector
/// \draw() draws out particles one by one after update
/// \update() contains all calculations on force (gravity, spring, wind, collision)
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef CLOTH_H_
#define CLOTH_H_

#include <ngl/Vec3.h>
#include <vector>
#include <memory>
#include <ngl/AbstractVAO.h>

#include "Particles.h"
#include "Spring.h"
#include "Triangle.h"
#include "Collide.h"


class Cloth
{
  public:

    Cloth();
    ~Cloth() = default;

    //--------------------------------------------------------------------------
    /// @brief fetch values from UI in real time
    //--------------------------------------------------------------------------
    void setWindF( float _windF );
    void setHangTL( bool _hangTL );
    void setHangTR( bool _hangTR );
    void setHangBL( bool _hangBL );
    void setHangBR( bool _hangBR );
    void setGroundCollide( bool _enable );
    void setSphereCollide( bool _enable );

    void setBendResist( float _bendResist );
    void setShearResist( float _shearResist );
    void setStiffFactor( float _stiffFactor );

    void setColliderPos( ngl::Vec3 io_colliderpos );
    void setDampFactor( float _dampFactor );
    void setSpringConstV( float _ks );


    //--------------------------------------------------------------------------
    /// @brief reset simulation, camera and UI values; Cloth lies on XZ Plane
    //--------------------------------------------------------------------------
    void resetClothOnXZPlane();

    //--------------------------------------------------------------------------
    /// @brief draw primitives and cloth particles onto the screen
    //--------------------------------------------------------------------------
    void draw() const;

    //--------------------------------------------------------------------------
    /// @brief calculate particle forces and update particle position
    //--------------------------------------------------------------------------
    void update();

    //--------------------------------------------------------------------------
    /// @brief calculate spring force between two given particles
    //--------------------------------------------------------------------------
    void calSpringBetweenParticles( int _part1index, int _part2index, float _forceScale );

    //--------------------------------------------------------------------------
    /// @brief calculate wind force
    //--------------------------------------------------------------------------
    void calWindBetweenParticles( int _part1index, int _part2index, int _part3index );


    private:

        bool        m_hangTL;           // top left fixed
        bool        m_hangTR;           // top right fixed
        bool        m_hangBL;           // bottom left fixed
        bool        m_hangBR;           // bottom right fixed
        bool        m_groundCollide;    // on or off ground collide
        bool        m_sphereCollide;    // on or off sphere collide

        float       m_bendResistance;   // scaling factor for bending spring
        float       m_shearResistance;  // scaling factor for shear spring
        float       m_stiffFactor;      // how stiff the cloth is
        float       m_dampFactor;       // damping factor to update new particle pos
        float       m_springConst;      // spring constant; pass to spring

        int         c_resParticles;     // resolution of particles
        int         c_numParticles;     // number of particles;
        float       m_gapGrid;          // gap between direct neighbor at axis-aligned direction
        float       m_gapDiagonal;      // gap between direct neighbor at diagonal direction

        float       c_gravityconst;     // gravity constant
        ngl::Vec3   m_windDir;          // wind direction and velocity
        ngl::Vec3   m_windF;            // wind force(after calculation)

        Spring      m_pSpring;          // string instance for calculate spring force
        Triangle    m_pTriangle;        // triangle instance for triangle normal (for windF)
        Collide     m_pCollide;         // collide instance for calculate collision force
        ngl::Vec3   m_colliderpos;      // collision object position

        std::vector<Particles>              m_particles;          // vector to store all cloth particles
        std::unique_ptr<ngl::AbstractVAO>   m_vao;                // for draw()
};


#endif // CLOTH_H_
