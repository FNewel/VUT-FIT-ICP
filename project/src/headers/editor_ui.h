/**
 * UML Editor - ICP Project 2022
 * @file editor_ui.h
 * @brief Header File for the MainWindow Class.
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "projectmanager.h"
#include "workscene.h"
#include "workview.h"

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
* Main Window Class.
* Class for the main application window.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
    *Consturctor for the Main Window.
    *@param parent Pointer to parent Widget
    */
    MainWindow(QWidget *parent = nullptr);
    /**
    *Destructor for the Main Window.
    */
    ~MainWindow();

    /**
    * Pointer to the Project Manager.
    */
    ProjectManager *projectManager;


private:
    /**
    *Pointer to ui elements of the Main Window.
    */
    Ui::MainWindow *ui;
    /**
    * Function for the initialization of the Work View.
    * Function initializes the QGraphicsView for the class and sequence diagram scenes.
    * @param workspaceView to the initialized view.
    */
    void InitWorkView(WorkView *workspaceView );
    /**
    * Function for binding signals of the Main Window buttons.
    * Function binds the buttons signals from the main window to their respective slots for in Project Manager.
    * @param projectManager Pointer to the project manager.
    */
    void bindSignals(ProjectManager *projectManager);

private slots:
    /**
    * Redefinition of close event.
    * Used for displaying a Save File prompt before exiting the program.
    * @param event CloseEvent of the application.
    */
    void closeEvent(QCloseEvent *event);
};

extern WorkScene *seq_scene;
extern WorkScene *class_scene;



#endif // MAINWINDOW_H
