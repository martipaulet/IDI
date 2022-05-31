#include "MyQPushButton.h"

int MyQPushButton::c1 = -1;
int MyQPushButton::c2 = -1;
int MyQPushButton::c3 = -1;
int MyQPushButton::trio = 0;

// constructor
MyQPushButton::MyQPushButton(QWidget *parent): QPushButton(parent) {
}

// implementació slots
void MyQPushButton::MakeStart(){
    emit startpressed();    //startpressed fuerza a changecolor
    if (c1 == c2 and c1 == c3) {
        ++trio;
        emit increase(trio);    //increase fuerza a display(int) del lcd
    }
}

void MyQPushButton::ChangeColor(){
    int x = rand()%4;
    if (x == 0) setStyleSheet("background-color: rgb(255, 0, 0);");
    else if (x == 1) setStyleSheet("background-color: rgb(0, 255, 0);");
    else if (x == 2) setStyleSheet("background-color: rgb(0, 0, 255);");
    else if (x == 3) setStyleSheet("background-color: rgb(255, 255, 0);");

    if (objectName() == "pushButton_3") c1 = x;
    else if (objectName() == "pushButton_4") c2 = x;
    else if (objectName() == "pushButton_5") c3 = x;
}

void MyQPushButton::Reset() {
    c1 = -1;
    c2 = -1;
    c3 = -1;
    trio = 0;
    emit increase(trio);
}

void MyQPushButton::Reset2() {
    setStyleSheet("background-color: rgb(144, 144, 144);");
}
