#ifndef CUSTOMLIST_HPP
#define CUSTOMLIST_HPP

#include <QListWidget>

class CustomList : public QListWidget
{
   public:
      explicit CustomList (QWidget *parent = nullptr);

   protected:
      void startDrag(Qt::DropActions supportedActions) override;
};

#endif // CUSTOMLIST_HPP
