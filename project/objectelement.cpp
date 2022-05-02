#include "objectelement.h"
#include "ui_objectelement.h"
#include <QDebug>

ObjectElement::ObjectElement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectElement)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Object Name");

    foreach(ClassElement *classElement, class_scene->classes){
        qDebug() << classElement;
    }
}

ObjectElement::~ObjectElement()
{
    //Remove pointer to this object element on destruction
    seq_scene->objects.removeOne(this);
    delete ui;
}
