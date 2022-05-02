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

    QString type = "+";
    QString value = "";

private slots:


    void on_removeButton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::ItemObject *ui;
};

#endif // ITEMOBJECT_H
