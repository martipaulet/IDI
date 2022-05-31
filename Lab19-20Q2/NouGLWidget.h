#include "MyGLWidget.h"

class NouGLWidget : public MyGLWidget
{
 Q_OBJECT
	   
 public:
  NouGLWidget (QWidget *parent=NULL);
  ~NouGLWidget ();

 public slots:
    void poncamara1();
    void poncamara2();

 signals:
    void camaraEscena();
    void camaraPov();

 protected:
  virtual void paintGL ();
  virtual void iniCamera ();
  virtual void iniEscena ();
  virtual void viewTransform ();
  virtual void projectTransform ();
  virtual void modelTransformPatricio ();
  void modelTransformPatricio2();
  
  // keyPressEvent - Es cridat quan es prem una tecla
  void keyPressEvent (QKeyEvent *event);
  
 private:
    float distance;
    float zNear, zFar, FovIni, Fov;
    float pat2x = 4;
    float pat2z = 6;
    bool camara2 = false;
    glm::vec3 OBS, VRP, UP;
};
