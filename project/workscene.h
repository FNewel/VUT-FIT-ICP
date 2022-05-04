#ifndef WORKSCENE_H
#define WORKSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>

class ClassElement;
class ObjectElement;
class ActorElement;


class WorkScene : public QGraphicsScene
{
    Q_OBJECT
public:
    QVector <ClassElement *> classes;
    QVector <ObjectElement *> objects;
    QVector <ActorElement *> actors;
    explicit WorkScene(QObject *parent = nullptr);
    ~WorkScene();

public slots:
    void spawnNewClass(const QPointF local);
    void spawnNewObject(const QPointF local);
    void spawnNewText(const QPointF local);
    void spawnNewActivation(const QPointF local);
    void spawnNewActor(const QPointF local);
    void removeLine(QGraphicsItem *line);
    void addLineArrow(int where, QGraphicsItem *line, int type);

    void setArrow(QGraphicsItem *msgLine, int arrowType);
    void removeMessage(QGraphicsItem *msgLine);




};

#endif // WORKSCENE_H
