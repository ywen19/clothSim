/// \file mainwindow.h
/// \brief encapsulates the main window / inherited from QMainWindow
/// \button behavious are defined with private slots methods
/// \author Yi Wen
/// \version 1.0
/// \date 29/5/19 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 1/5/19


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NGLScene.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();

    private:

        Ui::MainWindow  *m_ui;  // ui
        NGLScene        *m_gl;  // opengl widget scene

    private slots:

        // there is no need for 'Quit' button as it connects with slot signal

        //--------------------------------------------------------------------------
        /// @brief suspend simulation
        //--------------------------------------------------------------------------
        void on_StopButton_clicked();

        //--------------------------------------------------------------------------
        /// @brief replay simulation
        //--------------------------------------------------------------------------
        void on_replayButton_clicked();

        //--------------------------------------------------------------------------
        /// @brief reset all status back to initial
        //--------------------------------------------------------------------------
        void on_RedoButton_clicked();

};

#endif // MAINWINDOW_H
