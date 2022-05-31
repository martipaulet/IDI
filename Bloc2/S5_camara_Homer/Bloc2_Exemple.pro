TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp

INCLUDEPATH += /home/martipaulet/UPC_FIB/IDI/Model
SOURCES += /home/martipaulet/UPC_FIB/IDI/Model/model.cpp