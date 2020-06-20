#include "NGLScene.h"
#include <ngl/NGLStream.h>


/// \brief ctor
NGLScene::NGLScene( QWidget *parent ) :
                                     QOpenGLWidget(parent),
                                     m_objPosition(0.f),
                                     m_sim(true)
{
    setFocusPolicy( Qt::StrongFocus );
    setFocus();

    this->resize( parent->size() );
}


NGLScene::~NGLScene()
{
    std::cout<<"Quit Cloth Simulation Window \n";
}


//--------------------------------------------------------------------------
/// @brief fetch values from UI in real time
/// wether to collide with ground plane
//--------------------------------------------------------------------------
void NGLScene::setGroundCollide( bool _enable )
{
    m_cloth->setGroundCollide( _enable );
    update();
}


//--------------------------------------------------------------------------
/// @brief fetch wind force on z axis from UI in real time
//--------------------------------------------------------------------------
void NGLScene::setWindForce( double _windForce )
{
    m_cloth->setWindF( float( _windForce ) );
    update();
}


//--------------------------------------------------------------------------
/// @brief wether to fix top left corner of cloth
//--------------------------------------------------------------------------
void NGLScene::setHangTopL( bool _enable )
{
    m_cloth->setHangTL( _enable );
    update();
}


//--------------------------------------------------------------------------
/// @brief wether to fix top right corner of cloth
//--------------------------------------------------------------------------
void NGLScene::setHangTopR( bool _enable )
{
    m_cloth->setHangTR(_enable);
    update();
}


//--------------------------------------------------------------------------
/// @brief wether to fix bottom left corner of cloth
//--------------------------------------------------------------------------
void NGLScene::setHangBotL( bool _enable )
{
    m_cloth->setHangBL( _enable );
    update();
}


//--------------------------------------------------------------------------
/// @brief wether to fix bottom right corner of cloth
//--------------------------------------------------------------------------
void NGLScene::setHangBotR( bool _enable )
{
    m_cloth->setHangBR( _enable );
    update();
}


//--------------------------------------------------------------------------
/// @brief suspend simulation
//--------------------------------------------------------------------------
void NGLScene::stopSimulate()
{
    m_sim = false;
}


//--------------------------------------------------------------------------
/// @brief replay simulation
//--------------------------------------------------------------------------
void NGLScene::playSimulate()
{
    m_sim = true;
}


//--------------------------------------------------------------------------
/// @brief scaling factor for shearing string force from UI in real time
//--------------------------------------------------------------------------
void NGLScene::setShearFactor( double _shearFactor )
{
    m_cloth->setShearResist( float( _shearFactor ) );
    update();
}



//--------------------------------------------------------------------------
/// @brief scaling factor for bending string force from UI in real time
//--------------------------------------------------------------------------
void NGLScene::setBendFactor( double _bendFactor )
{
    m_cloth->setSpringConstV( float( _bendFactor ) );
    update();
}


//--------------------------------------------------------------------------
/// @brief stiffness factor for cloth simulation
//--------------------------------------------------------------------------
void NGLScene::setStiffnessFactor( double _stiffFactor )
{
    m_cloth->setStiffFactor( float( _stiffFactor ) );
    update();
}


//--------------------------------------------------------------------------
/// @brief damping factor for cloth simulation
//--------------------------------------------------------------------------
void NGLScene::setDampingFactor( double _dampFactor )
{
    m_cloth->setDampFactor( float( _dampFactor ) );
    update();
}


//--------------------------------------------------------------------------
/// @brief spring constant for mass-spring model
//--------------------------------------------------------------------------
void NGLScene::setSpringConst( double _ks )
{
    m_cloth->setDampFactor( float( _ks ) );
    update();
}


//--------------------------------------------------------------------------
/// @brief wether to collide with the sphere
//--------------------------------------------------------------------------
void NGLScene::setSphereCollide( bool _enable )
{
    m_cloth->setSphereCollide( _enable );
    update();
}


