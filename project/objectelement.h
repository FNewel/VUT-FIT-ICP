#ifndef OBJECTELEMENT_H
#define OBJECTELEMENT_H

#include <QWidget>
#include "workscene.h"

namespace Ui {
class ObjectElement;
}

class ObjectElement : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectElement(QWidget *parent = nullptr);
    ~ObjectElement();

private:
    Ui::ObjectElement *ui;
};

extern WorkScene *class_scene;
extern WorkScene *seq_scene;


#endif // OBJECTELEMENT_H
