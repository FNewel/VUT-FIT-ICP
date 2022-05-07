/**
 * UML Editor - ICP Project 2022
 * @file editor_ui.cpp
 * @brief Source File for the MainWindow Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#include "headers/editor_ui.h"
#include "headers/projectmanager.h"
#include "headers/workscene.h"
#include "headers/workview.h"
#include "ui_editor_ui.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMessageBox>

#define MAX_WORKSPACE_SCALE 10


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create new project manager and bind signals
    this->projectManager = new ProjectManager();
    bindSignals(projectManager);

    //Set the tab widget as central widget
    this->setCentralWidget(ui->tabWidget);
    
    //Set up views to use custom context menus
    ui->classView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->seqView->setContextMenuPolicy(Qt::CustomContextMenu);

    //Initialize the views
    InitWorkView(ui->classView);
    InitWorkView(ui->seqView);

    //Set Class Diagram tab as the default
    ui->tabWidget->setCurrentIndex(0);

}


MainWindow::~MainWindow()
{
    delete this->projectManager;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    (void)event;
    // Check if project is empty (clean)
    if(!class_scene->classes.empty()){
        // Prompt to save project
        int reply = QMessageBox(QMessageBox::Information, "Save file", "Do you want to save this file?", QMessageBox::Yes|QMessageBox::No).exec();
        if (reply == 16384){    // Yes button clicked
            seq_scene->projectManager->saveProject(0);
        }
    }
}

//Connect all the main window buttons to slots in project manager
void MainWindow::bindSignals(ProjectManager *projectManager){
    connect(ui->actionNew_Project, &QAction::triggered, projectManager, &ProjectManager::newProject);
    connect(ui->actionOpen_Project , &QAction::triggered, projectManager, &ProjectManager::openProject);
    connect(ui->actionSave , &QAction::triggered, projectManager, &ProjectManager::saveProject);
    connect(ui->actionSave_As, &QAction::triggered, projectManager, &ProjectManager::saveProjectAs);
    connect(ui->actionUndo, &QAction::triggered, projectManager, &ProjectManager::undoAction);
    connect(ui->actionRedo, &QAction::triggered, projectManager, &ProjectManager::redoAction);
    connect(ui->actionExit, &QAction::triggered, projectManager, &ProjectManager::exitApp);
    connect(ui->actionDocumentation, &QAction::triggered, projectManager, &ProjectManager::showDocs);
}

//Create new work scene
void MainWindow::InitWorkView(WorkView *workspaceView)
{
    auto *workspaceScene = new WorkScene(workspaceView);
    workspaceView->setScene(workspaceScene);
    //Maximum Workspace Size
    workspaceScene->setSceneRect(0,0,workspaceView->width()*MAX_WORKSPACE_SCALE, workspaceView->height()*MAX_WORKSPACE_SCALE);
    workspaceView->activeScene = workspaceScene;
}

