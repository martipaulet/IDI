
  
// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
  protected:
    void projectTransform ();
    void viewTransform ();
    // Si fiquem virtual la funcio pasa a ser daqui, per tant, quan es cridi
    // automaticament per qt les funcions initializeGL() i paintGL() es cridaran
    // les funcions daqui i no les del BL2GLWidget.cpp
    virtual void carregaShaders();
    virtual void initializeGL ();
    virtual void paintGL (); 
    virtual void modelTransform();
    virtual void keyPressEvent (QKeyEvent *event);
    void creaBuffers();
    void modelTransformTerra();
    GLuint projLoc;
    GLuint viewLoc;
    GLuint vpatLoc;
    GLuint cpatLoc;
    
    GLuint VAO;
    GLuint VAOTerra;
    float rotacio = 0;
    Model m; // un únic model
};