//--------------------------------------------------------------------------
/// @brief reset cam, simulation and UI value to initial state
/// 'suspend'/'play' is not changed with this function
//--------------------------------------------------------------------------
void NGLScene::resetAll()
{
    m_modelPos.set( 0.0f, 0.0f, 0.0f );
    m_objPosition.set( 0.0f, 0.0f, 0.0f );
    m_objTransform.reset();
    m_mouseTX.identity();
    m_winParam.spinXFace        =  0;
    m_winParam.spinYFace        =  0;
    m_winParam.cam_rotate       =  false;
    m_winParam.cam_translate    =  false;
    m_winParam.alt_flag         =  false;
    m_winParam.originXRot       =  0;
    m_winParam.originYRot       =  0;
    m_winParam.spinXFace        =  0;
    m_winParam.spinYFace        =  0;
    m_winParam.originXPos       =  0;
    m_winParam.originYPos       =  0;

    m_cloth->setDampFactor( 0.5f );
    m_cloth->setWindF( -0.3f );
    m_cloth->setSpringConstV( 0.7f );
    m_cloth->resetClothOnXZPlane();
}


//--------------------------------------------------------------------------
/// @brief move collision sphere by the cursor
/// coded with help from Cos
//--------------------------------------------------------------------------
void NGLScene::calSphereFromMouse()
{
    if( m_winParam.obj_flag==true )
    {
        m_eye.m_x *= (-1);
        ngl::Vec3 planeNormal = m_eye - m_look;
        planeNormal.normalize();

        // side axis; if normal aligns with y axis, cross would be zero
        ngl::Vec3 up( 0, 1, 0 );
        if ( (up.cross( planeNormal)).lengthSquared()==0 ) { up = ngl::Vec3( 0, 0, 1 ); }
        ngl::Vec3 side = up.cross( planeNormal );
        side.normalize();
        up = planeNormal.cross( side );
        up.normalize();

        if ( m_winParam.changeVec2D.lengthSquared() ) { m_winParam.changeVec2D.normalize(); }

        if ( planeNormal.m_y<0 ) { m_winParam.changeVec2D.m_y *= -1.f; }

        m_objPosition += m_winParam.changeVec2D.m_x * 0.2 * side;
        m_objPosition -= m_winParam.changeVec2D.m_y * 0.2 * up;

        m_winParam.changeVec2D.null();
    }
}


//--------------------------------------------------------------------------
/// @brief initialize OpenGL context, load shader and create primitives
//--------------------------------------------------------------------------
void NGLScene::initializeGL()
{
    ngl::NGLInit::instance();
    //background color
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    //enable depth
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE );

    m_cloth = std::make_unique<Cloth>();

    // camera; ref from: https://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/html/classngl_1_1_camera.html
    m_orig_eye = ngl::Vec3( 0.0f, 20.0f, 30.0f );
    m_look = ngl::Vec3( 0.0f, 0.0f, 0.0f );
    ngl::Vec3 up( 0.0f, 1.0f, 0.0f );    //up axis

    m_view = ngl::lookAt( m_orig_eye, m_look, up );
    m_projection = ngl::perspective( 45.0f, float( m_winParam.win_width/m_winParam.win_height ), 0.1f, 200.0f );

    glPointSize( 2 ); //particle size


    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    //shader for particles
    shader->loadShader( "Particle","shaders/PartVertex.glsl","shaders/PartFragment.glsl" );
    shader->use( "Particle" );

    //shader for collision object -> lambert
    createShaderProgram( "Lambert" );
    (*shader)["Lambert"]->use();
    shader->setUniform("lightPos", ngl::Vec3( 0.f, 20.f, 30.f ));
    shader->setUniform("Ld", ngl::Vec3( 2.5f ));
    shader->setUniform("Kd", ngl::Vec3( .75f ));

    //shader for grid primitive -> colour
    createShaderProgram( "Colour" );
    (*shader)["Colour"]->use();
    shader->setUniform( "clr", 1.0f, 1.0f, 1.0f, 1.0f );

    //create primitive shapes
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->createLineGrid( "plane", 20, 20, 20 );
    prim->createSphere( "sphere", 1.5, 120 );

    startTimer( 0 );
}


//--------------------------------------------------------------------------
/// @brief timer event to update cloth simulation
/// state determined by m_sim flag ( controlled by 'stop'/'play' button )
//--------------------------------------------------------------------------
void NGLScene::timerEvent( QTimerEvent * )
{
    if( m_sim==true )
    {
        m_cloth->update();
    }
    update();
}


