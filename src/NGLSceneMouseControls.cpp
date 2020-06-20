#include <QMoveEvent>
#include <QKeyEvent>
#include <iostream>
#include "NGLScene.h"


/* Functions for evaluating changes on collision sphere/cam caused by cursor;
 * Update will happen under certain test conditions;
 * Alt+LMB->Cam Spin; Alt+RMB->Cam trans; Wheel->Zoom;
 * LMB->obj_flag=true indicates the collision sphere is moved by the cursor;
 * Instance(pointer) of struct windowParams is made in NGLScene
 * for accessing/updating/evaluating data from cursor;
 * ------------------------------------------------------------------------- */



void NGLScene::mouseMoveEvent( QMouseEvent *_event )
{
    //Alt+LMB (camera rotate/spin)
    //obj_flag=false in the case so sphere will not move with the cursor
    if ( m_winParam.alt_flag==true && m_winParam.cam_rotate==true && _event->buttons()==Qt::LeftButton )
    {
        m_winParam.obj_flag=false;
        //cursor pos change between current time event and pervious one
        int deltaX = _event->x() - m_winParam.originXRot;
        int deltaY = _event->y() - m_winParam.originYRot;
        //calculate spin value with cursor changes
        m_winParam.spinXFace += static_cast<int>( 0.30f*deltaY );
        m_winParam.spinYFace += static_cast<int>( 0.30f*deltaX );
        m_winParam.originXRot = _event->x(); //update value
        m_winParam.originYRot = _event->y();
        update();
    }

    //Alt+RMB (camera trans)
    //obj_flag=false in the case so sphere will not move with the cursor
    if ( m_winParam.alt_flag==true && m_winParam.cam_translate==true && _event->buttons()==Qt::RightButton )
    {
        m_winParam.obj_flag=false;
        //cursor pos change between current time event and pervious one
        int deltaX = static_cast<int>( _event->x()-m_winParam.originXPos );
        int deltaY = static_cast<int>( _event->y()-m_winParam.originYPos );
        //calculate spin value with cursor changes
        m_winParam.originXPos = _event->x();
        m_winParam.originYPos = _event->y();
        //update modelpos (for MVP matrix (mouseTX) in NGLScene)
        m_modelPos.m_x += deltaX*INCREMENT;
        m_modelPos.m_y -= deltaY*INCREMENT;
        update();
    }

    //LMB (collision sphere trans)
    //alt_flag=false so cam will not move with the cursor
    if( m_winParam.alt_flag==false && _event->buttons()==Qt::LeftButton )
    {
        m_winParam.obj_flag = true;
        //cursor pos change between current time event and pervious one
        int deltaX = static_cast<int>( _event->x() - m_winParam.originXPosforSphere );
        int deltaY = static_cast<int>( _event->y() - m_winParam.originYPosforSphere );
        //calculate spin value with cursor changes
        m_winParam.originXPosforSphere = _event->x();
        m_winParam.originYPosforSphere = _event->y();
        //update (vector) change vector direction
        m_winParam.changeVec2D.set( float(deltaX), float(deltaY), 0.f );
        update();
    }
}


void NGLScene::wheelEvent( QWheelEvent *_event )
{
    //wheel for zoom; Const ZOOM defined in windowParams
    if ( _event->delta()>0 )
    {
        m_modelPos.m_z += ZOOM;
    }
    else if ( _event->delta()<0 )
    {
        m_modelPos.m_z -= ZOOM;
    }
    update();
}


void NGLScene::mousePressEvent( QMouseEvent *_event )
{
    //when mouse button is pressed read in current cursor position
    if ( _event->button()==Qt::LeftButton )
    {
        m_winParam.originXRot=_event->x();
        m_winParam.originYRot=_event->y();

        //for sphere trans
        m_winParam.originXPosforSphere=_event->y();
        m_winParam.originYPosforSphere=_event->x();
        m_winParam.cam_rotate=true;
    }

    else if ( _event->button()==Qt::RightButton )
    {
        m_winParam.originXPos=_event->x();
        m_winParam.originYPos=_event->y();
        m_winParam.cam_translate=true;
    }
}


void NGLScene::mouseReleaseEvent( QMouseEvent *_event )
{
    //when mouse button is released
    //disable mouse control to both cam and collision sphere
    if ( _event->button()==Qt::RightButton )
    {
        m_winParam.cam_translate=false;
        m_winParam.obj_flag = false;
    }
    if ( _event->button()==Qt::LeftButton )
    {
        m_winParam.cam_rotate=false;
        m_winParam.obj_flag = false;
    }
}


void NGLScene::keyPressEvent( QKeyEvent *_event )
{
    switch ( _event->key() )
    {
        case Qt::Key_Alt : m_winParam.alt_flag=true; break;
        default : break; //default for any other keys to avoid undefined behaviours
    }
      update(); //update
}


void NGLScene::keyReleaseEvent( QKeyEvent *_event )
{
    if ( _event->key()==Qt::Key_Alt )
    {
        m_winParam.alt_flag = false;
    }
    update();
}




