/**
 * UML Editor - ICP Project 2022
 * @file projectmanager.h
 * @brief Header file for the ProjectManager Class
 * @author Ondrej Kováč (xkovac57)
 * @author Martin Talajka (xtalaj00)
 */

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

/** 
*Project Manager Class.
*   Class for the Project Manager of the program. The project manager handles the main program operations such as opening files.
*/
class ProjectManager : public QObject
{
    Q_OBJECT
public:
    /**
    *Consturctor for Project Manager.
    * @param parent Pointer to the parent Widget.
    */
    explicit ProjectManager(QObject *parent = nullptr);

    /**
    * String containing the name of curren file.
    */
    QString filename = "";
    /**
    * Path to the file used in undo.
    */
    QString undoPath = "";

    /**
    * Function for creating Undo Save points.
    */
    void saveProjectNow(bool save);

public slots:
    /**
    * Slot for creating a new project.
    * The slot is bound to the New Project action.
    * It asks the user if he wishes to save his work before it removes everything from the scenes.
    * @param value Saving Flag
    */
    void newProject(int value);
    /**
    * Slot for opening an existing project.
    * The slot is bound to the Open Project action.
    * It asks the user if he wishes to save his work before it loads a project from the selected .json file.
    * @param value Saving Flag
    */
    void openProject(int value);    // flag = 0 - basic open project / 1 - undo/redo / 2 - redo open
    /**
    * Slot for saving the project.
    * The slot is bound to the Save action.
    * If the file path to the save file has already been specivied, saves the project. Otherwise acts as Save As.
    * @param value Saving Flag
    */
    void saveProject(int value);
    /**
    * Slot for saving the project under a certain name.
    * The slot is bound to the Save As action.
    * Asks the user for a file path and name to a new save file.
    * @param save Saving Flag
    */
    void saveProjectAs(bool save);
    /**
    * Slot for undoing the last action.
    * The slot is bound to the Undo action.
    * The function returns user a step back. Works for multiple steps back.
    */
    void undoAction();
    /**
    * Slot for redoing the last undone action.
    * The slot is bound to the Undo action.
    * The function redoes the last undone action. Works for only one step.
    */
    void redoAction();
    /**
    * Slot for showing the documentation.
    * The slot is bound to the Show Documentation action.
    * It opens a pdf with the documentation.
    */
    void showDocs();
    /**
    * Slot for exitting the app.
    * The slot is bound to the Quit action.
    * It asks the user if he wishes to save his work before exiting.
    */
    void exitApp();
    /**
    * Slot for generating the .json save file.
    * The slot is called from the save function.
    * @return Text for the save file.
    */
    QByteArray createJson();
};

#endif // PROJECTMANAGER_H
