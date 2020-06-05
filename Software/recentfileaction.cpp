#include "recentfileaction.h"

RecentFileAction::RecentFileAction(QObject *parent) : QAction(parent)
{

}

void RecentFileAction::openRecent(){
    emit openFile(this->text());
}
