#include "editor_ui.h"
#include "ui_editor_ui.h"
#include "workscene.h"
#include "workview.h"


#include <QGraphicsTextItem>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WorkView *workspaceView = new WorkView(this);

    this->setCentralWidget(workspaceView);

    workspaceView->setContextMenuPolicy(Qt::CustomContextMenu);

    workspaceView->activeScene = InitWorkspace(workspaceView);



}


MainWindow::~MainWindow()
{
    delete ui;
}

WorkScene* MainWindow::InitWorkspace(WorkView *workspaceView)
{
    auto *workspaceScene = new WorkScene(workspaceView);
    workspaceView->setScene(workspaceScene);
    return workspaceScene;
}

