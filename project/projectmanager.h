#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit ProjectManager(QObject *parent = nullptr);

    QString filename = "";

public slots:
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs(bool save);
    void undoAction();
    void redoAction();
    void showDocs();
    void exitApp();
    QByteArray createJson();
};

#endif // PROJECTMANAGER_H
