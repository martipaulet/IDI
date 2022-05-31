#include "MyLabel.h"
// constructor
MyLabel::MyLabel(QWidget *parent): QLabel(parent) {
    
    // Inicialització d'atributs si cal
    pos = 100;
    prev_text = this->text();
}

// implementació slots

void MyLabel::TextTruncat(QString s) {
    prev_text = s;
    QString aux = s;
    aux.truncate(pos); //per si inicialment la barra de truncar 
                        //ja esta per la meitat
    emit setText(aux);  //truncar i enviar 
}

void MyLabel::truncarLabel(int n) {
    pos = n;
    QString aux = prev_text;
    aux.truncate(pos);
    emit setText(aux);
}
//signals no se implementan pero si se declaran en el .h