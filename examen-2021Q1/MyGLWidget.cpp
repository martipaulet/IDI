// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL() {
  pintaPat = 0;
  mode = 0;
  mode2 = 0;
  blanc = 1;
  ExamGLWidget::initializeGL();
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  if (pintaPat) {
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  
  else {
    glBindVertexArray(VAO_Cub);
    // Pintem el cub1
    modelTransformCub (2.0, float(mode2%3));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Pintem el cub2
    modelTransformCub (2.5, float(mode2%3)+1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pintem el cub3
    modelTransformCub (3.0, float(mode2%3)+2);
    glDrawArrays(GL_TRIANGLES, 0, 36);  
  }

  glBindVertexArray(0);
  
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{ 
  escala = escala/0.5;
  angle = (angle*2*M_PI)/3.0;
  TG = glm::mat4(1.f);
  TG = rotate(TG,angle,glm::vec3(0,1,0));
  TG = translate(TG, glm::vec3(5,0,0));
  TG = scale(TG, glm::vec3 (escala, escala, escala));
  // En aquest mètode has de substituir aquest codi per construir la 
  // transformació geomètrica (TG) del cub usant els paràmetres adientment
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = rotate(TG, float((mode*2*M_PI)/3.0), glm::vec3(0,1,0));
  TG = translate(TG, glm::vec3(5,0,0));
  TG = scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = rotate(TG,float(-90*M_PI/180), glm::vec3(0,1,0));
  TG = translate(TG, -centreBasePat);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificarv
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  { //lookat(obs,vrp,up)
    View = glm::lookAt(glm::vec3(0,2*radiEsc,0), glm::vec3(0,0,0), glm::vec3(1,0,0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    if (ra >= 1) Proj = glm::ortho(-ra*radiEsc, ra*radiEsc, -radiEsc, radiEsc, zn, zf);
    else Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc/ra, radiEsc/ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
    pintaPat = !pintaPat;
    break;
	}
  case Qt::Key_1: {
    mode = 0;
    break;
	}
  case Qt::Key_2: {
    mode = 1;
    break;
	}
  case Qt::Key_3: {
    mode = 2;
    break;
	}
  case Qt::Key_F: {
      blanc = !blanc;
      if (!blanc) colFocus = glm::vec3 (1, 1, 0);
      else colFocus = glm::vec3 (1, 1, 1);
      glUniform3fv (colfocusLoc, 1, &colFocus[0]);
    break;
	}
  case Qt::Key_C: {
    camPlanta = !camPlanta;
    projectTransform();
    viewTransform ();
    break;
	}
  case Qt::Key_Right: {
    ++mode2;
    mode = mode2;
    break;
	}
  case Qt::Key_Left: {
    --mode2;
    mode = mode2;
    break;
	}
  case Qt::Key_R: {
    initializeGL();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

