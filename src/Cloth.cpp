#include <ngl/NGLInit.h>
#include <iostream>
#include <math.h>
#include <ngl/NGLStream.h>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>

#include "Cloth.h"


/// \brief default ctor
Cloth::Cloth() :
                m_hangTL(true),
                m_hangTR(true),
                m_hangBL(true),
                m_hangBR(true),
                m_groundCollide(true),
                m_sphereCollide(false),
                m_bendResistance(2.f),
                m_shearResistance(1.f),
                m_stiffFactor(1.f),
                m_dampFactor(.5f),
                m_springConst(.7f),
                c_resParticles(30),
                c_gravityconst(-0.098f/2.f),
                m_vao{ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS)}
{
    resetClothOnXZPlane();
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to fix top left corner of cloth
//--------------------------------------------------------------------------
void Cloth::setHangTL( bool _hangTL )
{
    m_hangTL = _hangTL;
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to fix top right corner of cloth
//--------------------------------------------------------------------------
void Cloth::setHangTR( bool _hangTR )
{
    m_hangTR = _hangTR;
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to fix bottom left corner of cloth
//--------------------------------------------------------------------------
void Cloth::setHangBL( bool _hangBL )
{
    m_hangBL = _hangBL;
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to fix bottom right corner of cloth
//--------------------------------------------------------------------------
void Cloth::setHangBR( bool _hangBR )
{
    m_hangBR = _hangBR;
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to collide with ground plane
//--------------------------------------------------------------------------
void Cloth::setGroundCollide( bool _enable )
{
    m_groundCollide = _enable;
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to collide with the sphere
//--------------------------------------------------------------------------
void Cloth::setSphereCollide( bool _enable )
{
    m_sphereCollide = _enable;
}


//--------------------------------------------------------------------------
/// @brief get collide object position from mouse movement
//--------------------------------------------------------------------------
void Cloth::setColliderPos( ngl::Vec3 io_colliderpos )
{
    m_colliderpos = io_colliderpos;
}


//--------------------------------------------------------------------------
/// @brief fetch wind force on z axis from UI in real time
//--------------------------------------------------------------------------
void Cloth::setWindF( float _m_windF )
{
    m_windDir.m_z = _m_windF;
}


//--------------------------------------------------------------------------
/// @brief scaling factor for bending string force from UI in real time
//--------------------------------------------------------------------------
void Cloth::setBendResist( float _bendResist )
{
    m_bendResistance = _bendResist;
}


//--------------------------------------------------------------------------
/// @brief scaling factor for shearing string force from UI in real time
//--------------------------------------------------------------------------
void Cloth::setShearResist( float _shearResist )
{
    m_shearResistance = _shearResist;
}


//--------------------------------------------------------------------------
/// @brief scaling cloth stiffness factor from UI in real time
//--------------------------------------------------------------------------
void Cloth::setStiffFactor( float _stiffFactor )
{
    m_stiffFactor = _stiffFactor;
}


//--------------------------------------------------------------------------
/// @brief fetch damping factor from UI in real time
//--------------------------------------------------------------------------
void Cloth::setDampFactor( float _dampFactor )
{
    m_dampFactor = _dampFactor;
}


//--------------------------------------------------------------------------
/// @brief spring constant from UI in real time
//--------------------------------------------------------------------------
void Cloth::setSpringConstV( float _ks )
{
    m_springConst = _ks;
}


//--------------------------------------------------------------------------
/// @brief reset cloth simulation back to initial state (rest position) on XZ plane
//--------------------------------------------------------------------------
void Cloth::resetClothOnXZPlane()
{
    c_resParticles = 30;
    c_numParticles = ( c_resParticles + 1 ) * ( c_resParticles + 1 );
    m_particles.resize( c_numParticles );

    ngl::Vec3 firstPosition( -5.0f, 10.0f, -5.0f );
    m_gapGrid = 10.0f / float( c_resParticles );
    m_gapDiagonal = sqrt(2) * 10.0f / float( c_resParticles );

    // get initial position for each particle
    for(int rowI=0; rowI<( c_resParticles + 1 ); rowI++ )
    {
        firstPosition.m_y = 10.f;
        firstPosition.m_z = -5.f + m_gapGrid * rowI;

        for(int columeI=0; columeI<( c_resParticles+1 ); columeI++ )
        {
            firstPosition.m_x = -5.0f + m_gapGrid * columeI;
            int index = rowI * ( c_resParticles + 1 ) + columeI;
            m_particles[index].setParticlePos( firstPosition );
            m_particles[index].setParticlePrePos( firstPosition );
        }
    }
}


//--------------------------------------------------------------------------
/// @brief draw out cloth after update
//--------------------------------------------------------------------------
void Cloth::draw() const
{
    std::vector<ngl::Vec3> m_particlePositions;
    for (auto p: m_particles)
    {
        // step through each particle in the cloth sim
        // and push their position into the vector for easy accessing
        // as particle holds not only position as attrs
        m_particlePositions.push_back( p.getParticlePos() );
    }

    // select vertex array and set size for this object (every position for a vertex is a ngl::Vec3)
    m_vao->bind();
    m_vao->setData(ngl::SimpleVAO::VertexData( m_particlePositions.size()*sizeof(ngl::Vec3), m_particlePositions[0].m_x) );

    // giving layout for the buffer for this generic vertex attribute
    m_vao->setVertexAttributePointer( 0, 3, GL_FLOAT, sizeof( ngl::Vec3 ), 0 );

    m_vao->setNumIndices( m_particlePositions.size() );
    m_vao->draw();

    //unbind after draw for the next update (then draw)
    m_vao->unbind();
}


//--------------------------------------------------------------------------
/// @brief calculate particle forces and update particle position
/// external forces take into account: gravity, wind, collision
/// internal forces take into account: structural/bend/shear spring
//--------------------------------------------------------------------------
void Cloth::update()
{
    m_pSpring.setKs( m_springConst );

    for(int rowI=0; rowI<=c_resParticles; rowI++)
    {
        for(int columeI=0; columeI<=c_resParticles; columeI++)
        {
             int index = rowI * ( c_resParticles + 1 ) + columeI;
             m_particles[index].setKd( m_dampFactor );

             // structural spring: (1) p(i,j), p(i+1,j); (2) p(i,j), p(i,j+1) ;
             {
                 m_pSpring.setRestlen( m_gapGrid );
                 if ( columeI<c_resParticles )
                 {
                     calSpringBetweenParticles( index, (index+1), m_stiffFactor );
                 }
                 if ( rowI<c_resParticles )
                 {
                     calSpringBetweenParticles( index, (index+c_resParticles+1), m_stiffFactor);
                 }
             }

             // shearing spring: (1) p(i,j), p(i+1,j+1); (2) p(i+1,j), p(i,j+1) ;
             {
                 m_pSpring.setRestlen( m_gapDiagonal );
                 if ( columeI<c_resParticles && rowI<c_resParticles )
                 {
                     calSpringBetweenParticles( index, (index+c_resParticles+2), m_shearResistance );
                 }
                 if ( columeI<c_resParticles && rowI<c_resParticles )
                 {
                     calSpringBetweenParticles( index, (index+c_resParticles+1), m_shearResistance );
                 }
             }

             // bending spring: (1) p(i,j), p(i+2,j); (2) p(i,j), p(i,j+2)
             {
                 m_pSpring.setRestlen(2.f * m_gapGrid);
                 if ( columeI < (c_resParticles-1) )
                 {
                     calSpringBetweenParticles( index, (index+2), m_bendResistance );
                 }
                 if ( rowI < (c_resParticles-1) )
                 {
                     calSpringBetweenParticles( index, (index+2*c_resParticles+2), m_bendResistance );
                 }
              }

             // collision force: with ground; only detect if collision is enabled
             {
                 if( m_groundCollide==true )
                 {
                     m_pCollide.planarCollide( &m_particles[index], .5f );
                     m_particles[index].getTotalForce( m_pCollide.getCollideF() );
                 }
             }

             // collision force: with sphere; only detect if collision is enabled
             {
                 if( m_sphereCollide==true )
                 {
                     m_pCollide.setColliderPos( m_colliderpos );
                     m_pCollide.sphereCollide( &m_particles[index], 2.1f );
                     m_particles[index].getTotalForce( 10.f*m_pCollide.getCollideF() );
                 }
             }


             // gravity
             {
                 m_particles[index].getTotalForce( ngl::Vec3(0.f,c_gravityconst,0.f) );
             }


             // wind force: (1) p(i,j), p(i+1), p(j+1); (2) p(i+1,j), p(i+1,j+1), p(i, j+1);
             {
                 if( columeI<c_resParticles && rowI<c_resParticles )
                 {
                     calWindBetweenParticles( (index+1), index, (index+c_resParticles+1) );
                     calWindBetweenParticles( (index+c_resParticles+2), (index+1), (index+c_resParticles+1) );
                 }
             }


             // for fixed particles: clear the particle total force to zero
             for(int fixed_num=0; fixed_num<3; fixed_num++)
             {
                 if( m_hangTL==true ) { m_particles[fixed_num].setZeroForce(); }
                 if( m_hangTR==true ) { m_particles[c_resParticles-fixed_num].setZeroForce(); }
                 if( m_hangBR==true ) { m_particles[c_numParticles-1-fixed_num].setZeroForce(); }
                 if( m_hangBL==true ) { m_particles[c_numParticles-1-c_resParticles+fixed_num].setZeroForce(); }
             }


             m_particles[index].updatePos();

             // clear all sub-forces to zero for the next calculation
             m_pSpring.setZeroSpringF();
             m_pCollide.clearCollideF();
        }
    }
}


//--------------------------------------------------------------------------
/// @brief calculate spring force between two given particles
/// p1 += force; p2 -= force;
//--------------------------------------------------------------------------
void Cloth::calWindBetweenParticles(int _part1index, int _part2index, int _part3index)
{
    m_pTriangle.setTriParticles( &m_particles[_part1index], &m_particles[_part2index], &m_particles[_part3index] );
    m_windF = ( m_pTriangle.getTriangleN() ) * ( m_pTriangle.getTriangleN().dot( m_windDir ) );
    m_particles[_part1index].getTotalForce( m_windF );
    m_particles[_part2index].getTotalForce( m_windF );
    m_particles[_part3index].getTotalForce( m_windF );
    m_windF.null();
}


//--------------------------------------------------------------------------
/// @brief calculate wind force
/// F=N*(d*Vwind), d=N/|N|
/// wind force is interpolate with direction on angular surface normals
/// every three direct neighboring particles form a triangle
//--------------------------------------------------------------------------
void Cloth::calSpringBetweenParticles(int _part1index , int _part2index, float _forceScale)
{
    m_pSpring.calSpringForce( &m_particles[_part1index], &m_particles[_part2index] );
    //now add force to particles
    m_particles[_part1index].getTotalForce( m_pSpring.getSpringF() * _forceScale );
    m_particles[_part2index].getTotalForce( (-1.f)*m_pSpring.getSpringF() * _forceScale );

}

