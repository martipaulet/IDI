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
    case Qt::Key_R: { //rotar
      rotate += float(45*M_PI/180.0f);
      break;
	  }
    case Qt::Key_O: { //canviar optica
      if (perspectiva == 1) perspectiva = 0;
      else perspectiva = 1;
      projectTransform();
      break;
    }
    case Qt::Key_X: { //zoom out
      if (perspectiva == 1) FOV += 0.1;
      else {
        l -= 0.1;
        r += 0.1;
        b -= 0.1;
        t += 0.1;
      }
      projectTransform();
      break;
    }
    case Qt::Key_Z: { //zoom in
      if (perspectiva == 1) FOV -= 0.1;
      else {
        l += 0.1;
        r -= 0.1;
        b += 0.1;
        t -= 0.1;
      }
      //projectTransform();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
	makeCurrent();
	
	if(e->x() > x_ant) girPsi += 0.03;
	else if(e->x() < x_ant) girPsi -= 0.03;
	
	if(e->y() > y_ant) girTheta += 0.03;
	else if(e->y() < y_ant) girTheta -= 0.03;
	
	x_ant = e->x();
	y_ant = e->y();
	viewTransform();
	update();
}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL ();
    
    rotate = 0.0;
    perspectiva = 1;
    glEnable (GL_DEPTH_TEST);
  
    calculaCapsaModel();
    calculaCapsaEscena();
    calculaCentreEscena();
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
  FOVIni = 2.0 * glm::asin(radi / distancia);
  FOV = FOVIni;

  girTheta = 0.0f;
  girPsi = 0.0f;

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
/*
void MyGLWidget::modelTransformPatricio(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, rotate, glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(scale));
  //movem model al (0,0,0)
  transform = glm::translate(transform, -centreBasePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
*/

void MyGLWidget::modelTransformPatricio1(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(2.0,0.0,2.0));
  transform = glm::scale(transform, glm::vec3(scale));
  //movem model al (0,0,0)
  transform = glm::translate(transform, -centreBasePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPatricio2(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, 90*float(M_PI/180), glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(scale));
  //movem model al (0,0,0)
  transform = glm::translate(transform, -centreBasePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPatricio3(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(-2.0, 0.0, -2.0));
  transform = glm::rotate(transform, 180*float(M_PI/180), glm::vec3(0,1,0));
  transform = glm::scale(transform, glm::vec3(scale));
  //movem model al (0,0,0)
  transform = glm::translate(transform, -centreBasePatricio);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}



void MyGLWidget::modelTransformTerra(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  //transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform () 
{
  glm::mat4 Proj;
	if (perspectiva) Proj = glm::perspective (FOV, raw, Znear, Zfar);
  else Proj = glm::ortho(l, r, b, t, Znear, Zfar);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () 
{
	/*SENSE EULER
	glm::mat4 View = glm::lookAt (OBS, VRP, UP);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
	*/

  //EULER
	glm::mat4 View(1.0);
	View = glm::translate(View, glm::vec3(0, 0, -distancia));
	View = glm::rotate(View, girTheta, glm::vec3(1, 0, 0));
	View = glm::rotate(View, girPsi, glm::vec3(0, 1, 0));
	View = glm::translate(View, -VRP);
	glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
	
}


void MyGLWidget::paintGL () {

    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
    // useu els paràmetres que considereu (els que hi ha són els de per defecte)
    //glViewport (0, 0, ample, alt);

    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray (VAOPatricio);
    modelTransformPatricio1();
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    modelTransformPatricio2();
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    modelTransformPatricio3();
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);


    modelTransformTerra();
    glBindVertexArray (VAOTerra);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::creaBuffers() {
    

    m.load ("/home/martipaulet/UPC_FIB/IDI/models/Patricio.obj");

    //El VAO el faig servir per en homero
    glGenVertexArrays(1, &VAOPatricio);
    glBindVertexArray(VAOPatricio);

    GLuint VBO_Patricio[2]; //posicio i color
    glGenBuffers(2, VBO_Patricio);

    //VBO[0]->posicio
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[0]);
    glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_vertices (), GL_STATIC_DRAW);
    // Activem l'atribut vpatLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    //VBO[1]->color
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Patricio[1]);
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

  GLuint VBOTerra[2]; //posicio i color
  glGenBuffers(2, VBOTerra);

  //VBOTerra[0]->posicio
  glBindBuffer(GL_ARRAY_BUFFER, VBOTerra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(terra), terra, GL_STATIC_DRAW);
  // Activem l'atribut vterraLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  //VBOTerra[1]->color
  glBindBuffer(GL_ARRAY_BUFFER, VBOTerra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colorTerra), colorTerra, GL_STATIC_DRAW);
  // Activem l'atribut colorTerraLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders() { // declarem-lo també en MyGLWidget.h
    BL2GLWidget::carregaShaders(); // cridem primer al mètode de BL2GLWidget
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
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
  scale = 1.0f/(maxy-miny); //alçada q volem menys la que te el model
  centreBasePatricio= glm::vec3((minx+maxx)/2, miny, (minz+maxz)/2);
}

void MyGLWidget::calculaCapsaEscena ()
{
  escenaMaxima.x = 2.5f;
  escenaMaxima.y = 4.0f;
  escenaMaxima.z = 2.5f;

  escenaMinima.x = -2.5f;
  escenaMinima.y = 0.0f;
  escenaMinima.z = -2.5f;
}

void MyGLWidget::calculaRadiEscena ()
{
  radi = glm::distance(escenaMinima, escenaMaxima)/2.0f;
}

void MyGLWidget::calculaCentreEscena ()
{
  centreEscena = (escenaMaxima + escenaMinima)/(glm::vec3(2.0f));
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
	
	projectTransform();
	glViewport(0, 0, w, h);
}

MyGLWidget::~MyGLWidget() {
}

