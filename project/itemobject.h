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

    int type = 0;   // 0: + | 1: - | 2: # | 3: ~
    QString value = "";

private slots:
    void on_removeButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::ItemObject *ui;
};

#endif // ITEMOBJECT_H
