#ifndef ITEMOBJECT_H
#define ITEMOBJECT_H

#include <QWidget>

namespace Ui {
class ItemObject;
}

class ItemObject : public QWidget
{
    Q_OBJECT

public:
    explicit ItemObject(QWidget *parent = nullptr);
    ~ItemObject();

private slots:


    void on_removeButton_clicked();

private:
    Ui::ItemObject *ui;
};

#endif // ITEMOBJECT_H
