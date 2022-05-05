#ifndef OBJECTELEMENT_H
#define OBJECTELEMENT_H

#include <QComboBox>
#include <QMap>
#include <QWidget>
#include "workscene.h"

class MessageAnchor;
class SeqMessage;
class ActivationElement;

extern WorkScene *class_scene;
extern WorkScene *seq_scene;



namespace Ui {
class ObjectElement;
}

class ObjectElement : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectElement(QWidget *parent = nullptr);
    QGraphicsLineItem * createLifeLine(ObjectElement *objectPtr);

    QString name = "";

    void updateClasses();
    ~ObjectElement();

    Ui::ObjectElement *ui; //ui is public in this case so that message names can find current class of object
    //Dictionary of proxy widgets and message anchors
    QMap<QGraphicsProxyWidget*, MessageAnchor*> anchors;

    //Vector of proxies (keys) -> needed to find last added in dictionary
    QVector<QGraphicsProxyWidget *> proxyList;

    MessageAnchor *leftAnchor = nullptr;

    MessageAnchor *rightAnchor = nullptr;

private:
    QPoint offset;
    QGraphicsLineItem * lifeLine = nullptr;

    QGraphicsProxyWidget *leftAnchorProxy = nullptr;

    QGraphicsProxyWidget *rightAnchorProxy = nullptr;



protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private slots:

    void increaseLifeLine();
    void decreaseLifeLine();
    void deleteObject();

    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_lineEdit_textChanged(const QString &arg1);
};


//Message Anchor Class
namespace Ui {
class MessageAnchor;
}

class MessageAnchor : public QWidget
{
    Q_OBJECT

public:
    explicit MessageAnchor(QWidget *parent = nullptr);
    ~MessageAnchor();
    QGraphicsProxyWidget * proxy = nullptr;
    SeqMessage *message = nullptr;
    ActivationElement *activation = nullptr;

private:
    Ui::MessageAnchor *ui;
    QGraphicsItem* destructionIcon = nullptr;




protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void moveEvent(QMoveEvent *event);
};



//Message Class
class SeqMessage : public QWidget
{
    Q_OBJECT
public:
    explicit SeqMessage(QWidget *parent = nullptr);
    ~SeqMessage();

    void addComboBox();
    void updateMessages();
    void updateCBoxLoc();
    void updateArrowHead();
    void setArrow(int arrowType);

    QGraphicsLineItem *messageLine = nullptr;

    QComboBox *messageName = nullptr;
    QGraphicsProxyWidget *messageNameProxy = nullptr;
    QGraphicsItem *arrowHeadProxy = nullptr;

    MessageAnchor *sourceAnchor = nullptr;
    MessageAnchor *destAnchor = nullptr;

    QPoint sourcePos;
    QPoint destPos;

signals:

};

//Activation Class
class ActivationElement : public QWidget
{
    Q_OBJECT
public:
    explicit ActivationElement(QWidget *parent = nullptr);
    ~ActivationElement();

    QGraphicsRectItem *actRect = nullptr;

    MessageAnchor *sourceAnchor = nullptr;
    MessageAnchor *destAnchor = nullptr;

    QPoint sourcePos;
    QPoint destPos;

signals:

};


#endif // OBJECTELEMENT_H