//--------------------------------------------------------------------------
/// @brief paint background, primitives and cloth
//--------------------------------------------------------------------------
void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport( 0, 0, m_winParam.win_width, m_winParam.win_height );

    // instance library class in the local scope to use
    auto prim=ngl::VAOPrimitives::instance();
    auto shader=ngl::ShaderLib::instance();

    // transform matrix on camera caused by mouse event
    ngl::Mat4 rotX;
    ngl::Mat4 rotY;
    rotX.rotateX( m_winParam.spinXFace );
    rotY.rotateY( m_winParam.spinYFace );
    m_mouseTX = rotY * rotX;
    m_mouseTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseTX.m_m[3][2] = m_modelPos.m_z;

    //this is only needed for moving sphere by cursor
    m_eye = m_mouseTX * m_orig_eye;
    calSphereFromMouse();

    //paint collision sphere
    m_objTransform.reset();
    {
        //paint collision sphere
        m_objTransform.setPosition( m_objPosition.m_x, m_objPosition.m_y+1.5f, m_objPosition.m_z ); //set position for sphere
        loadMatricesToShader("Lambert");
        prim->draw("sphere");
        //collision sphere pos for cloth collision
        ngl::Vec3 _colliderpose;
        _colliderpose.set( m_objPosition.m_x, m_objPosition.m_y+1.5f, m_objPosition.m_z);
        m_cloth->setColliderPos(_colliderpose);
    }

    //paint particles
    {
        shader->use("Particle");
        shader->setUniform( "MVP", m_projection*m_view*m_mouseTX );
        m_cloth->draw();
    }

    //paint guid grid for ground plane
    m_objTransform.reset();
    {
        m_objTransform.setPosition( 0.0f, 0.0f, 0.0f ); //set position for guide grid(also gound plane)
        shader->use("Colour");
        ngl::Mat4 MVP = m_projection * m_view * m_mouseTX * m_objTransform.getMatrix();
        shader->setUniform( "MVP",MVP );
        prim->draw("plane");
    }
}


//--------------------------------------------------------------------------
/// @brief resize OpenGL widget
//--------------------------------------------------------------------------
void NGLScene::resizeGL( int _width, int _height )
{
    m_projection=ngl::perspective( 45.0f, static_cast<float>( _width ) / _height, 0.1f, 200.0f );
    m_winParam.win_width = static_cast<int>( _width * devicePixelRatio() );
    m_winParam.win_height = static_cast<int>( _height * devicePixelRatio() );
}


//--------------------------------------------------------------------------
/// @brief create shader program and load/attach/copile sources for initialization
//--------------------------------------------------------------------------
void NGLScene::createShaderProgram( const std::string &shaderPrefix_ )
{
    auto shader = ngl::ShaderLib::instance();

    auto vertexShader = shaderPrefix_ + "Vertex";
    auto fragmentShader = shaderPrefix_ + "Fragment";
    shader->createShaderProgram( shaderPrefix_ );
    shader->attachShader(vertexShader, ngl::ShaderType::VERTEX);
    shader->attachShader(fragmentShader, ngl::ShaderType::FRAGMENT);
    shader->loadShaderSource(vertexShader, "shaders/"+shaderPrefix_+"Vertex.glsl");
    shader->loadShaderSource(fragmentShader, "shaders/"+shaderPrefix_+"Fragment.glsl");
    shader->compileShader(vertexShader);
    shader->compileShader(fragmentShader);
    shader->attachShaderToProgram(shaderPrefix_, vertexShader);
    shader->attachShaderToProgram(shaderPrefix_, fragmentShader);
    shader->linkProgramObject(shaderPrefix_);
}


//--------------------------------------------------------------------------
/// @brief load MVP, MV etc. matrix into shader uniform variables
//--------------------------------------------------------------------------
void NGLScene::loadMatricesToShader( const std::string &shaderName_ )
{
   ngl::ShaderLib* shader = ngl::ShaderLib::instance();
   shader->use( shaderName_ ); //active shader

   ngl::Mat4 M = m_mouseTX * m_objTransform.getMatrix();
   ngl::Mat4 MV = m_view * M;
   ngl::Mat4 MVP = m_projection * MV;
   ngl::Mat3 normalMatrix = MV;

   normalMatrix.inverse().transpose();
   shader->setUniform( "MV", MV );
   shader->setUniform( "MVP", MVP );
   shader->setUniform( "normalMatrix", normalMatrix );
}



