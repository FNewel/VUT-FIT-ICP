#ifndef CLASSELEMENT_H
#define CLASSELEMENT_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class ClassElement;
}

class ClassElement : public QWidget
{
    Q_OBJECT

public:
    explicit ClassElement(QWidget *parent = nullptr);
    ~ClassElement();

private slots:
    void on_operationAddButton_clicked();

    void on_attributeAddButton_clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::ClassElement *ui;
    QPoint offset;
};

#endif // CLASSELEMENT_H
