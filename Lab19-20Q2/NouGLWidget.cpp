#include "NouGLWidget.h"

#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::paintGL() {
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //--------------------------------------------------------
  // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);  
  // pintem terra
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 12);

  //--------------------------------------------------------
  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Pat);
  // pintem el Patricio
  modelTransformPatricio();
  glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

  modelTransformPatricio2();
  glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

  //--------------------------------------------------------
  glBindVertexArray(0);
}

void NouGLWidget::iniCamera ()
{
  distance = 2*radiEsc;
  zNear = distance-radiEsc;
  zFar = distance + radiEsc;
  FovIni = 2*asin(radiEsc/distance);
  Fov = FovIni;
  rav = 1.0;
  angleX = M_PI/6.0;
  angleY = 0.0;  
  
  projectTransform ();
  viewTransform ();
}

void NouGLWidget::iniEscena ()
{
  MyGLWidget::iniEscena();

  glm::vec3 escenaMax = glm::vec3 (8,4,8);
  glm::vec3 escenaMin = glm::vec3 (0,0,0);

  centreEsc = (escenaMax+escenaMin)/(glm::vec3(2.0f));
  radiEsc = glm::distance(escenaMax,escenaMin)/2.0f;
}

void NouGLWidget::modelTransformPatricio ()
{
    // Codi per a la TG necessària
  glm::mat4 patTG = glm::mat4(1.0f);
  patTG = glm::translate(patTG, glm::vec3(2,0,1));
  patTG = glm::rotate(patTG,float(90*M_PI/180),glm::vec3(0,1,0));
  patTG = glm::scale(patTG, glm::vec3(2*escalaPat, 2*escalaPat, 2*escalaPat));
  patTG = glm::translate(patTG, -centreBasePat);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}


void NouGLWidget::modelTransformPatricio2 ()
{
    // Codi per a la TG necessària
  glm::mat4 patTG = glm::mat4(1.0f);
  patTG = glm::translate(patTG, glm::vec3(pat2x,0,pat2z));
  patTG = glm::rotate(patTG,float(180*M_PI/180),glm::vec3(0,1,0));
  patTG = glm::scale(patTG, glm::vec3(3*escalaPat, 3*escalaPat, 3*escalaPat));
  patTG = glm::translate(patTG, -centreBasePat);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}


void NouGLWidget::viewTransform() {
  glm::mat4 View;
  if (camara2 == false){
    View = glm::translate(View, glm::vec3(0, 0, -distance));
    View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);
  }
  else {
    OBS = glm::vec3(pat2x,3.5,pat2z);
    VRP = glm::vec3(pat2x,3.5,0);
    UP = glm::vec3(0,1,0); 
    View = glm::lookAt(OBS,VRP,UP);
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void NouGLWidget::projectTransform() {
  glm::mat4 Proj;  // Matriu de projecció
  if (camara2) {
    Proj = glm::perspective(float(M_PI/2.0), rav, float(0.1), zFar);
  }
  else {
    Proj = glm::perspective(Fov, rav, zNear, zFar);
  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: {
      if (pat2z < 8.0)
        pat2z += 0.5;
      viewTransform();
      projectTransform();   
      break;
    }
    case Qt::Key_W: {
      if (pat2z > 0.5)
        pat2z -= 0.5;
      viewTransform();
      projectTransform();  
      break;
    }
    case Qt::Key_A: {
      if (pat2x > 0.5)
        pat2x -= 0.5;
      viewTransform();
      projectTransform();
      break;
    }
    case Qt::Key_D: {
      if (pat2x < 7.5)
        pat2x += 0.5;
      viewTransform();
      projectTransform();
      break;
    }
    case Qt::Key_C: {
      camara2 = !camara2;
      if (camara2) emit camaraPov();
      else emit camaraEscena();
      viewTransform();
      projectTransform();
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }
  update();
}

//camara escena
void NouGLWidget::poncamara1() {
  makeCurrent();
  camara2 = false;
  viewTransform();
  projectTransform();
  update();
}

//camara POV
void NouGLWidget::poncamara2() {
  makeCurrent();
  camara2 = true;
  viewTransform();
  projectTransform();
  update();
}


  
