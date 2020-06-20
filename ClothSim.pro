# This specifies the exe name
TARGET=Cloth_Simulation
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

cache()

# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/main.cpp \
          $$PWD/src/mainwindow.cpp \
          $$PWD/src/NGLScene.cpp \
          $$PWD/src/NGLSceneMouseControls.cpp \
          $$PWD/src/Particles.cpp \
          $$PWD/src/Cloth.cpp \
          $$PWD/src/Spring.cpp \
          $$PWD/src/Triangle.cpp \
          $$PWD/src/Collide.cpp

HEADERS+= $$PWD/include/mainwindow.h \
          $$PWD/include/NGLScene.h \
          $$PWD/include/windowParams.h \
          $$PWD/include/Particles.h \
          $$PWD/include/Cloth.h \
          $$PWD/include/Spring.h \
          $$PWD/include/Triangle.h \
          $$PWD/include/Collide.h

FORMS+= $$PWD/UI/mainwindow.ui

# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= README.md \
              $$PWD/shaders/*.glsl

# were are going to default to a console app
CONFIG += console
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}
