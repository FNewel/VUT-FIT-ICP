#ifndef WORKSCENE_H
#define WORKSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>

//#include "classelement.h"

class ClassElement;
class ObjectElement;


class WorkScene : public QGraphicsScene
{
    Q_OBJECT
public:
    QVector <ClassElement *> classes;
    QVector <ObjectElement *> objects;
    explicit WorkScene(QObject *parent = nullptr);
    ~WorkScene();

public slots:
    void spawnNewClass(const QPointF local);
    void spawnNewObject(const QPointF local);
    void spawnNewText(const QPointF local);
    void spawnNewActivation(const QPointF local);
    void removeLine(QGraphicsItem *line);
    void addLineArrow(int where, QGraphicsItem *line, int type);

signals:

};

#endif // WORKSCENE_H
