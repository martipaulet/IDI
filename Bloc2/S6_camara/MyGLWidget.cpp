// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <vector>

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
void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
		  rotate += float(M_PI)/4.0f;
		  break;
	  }
    case Qt::Key_O: {
		  if (perspectiva == 1) perspectiva = 0;
      else perspectiva = 1;
      projectTransform();
		  break;
	  }

    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL ();

    rotate = 0.0f;
    perspectiva = 1;
    glEnable (GL_DEPTH_TEST);

    calculaCapsaModel();
    calculaCapsaEscena();
    calculaCentreEscnea();
    calculaRadiEscena();
  
    ini_camera();
}

void MyGLWidget::ini_camera () 
{
  // distancia pot ser valor qualsevol que peremti veure tota lescena
  distancia = radi * 2.0;
  Znear = distancia - radi;
  Zfar = distancia + radi;
  raw = 1.0f;
  FOVIni = 2.0 * asin(radi / distancia);
  FOV = FOVIni;

  l = -radi;
  r = radi;
  b = -radi;
  t = radi;
  projectTransform();
  
  OBS = glm::vec3(centreEscena.x,centreEscena.y,distancia);
  VRP = glm::vec3(centreEscena.x,centreEscena.y,centreEscena.z);
  UP = glm::vec3(0,1,0);
  viewTransform();
}

void MyGLWidget::modelTransformHomer(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate (transform, rotate, glm::vec3 (0, 1, 0));
  //moure model al (0,0,0)
  transform = glm::translate(transform, -modelCentreBase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  //transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::paintGL () {

    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //glViewport (0, 0, ample, alt);

    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray (VAOHomer);
    modelTransformHomer();
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (VAOTerra);
    modelTransformTerra();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::creaBuffers() {
    
    m.load ("/home/martipaulet/UPC_FIB/IDI/models/HomerProves.obj");

    //El VAO el faig servir per en homero
    glGenVertexArrays(1, &VAOHomer);
    glBindVertexArray(VAOHomer);

    GLuint VBO_HomerPos;
    glGenBuffers(1, &VBO_HomerPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerPos);
    glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_vertices (), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

   
    GLuint VBO_HomerCol;
    glGenBuffers(1, &VBO_HomerCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerCol);
    glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_matdiff (), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);


    glm::vec3 terra[6] = {
      glm::vec3(2.5, 0.0, 2.5),
      glm::vec3(2.5, 0.0, -2.5),
      glm::vec3(-2.5, 0.0, -2.5),

      glm::vec3(2.5, 0.0, 2.5),
      glm::vec3(-2.5, 0.0, -2.5),
      glm::vec3(-2.5, 0.0, 2.5)
    };

    glm::vec3 colorTerra[6] = {
      glm::vec3( 1.0, 0.0, 0.0),
      glm::vec3( 1.0, 0.0, 0.0),
      glm::vec3( 1.0, 0.0,0.0),
      glm::vec3( 1.0, 0.0, 0.0),
      glm::vec3( 1.0, 0.0, 0.0),
      glm::vec3( 1.0, 0.0,0.0)
    };

    //El VAOTerra es per dibuixar el terra
    glGenVertexArrays(1, &VAOTerra);
    glBindVertexArray(VAOTerra);

    GLuint VBOTerraPos;
    glGenBuffers(1, &VBOTerraPos);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTerraPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terra), terra, GL_STATIC_DRAW);
  

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    GLuint VBOTerraCol;
    glGenBuffers(1, &VBOTerraCol);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTerraCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorTerra), colorTerra, GL_STATIC_DRAW);
    

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders() { // declarem-lo també en MyGLWidget.h

    BL2GLWidget::carregaShaders(); // cridem primer al mètode de BL2GLWidget
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::viewTransform () {
	glm::mat4 View = glm::lookAt (OBS, VRP, UP);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform () {
  glm::mat4 Proj;
	if (perspectiva) Proj= glm::perspective (FOV, raw, Znear, Zfar);
  else Proj = glm::ortho (l, r, b, t, Znear, Zfar);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = m.vertices()[0];
  miny = maxy = m.vertices()[1];
  minz = maxz = m.vertices()[2];
  for (unsigned int i = 3; i < m.vertices().size(); i+=3)
  {
    if (m.vertices()[i+0] < minx)
      minx = m.vertices()[i+0];
    if (m.vertices()[i+0] > maxx)
      maxx = m.vertices()[i+0];
    if (m.vertices()[i+1] < miny)
      miny = m.vertices()[i+1];
    if (m.vertices()[i+1] > maxy)
      maxy = m.vertices()[i+1];
    if (m.vertices()[i+2] < minz)
      minz = m.vertices()[i+2];
    if (m.vertices()[i+2] > maxz)
      maxz = m.vertices()[i+2];
  }
  scale = 4.0f/(maxy-miny); //altura que volem entre la que realment te
  modelCentreBase = glm::vec3((maxx+minx)/2,miny,(maxz+minz)/2);
}

void MyGLWidget::calculaCapsaEscena ()
{
  escenaMaxima.x = 2.5;
  escenaMaxima.y = 4.0f;
  escenaMaxima.z = 2.5;

  escenaMinima.x = -2.5;
  escenaMinima.y = 0.0f;
  escenaMinima.z = -2.5;
}

void MyGLWidget::calculaRadiEscena ()
{
  radi = distance(escenaMinima, escenaMaxima)/2.0;
}

void MyGLWidget::calculaCentreEscnea ()
{
  centreEscena = (escenaMaxima + escenaMinima) / glm::vec3(2.0);;
}

void MyGLWidget::resizeGL (int w, int h) 
{
  BL2GLWidget::resizeGL(w,h);
  float rav = float(w) / float(h);
  //perspectiva
  if (perspectiva) {
    //si rav > 1 nomes cal fer aixo
    raw = rav;
    //sino cal modificar el FOV perque no quedin espais als costats
    if (rav < 1.0) FOV = 2.0*atan(tan(FOVIni/2.0)/rav);
  }
  //ortogonal
  else {
    if (rav > 1.0) {
      l = -radi*rav;
      r = radi*rav;
      b = -radi;
      t = radi;
    }
    if (rav < 1.0) {
      l = -radi;
      r = radi;
      b = -radi*rav;
      t = radi*rav;
    }
  }
	
	projectTransform ();
	glViewport(0, 0, w, h);
}

MyGLWidget::~MyGLWidget() {
}

