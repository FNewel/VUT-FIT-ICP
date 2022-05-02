#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "projectmanager.h"
#include "workscene.h"

#include <QMainWindow>
#include "workview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ProjectManager *projectManager;


private:
    Ui::MainWindow *ui;
    void InitWorkView(WorkView *workspaceView );
    void bindSignals(ProjectManager *projectManager);
};



#endif // MAINWINDOW_H
