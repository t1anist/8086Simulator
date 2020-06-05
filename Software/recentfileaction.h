#ifndef RECENTFILEACTION_H
#define RECENTFILEACTION_H

#include <QAction>
#include <QDebug>

class RecentFileAction : public QAction
{
    Q_OBJECT
public:
    explicit RecentFileAction(QObject *parent = nullptr);

public slots:
    void openRecent();

signals:
    void openFile(QString);

};

#endif // RECENTFILEACTION_H
