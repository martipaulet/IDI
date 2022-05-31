
  
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
    // Si fiquem virtual la funcio pasa a ser daqui, per tant, quan es cridi
    // automaticament per qt les funcions initializeGL() i paintGL() es cridaran
    // les funcions daqui i no les del BL2GLWidget.cpp
    virtual void carregaShaders();
    virtual void initializeGL ();
    virtual void paintGL (); 
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void resizeGL(int w, int h);

    void projectTransform ();
    void viewTransform ();

    void modelTransformPatricio1();
    void modelTransformPatricio2();
    void modelTransformPatricio3();
    void mouseMoveEvent(QMouseEvent *e);
    void creaBuffers();
    void modelTransformTerra();

    void ini_camera ();
    void calculaCapsaModel ();
    void calculaCapsaEscena ();
    void calculaCentreEscena ();
    void calculaRadiEscena ();

    GLuint projLoc;
    GLuint viewLoc;
    GLuint VAOPatricio;
    GLuint VAOTerra;

    float FOV, raw, Znear, Zfar, distancia, FOVIni;
    float l,r,b,t;
    float radi;
    float rotate;
    float scale;
    int perspectiva;
    float girTheta, girPsi, x_ant, y_ant;
    
    glm::vec3 OBS, VRP, UP; 
    glm::vec3 escenaMinima, escenaMaxima;
    glm::vec3 centreEscena;
    glm::vec3 centreBasePatricio;

    Model m; // un únic model
};