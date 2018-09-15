/********************************************************************************************
 ** CustomWindow is a custom QWidget that allows tou to create custom windows for your app.
 ** Copyright (C) 2016  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 3 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/
#ifndef CustomWindow_H
#define CustomWindow_H

#include <QWidget>

class QMenu;

namespace Ui
{
    class CustomWindow;
}

/**
 * @brief The CustomWindow class represents the window titlebar with the main widgets container. In the
 * constructor it is possible to define the type of the titlebar with custom buttons. If a menu
 * would linke to be added in the titlebar, the setTitlebarMenu() is the method.
 *
 * To add the functionlity with widgets defined by user it is necessary to call setCentralWidget() with the
 * widget that have been created and the name of the window.
 */
class CustomWindow : public QWidget
{
    Q_OBJECT
        
    public:
        /**
         * @brief The TitleMode defines the type of titlebar that will be shown.
         */
        enum class TitleMode { CleanTitle = 0, OnlyCloseButton, MenuOff, MaxMinOff, FullScreenMode, MaximizeModeOff, MinimizeModeOff, FullTitle };
        /**
         * @brief CustomWindow Main constructor that configures de UI with interal parameters.
         * @param parent The parent widget.
         */
        explicit CustomWindow(QWidget *parent = nullptr);
        /**
         * @brief CustomWindow destructor.
         */
        ~CustomWindow();
        /**
         * @brief setTitlebarMode With this method the type of titlebar is defined based on TitleMode enum.
         * @param flag The type of titlebar.
         */
        void setTitlebarMode(const TitleMode &flag);
        /**
         * @brief setTitlebarMenu With this method a custom menu for the titlebar can be set.
         * @param menu The custom menu.
         * @param icon The path to the icon.
         */
        void setTitlebarMenu(QMenu *menu, const QString &icon = "");
        /**
         * @brief setCentralWidget This is the main method of the class which is defined the central
         * widget that will be set as centralWidget in the window.
         * @param widget The further centralwidget.
         * @param widgetName The name of the window.
         */
        void setCentralWidget(QWidget *widget, const QString &widgetName);

    protected:
		/**
		 * @brief moveWindow Method that moves the window to the new position when the window
		 * is moved.
		 * @param e The mouse event that sets the new position.
		 */
		void moveWindow(QMouseEvent *e);

    private:
        /**
         * @brief ui User interface module.
         */
        Ui::CustomWindow *ui = nullptr;
        /**
         * @brief dragPosition Increment of the position movement.
         */
        QPoint mDragPosition;
        /**
         * @brief m_titleMode Flags that defines the current titlebar mode.
         */
        TitleMode mTitleMode = TitleMode::FullTitle;
        /**
         * @brief moveWidget Specifies if the window is in move action.
         */
        bool mMoveWidget = false;
        /**
         * @brief inResizeZone Specifies if the mouse is in resize zone.
         */
        bool mInResizeZone = false;
        /**
         * @brief allowToResize Specifies if the mouse is allowed to resize.
         */
        bool mAllowResize = false;
        /**
         * @brief resizeVerSup Specifies if the resize is in the top of the window.
         */
        bool mResizeVerTop = false;
        /**
         * @brief resizeHorEsq Specifies if the resize is in the left of the window.
         */
        bool mResizeHorLeft = false;
        /**
         * @brief resizeDiagSupEsq Specifies if the resize is in the top left of the window.
         */
        bool mResizeDiagTopLeft = false;
        /**
         * @brief resizeDiagSupDer Specifies if the resize is in the top right of the window.
         */
        bool mResizeDiagTopRight = false;
        /**
          + @brief Pixels around the border to mouse cursor change.
          */
        const static int PIXELS_TO_ACT = 5;

        /**
         * @brief mouseMoveEvent Overloaded member that moves of resizes depending of the
         * configuration sotred at mousePressEvent().
         * @param e The mouse event.
         */
        void mouseMoveEvent(QMouseEvent *e);
        /**
         * @brief mousePressEvent Overloaded member that stores that changes the cursor and
         * configures the side that is changing.
         * @param e The mouse event.
         */
        void mousePressEvent(QMouseEvent *e);
        /**
         * @brief mouseReleaseEvent Overloaded member that removes the configuration set in mousePressEvent().
         * @param e The mouse event.
         */
        void mouseReleaseEvent(QMouseEvent *e);
        /**
         * @brief mouseDoubleClickEvent Overloadad member that maximize/restore the window if is
         * doubleclicked and the position of the mouse is not the top left of the window (menu zone).
         * @param e The mouse event.
         */
        void mouseDoubleClickEvent(QMouseEvent *e);
        /**
         * @brief paintEvent Overloaded method that allows to customize the styles of the window.
         */
        void paintEvent (QPaintEvent *);
        /**
         * @brief resizeWindow Method that calculates the resize and new position of the window an
         * does this actions.
         * @param e The mouse event to calculate the new position and size.
         */
        void resizeWindow(QMouseEvent *e);
        /**
         * @brief maximizeBtnClicked Maximizes or restores the window depending on the last status.
         */
        void maximizeBtnClicked();
        /**
         * @brief minimizeBtnClicked Minimizes or restores the window depending on the last status.
         */
        void minimizeBtnClicked();
        /**
         * @brief resizeWhenVerCursor Method that resizes when the cursor is type VerCursor.
         * @param p The position of the mouse.
         */
        void resizeWhenVerCursor (const QPoint &p);
        /**
         * @brief resizeWhenHorCursor Method that resizes when the cursor is type HorCursor.
         * @param p The position of the mouse.
         */
        void resizeWhenHorCursor (const QPoint &p);
        /**
         * @brief resizeWhenBDiagCursor Method that resizes when the cursor is type  BDiagCursor.
         * @param p The position of the mouse.
         */
        void resizeWhenBDiagCursor (const QPoint &p);
        /**
         * @brief resizeWhenFDiagCursor Method that resizes when the cursor is type FDiagCursor.
         * @param p The position of the mouse.
         */
        void resizeWhenFDiagCursor (const QPoint &p);
};

#endif // CustomWindow_H
