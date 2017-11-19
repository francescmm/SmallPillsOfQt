#include "CustomList.hpp"
#include <QMimeData>
#include <QDrag>

CustomList::CustomList (QWidget *parent) : QListWidget(parent)
{
}

void CustomList::startDrag(Qt::DropActions supportedActions)
{
   Q_UNUSED (supportedActions)

   auto mimeData = new QMimeData ();
   mimeData->setData("application/x-item", currentItem ()->text ().toUtf8 ());

   auto drag = new QDrag(this);
   drag->setMimeData (mimeData);
   drag->exec(Qt::MoveAction);
}
