#include "mainwindow.h"
#include "ui_mainwindow.h"


/// \brief ctor
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi( this );
    m_gl = new NGLScene( this) ;

    // add opengl widget: 5 sections for ui and 1 for widget
    m_ui->mainwin_grid_layout->addWidget( m_gl, 1, 0, 5, 1 );

    connect( m_ui->Ground, SIGNAL(toggled(bool)), m_gl, SLOT(setGroundCollide(bool)) );                 //on/off ground collision
    connect( m_ui->WindF_V, SIGNAL(valueChanged(double)), m_gl, SLOT(setWindForce(double)) );           //wind force

    connect( m_ui->HangingTL, SIGNAL(toggled(bool)), m_gl, SLOT(setHangTopL(bool)) );                   //hang on Top Left
    connect( m_ui->HangingTR, SIGNAL(toggled(bool)), m_gl, SLOT(setHangTopR(bool)) );                   //hang on Top Right
    connect( m_ui->HangingBL, SIGNAL(toggled(bool)), m_gl, SLOT(setHangBotL(bool)) );                   //hang on Bottom Left
    connect( m_ui->HangingBR, SIGNAL(toggled(bool)), m_gl, SLOT(setHangBotR(bool)) );                   //hang on Bottom Right

    connect( m_ui->Shear_V, SIGNAL(valueChanged(double)), m_gl, SLOT(setShearFactor(double)) );         //shear resistance
    connect( m_ui->Bend_V, SIGNAL(valueChanged(double)), m_gl, SLOT(setBendFactor(double)) );           //bend resistance
    connect( m_ui->Stiffness_V, SIGNAL(valueChanged(double)), m_gl, SLOT(setStiffnessFactor(double)) ); //stiffness factor
    connect( m_ui->Damping_V, SIGNAL(valueChanged(double)), m_gl, SLOT(setDampingFactor(double)) );     //damping factor
    connect( m_ui->Ks_V, SIGNAL(valueChanged(double)), m_gl, SLOT(setSpringConst(double)) );            //spring constant

    connect( m_ui->objcollide, SIGNAL(toggled(bool)), m_gl, SLOT(setSphereCollide(bool)) );             //on/off sphere collision
}


/// \brief dtor
MainWindow::~MainWindow()
{
    delete m_ui;
}


//--------------------------------------------------------------------------
/// @brief suspend simulation
//--------------------------------------------------------------------------
void MainWindow::on_StopButton_clicked()
{    m_gl->stopSimulate();
}


//--------------------------------------------------------------------------
/// @brief replay simulation
//--------------------------------------------------------------------------
void MainWindow::on_replayButton_clicked()
{
    m_gl->playSimulate();
}


//--------------------------------------------------------------------------
/// @brief reset all status back to initial
//--------------------------------------------------------------------------
void MainWindow::on_RedoButton_clicked()
{
    m_ui->Ground->setCheckState( Qt::Checked );
    m_ui->WindF_V->setValue( -0.6 );

    m_ui->HangingTL->setCheckState( Qt::Checked );
    m_ui->HangingTR->setCheckState( Qt::Checked );
    m_ui->HangingBL->setCheckState( Qt::Checked );
    m_ui->HangingBR->setCheckState( Qt::Checked );

    m_ui->Shear_V->setValue( 1.0 );
    m_ui->Bend_V->setValue( 2.0 );
    m_ui->Stiffness_V->setValue( 1.0 );
    m_ui->Damping_V->setValue( 0.5 );
    m_ui->Ks_V->setValue( 0.7 );

    m_ui->objcollide->setCheckState( Qt::Unchecked );

    m_gl->resetAll();
}
