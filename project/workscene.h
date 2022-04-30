#ifndef WORKSCENE_H
#define WORKSCENE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>


class WorkScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit WorkScene(QObject *parent = nullptr);

public slots:
    void spawnNewClass(const QPointF local);
    void spawnNewObject(const QPointF local);
    void spawnNewText(const QPointF local);
    void spawnNewActivation(const QPointF local);


signals:

};

#endif // WORKSCENE_H
