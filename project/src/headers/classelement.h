/**
 * UML Editor - ICP Project 2022
 * @file classelement.h
 * @brief Header File for the ClassElement Class of class diagram.
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef CLASSELEMENT_H
#define CLASSELEMENT_H

#include "workscene.h"
#include "classlines.h"
#include "itemobject.h"
#include "classlines.h"
#include "objectelement.h"
#include "ui_classelement.h"

#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include <QPointer>
#include <QLine>
#include <QPainter>
#include <QGraphicsLineItem>


namespace Ui {
class ClassElement;
}

/**
*   Class Element Class.
*   Class for the Class Element of class diagram.
*/
class ClassElement : public QWidget
{
    Q_OBJECT

public:
    /**
    *Consturctor for Class Element.
    */
    explicit ClassElement(QWidget *parent = nullptr);
    /**
    *Destructor for Class Element.
    */
    ~ClassElement();

    /**
    *Pointer to ui elements of Class Element.
    */
    Ui::ClassElement *ui;

    /**
    *Name of the class.
    *Name of the class displayed in the Line Edit above the Class Element in class diagram.
    */
    QString name = "";

    /**
    * Vector of Class Attributes.
    * Vector that holds pointers to ItemObjects that represent attributes of the class in class diagram.
    */
    QVector <ItemObject*> attributes;
    /**
    * Vector of Class Methods.
    * Vector that holds pointers to ItemObjects that represent methods of the class in class diagram.
    */
    QVector <ItemObject*> methods;

    /**
    * Vector of Lines.
    * Vector that holds pointers to ClassLines that connect this Class Element to other elements in class diagram.
    */
    QVector <ClassLines*> lineItems;

private slots:
    /**
    * Slot for adding an operation (method) to the Class Element.
    *This slot is bound to a signal emitted from a pressed "Add Method" button located on the Class Element.
    *
    */
    void on_operationAddButton_clicked();
    /**
    * Slot for adding an attribute to the Class Element.
    *This slot is bound to a signal emitted from a pressed "Add Attribute" button located on the Class Element.
    */
    void on_attributeAddButton_clicked();
    /**
    * Slot for deleting the Class Element.
    *This slot is bound to a signal emitted from a pressed "R" button located on the Class Element.
    */
    void on_pushButton_clicked();
    /**
    * Slot for editation of Class Element's name.
    * This slot is called every time the name of the Class Element is changed.
    */
    void on_name_input_textChanged(const QString &arg1);


protected:
    /**
    * Redefinition of mousePressEvent for Class Element.
    * Used for creation of ClassLines and setting the offset of mouse.
    */
    virtual void mousePressEvent(QMouseEvent *event);
    /**
    * Redefinition of mouseMoveEvent for Class Element.
    * Used to move the Class Element around the scene.
    */
    virtual void mouseMoveEvent(QMouseEvent *event);
    /**
    * 
    */
    virtual void linePosCheck(ClassLines *lines, bool update);

private:
    /**
    * Offset of Mouse.
    * Offset of mouse related to the Class Element. This prevents the class from "snapping" to mouse position when trying to move it.
    */
    QPoint offset;
};

/**
* Pointer to a global Class Scene variable.
* A global pointer referencing the class scene.
*/
extern WorkScene *class_scene;
/**
* Pointer to a global Sequence Scene variable.
* A global pointer referencing the sequence scene.
*/
extern WorkScene *seq_scene;

#endif // CLASSELEMENT_H
