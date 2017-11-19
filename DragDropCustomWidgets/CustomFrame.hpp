#ifndef CustomFrame_HPP
#define CustomFrame_HPP

#include <QFrame>

class QDropEvent;
class QDragEnterEvent;
class QDragLeaveEvent;

class CustomFrame : public QFrame
{
   public:
      explicit CustomFrame(QWidget *parent = nullptr);

   protected:
      void dropEvent(QDropEvent *event) override;
      void dragEnterEvent(QDragEnterEvent *event) override;
      void dragLeaveEvent (QDragLeaveEvent *event) override;
};

#endif // CustomFrame_HPP
