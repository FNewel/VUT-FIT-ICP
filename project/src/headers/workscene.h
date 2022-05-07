/**
 * UML Editor - ICP Project 2022
 * @file workscene.h
 * @brief Header File for the WorkScene Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef WORKSCENE_H
#define WORKSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>


class ClassElement;
class ObjectElement;
class ActorElement;
class SeqMessage;
class MessageAnchor;
class ActivationElement;
class ProjectManager;

/**
* Work Scene Class
* Class for work scenes (sequence and class scene)
*/
class WorkScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
    * Vector of Class Elements
    * This vector is filled with Class Elements in the class scene
    */
    QVector <ClassElement *> classes;
    /**
    * Vector of Object Elements
    * This vector is filled with Object Elements in the sequence scene
    */
    QVector <ObjectElement *> objects;
    /**
    * Vector of Actor Elements
    * This vector is filled with Actor Elements in the sequence scene
    */
    QVector <ActorElement *> actors;
    /**
    * Vector of Sequence Messages
    * This vector is filled with Messages in the sequence scene
    */
    QVector <SeqMessage *> messages;
    /**
    * Vector of Activation Elements
    * This vector is filled with Activation Elements in the sequence scene
    */
    QVector <ActivationElement *> activations;

    /**
    * Pointer to the Project Manager
    */
    ProjectManager* projectManager = nullptr;

    /**
    * Bool for message creation.
    * Flips between true and false to indicate if a first point of message has been selected.
    */
    bool msgClicked = false;
    /**
    * Bool for activation creation.
    * Flips between true and false to indicate if a first point of activation has been selected.
    */
    bool actClicked = false;
    /**
    *   Consturctor for the Work Scene.
    *   @param parent Pointer to parent widget.
    */
    explicit WorkScene(QObject *parent = nullptr);
    /**
    * Deconstructor for Work Scene.
    */
    ~WorkScene();

public slots:
    /**
    * Spawns a new Class Element to class Diagram.
    * This slot is bound to a signal emitted from a "New Class" action in context menu of the class scene.
    * It spawns a new class at the location of the mouse click.
    * @param local Spawn position.
    */
    void spawnNewClass(const QPointF local);
    /**
    * Spawns a new Object Element to class Diagram.
    * This slot is bound to a signal emitted from a "New Object" action in context menu of the sequence scene.
    * It spawns a new object at the location of the mouse click.
    * @param local Spawn position.
    */
    void spawnNewObject(const QPointF local);
    /**
    * Spawns a new Text Element to class or sequence Diagram (not implemented).
    * This slot is bound to a signal emitted from a "New Text" action in context menu of the class or sequence scene.
    * It spawns a new text at the location of the mouse click.
    * @param local Spawn position.
    */
    void spawnNewText(const QPointF local);
    /**
    * Spawns a new Actor Element to sequence Diagram.
    * This slot is bound to a signal emitted from a "New Object" action in context menu of the sequence scene.
    * It spawns a new actor at the location of the mouse click.
    * @param local Spawn position.
    */
    void spawnNewActor(const QPointF local);
    /**
    * Destroys a selected Class Line.
    * This slot is bound to a signal emitted from a "Remove Line" action in context menu of the class scene (when a line is selected).
    * @param line Pointer to the deleted line.
    */
    void removeLine(QGraphicsItem *line);
    /**
    * Appends an arrow to the end of the Class Line.
    * This slot is bound to a signal emitted from the actions in context menu of the class scene (when a line is selected).
    * @param where Whether the source or the target of the line is being altered.
    * @param line Pointer to the edited line.
    * @param type Type of the arrow.
    */
    void addLineArrow(int where, QGraphicsItem *line, int type);
    /**
    * Sets the arrow type of a Message in sequence diagram.
    * This slot is bound to a signal emitted from the actions in context menu of the sequence scene (when a message is selected).
    * @param msgLine Pointer to the edited message.
    * @param arrowType Type of the message.
    */
    void setArrow(QGraphicsItem *msgLine, int arrowType);
    /**
    * Removes the selected message.
    * This slot is bound to a signal emitted from the "Remove Message"action in context menu of the sequence scene (when a message is selected).
    * @param msgLine Pointer to the deleted message.
    */
    void removeMessage(QGraphicsItem *msgLine);
};

#endif // WORKSCENE_H
