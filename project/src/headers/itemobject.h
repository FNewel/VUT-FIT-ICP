/**
 * UML Editor - ICP Project 2022
 * @file itemobject.h
 * @brief Header File for the ItemObject Class (method/attribute) of class diagram.
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef ITEMOBJECT_H
#define ITEMOBJECT_H

#include <QWidget>


namespace Ui {
class ItemObject;
}

/**
* Item Object Class.
* Class for the Methods and Attributes of the Class Element in class diagram.
*/
class ItemObject : public QWidget
{
    Q_OBJECT

public:
    /**
    *Consturctor for the Item Object (method/attribute).
    *@param parent Parent Widget of the Item Object (Class Element)
    */
    explicit ItemObject(QWidget *parent = nullptr);
    /**
    *Destructor for the Item Object (method/attribute).
    */
    ~ItemObject();
    /**
    *Pointer to ui elements of Item Object (method/attribute).
    */
    Ui::ItemObject *ui;

    /**
    * Type of security of Item Object (method/attribute)
    * This variable holds a value representing the security option for a method or attribute.
    * Legend:
    * 0 - Public (+)
    * 1 - Private (-)
    * 2 - Protected (#)
    * 3 - Package (~)
    */
    int type = 0;   // 0: + | 1: - | 2: # | 3: ~

    /**
    * Name of the Item Object (method/attribute)
    */
    QString value = "";

private slots:
    /**
    * Remove the Item Object (method/attribute)
    * This slot is bound to a signal emitted from a pressed "R" Button located next to the Item Object on the Class Element.
    */
    void on_removeButton_clicked();
    /**
    * Change the security of Item Object
    * This slot is bound to a signal emitted from a changed security Combo Box located next to the Item Object on the Class Element.
    *@param index Index of the selected item (security option)
    */
    void on_comboBox_currentIndexChanged(int index);
    /**
    * Update the name of the Item Object.
    * This slot is bound to a signal emitted from a changed Line Edit located on the Item Object on the Class Element.
    * It updates the name of the Item Object.
    * @param arg1 Strign that the Line Edit was changed to (new name).
    */
    void on_lineEdit_textChanged(const QString &arg1);

};

#endif // ITEMOBJECT_H
