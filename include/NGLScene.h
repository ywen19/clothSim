/// \file NGLScene.h
/// \brief encapsulates NGLScene to create opengl, paint, update
/// \Methods in public slots are connected with UI for real-time update
/// \An instance of class Cloth (pointer) is made in this class
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef NGLSCENE_H
#define NGLSCENE_H

#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/Util.h>
#include <math.h>

#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
#include <QGuiApplication>

#include "windowParams.h"
#include "Particles.h"
#include "Cloth.h"


class NGLScene : public QOpenGLWidget
{
    Q_OBJECT
    public:

        NGLScene( QWidget *parent );
        ~NGLScene();

    public slots:

        //--------------------------------------------------------------------------
        /// @brief fetch values from UI in real time
        //--------------------------------------------------------------------------

        void setGroundCollide( bool _enable );
        void setWindForce( double _windForce );

        void setHangTopL( bool _enable );
        void setHangTopR( bool _enable );
        void setHangBotL( bool _enable );
        void setHangBotR( bool _enable );

        void setShearFactor( double _shearFactor );
        void setBendFactor( double _bendFactor );
        void setStiffnessFactor( double _stiffFactor );
        void setDampingFactor( double _dampFactor );
        void setSpringConst( double _ks );

        void setSphereCollide( bool _enable );
        void calSphereFromMouse();

        void stopSimulate();
        void playSimulate();
        void resetAll();


    protected:

        //--------------------------------------------------------------------------
        /// @brief initialize OpenGL context, load shader and create primitives
        //--------------------------------------------------------------------------
        void initializeGL() override;

        //--------------------------------------------------------------------------
        /// @brief paint background, primitives and cloth
        //--------------------------------------------------------------------------
        void paintGL() override;

        //--------------------------------------------------------------------------
        /// @brief resize OpenGL widget
        //--------------------------------------------------------------------------
        void resizeGL( int _width, int _height ) override;

        //--------------------------------------------------------------------------
        /// @brief load MVP, MV etc. matrix into shader uniform variables
        //--------------------------------------------------------------------------
        void loadMatricesToShader( const std::string &shaderName_ );

        //--------------------------------------------------------------------------
        /// @brief create shader program for initialization
        //--------------------------------------------------------------------------
        void createShaderProgram( const std::string &shaderPrefix_ );

        ngl::Vec3               m_orig_eye;     //original positin for camera
        ngl::Vec3               m_eye;          // position for camera
        ngl::Vec3               m_look;         // point camera to
        ngl::Vec3               m_modelPos;     // model matrix
        ngl::Mat4               m_view;         // viewing matrix
        ngl::Mat4               m_projection;   // projection matrix
        ngl::Mat4               m_mouseTX;      //transformation matrix on camera caused by cursor
        ngl::Transformation     m_objTransform; //to store object transform and be converted into matrix for MVP
        std::unique_ptr<Cloth>  m_cloth;        //Build the cloth with a pointer point to the instance


   private:

        winParam    m_winParam;
        ngl::Vec3   m_objPosition;
        bool        m_sim;
        bool        m_isOnXZ;
        int         m_preClothType; // this is set before next update; if changed->reset

        //define mouse movements
        void mouseMoveEvent( QMouseEvent *_event ) override;
        void mousePressEvent( QMouseEvent *_event ) override;
        void mouseReleaseEvent( QMouseEvent *_event ) override;
        void wheelEvent( QWheelEvent *_event ) override;
        void keyPressEvent( QKeyEvent *_event ) override;
        void keyReleaseEvent( QKeyEvent *_event ) override;

        //time event: for simulating and animating update
        void timerEvent( QTimerEvent * ) override;

};


#endif // NGLSCENE_H
