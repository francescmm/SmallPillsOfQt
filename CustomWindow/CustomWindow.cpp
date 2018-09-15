#include "CustomWindow.h"
#include "ui_CustomWindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QCursor>
#include <QSettings>
#include <QStyleOption>
#include <QMenu>

CustomWindow::CustomWindow(QWidget *parent) : QWidget(parent), ui(new Ui::CustomWindow)
{
    QFile file(":/CustomWindow.css");

    if (file.open(QFile::ReadOnly))
    {
        setStyleSheet(file.readAll());
        file.close();
    }

    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);

    ui->titleBar->setMouseTracking(true);
    ui->LTitle->setMouseTracking(true);
    ui->tbMenu->setMouseTracking(true);
    ui->pbMin->setMouseTracking(true);
    ui->pbMax->setMouseTracking(true);
    ui->pbClose->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);

    addAction(ui->actionClose);

    connect(ui->pbMin, &QPushButton::clicked, this, &CustomWindow::minimizeBtnClicked);
    connect(ui->pbMax, &QPushButton::clicked, this, &CustomWindow::maximizeBtnClicked);
    connect(ui->pbClose, &QPushButton::clicked, this, &CustomWindow::close);

    QSettings settings("dialogs.ini", QSettings::IniFormat);
    const auto geo = settings.value("geometry").toRect();
    const auto desktopSize = QApplication::desktop();

    if (geo.height() > 0 and geo.x() < desktopSize->width() and geo.width() > 0 and geo.y() < desktopSize->height())
		setGeometry(geo);

	if (settings.value("maximized").toBool())
    {
		showMaximized();
        ui->pbMax->setIcon(QIcon(":/ui/images/app_rest.png"));
    }
}

CustomWindow::~CustomWindow()
{
	QSettings settings("dialogs.ini", QSettings::IniFormat);
	settings.setValue("geometry", geometry());
	settings.setValue("maximized", isMaximized());

    delete ui;
}

void CustomWindow::mouseMoveEvent(QMouseEvent *e)
{
    const auto xMouse = e->pos().x();
    const auto yMouse = e->pos().y();
    const auto wWidth = geometry().width();
    const auto wHeight = geometry().height();

    if (mMoveWidget)
    {
        mInResizeZone = false;
        moveWindow(e);
    }
    else if (mAllowResize)
        resizeWindow(e);
    else if (xMouse >= wWidth - PIXELS_TO_ACT) //Cursor right side
    {
        mInResizeZone = true;

        if (yMouse >= wHeight - PIXELS_TO_ACT)
            setCursor(Qt::SizeFDiagCursor);
        else if (yMouse <= PIXELS_TO_ACT)
            setCursor(Qt::SizeBDiagCursor);
        else
            setCursor(Qt::SizeHorCursor);

        resizeWindow(e);
    }
    else if (xMouse <= PIXELS_TO_ACT) //Cursor left side
    {
        mInResizeZone = true;

        if (yMouse >= wHeight - PIXELS_TO_ACT)
            setCursor(Qt::SizeBDiagCursor);
        else if (yMouse <= PIXELS_TO_ACT)
            setCursor(Qt::SizeFDiagCursor);
        else
            setCursor(Qt::SizeHorCursor);

        resizeWindow(e);
    }
    else if (yMouse >= wHeight - PIXELS_TO_ACT) //Cursor low side
    {
        mInResizeZone = true;
        setCursor(Qt::SizeVerCursor);

        resizeWindow(e);
    }
    else if (yMouse <= PIXELS_TO_ACT) //Cursor top side
    {
        mInResizeZone = true;
        setCursor(Qt::SizeVerCursor);

        resizeWindow(e);
    }
    else
    {
        mInResizeZone = false;
        setCursor(Qt::ArrowCursor);
    }

    e->accept();
}

void CustomWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        const auto xPos = e->pos().x();
        const auto yPos = e->pos().y();

        if (mInResizeZone)
        {
            mAllowResize = true;

            if (yPos <= PIXELS_TO_ACT)
            {
                if (xPos <= PIXELS_TO_ACT)
                    mResizeDiagTopLeft = true;
                else if (xPos >= geometry().width() - PIXELS_TO_ACT)
                    mResizeDiagTopRight = true;
                else
                    mResizeVerTop = true;
            }
            else if (xPos <= PIXELS_TO_ACT)
                mResizeHorLeft = true;
        }
        else if (xPos >= PIXELS_TO_ACT and xPos < ui->titleBar->geometry().width()
                 and yPos >= PIXELS_TO_ACT and yPos < ui->titleBar->geometry().height())
        {
            mMoveWidget = true;
            mDragPosition = e->globalPos() - frameGeometry().topLeft();
        }
    }

    e->accept();
}

void CustomWindow::mouseReleaseEvent(QMouseEvent *e)
{
    mMoveWidget = false;
    mAllowResize = false;
    mResizeVerTop = false;
    mResizeHorLeft = false;
    mResizeDiagTopLeft = false;
    mResizeDiagTopRight = false;

    e->accept();
}

void CustomWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    const auto tbMenuGeo = ui->tbMenu->geometry();
    const auto titleBarGeo = ui->titleBar->geometry();
    const auto xPos = e->pos().x();
    const auto yPos = e->pos().y();

    if (xPos < tbMenuGeo.right() and yPos < tbMenuGeo.bottom() and xPos >= tbMenuGeo.x() and yPos >= tbMenuGeo.y() and ui->tbMenu->isVisible())
        close();
    else if (mTitleMode != TitleMode::FullScreenMode and xPos < titleBarGeo.width() and yPos < titleBarGeo.height())
        maximizeBtnClicked();

    e->accept();
}

void CustomWindow::paintEvent (QPaintEvent *)
{
    QStyleOption opt;
    opt.init (this);

    QPainter p(this);
    style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void CustomWindow::moveWindow(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
	{
        move(e->globalPos() - mDragPosition);
		e->accept();
	}
}

void CustomWindow::resizeWindow(QMouseEvent *e)
{
    if (mAllowResize)
	{
        const auto cursorShape = cursor().shape();

        switch (cursorShape)
        {
            case Qt::SizeVerCursor:
                resizeWhenVerCursor(e->pos());
            break;
            case Qt::SizeHorCursor:
                resizeWhenHorCursor(e->pos());
            break;
            case Qt::SizeBDiagCursor:
                resizeWhenBDiagCursor(e->pos());
            break;
            case Qt::SizeFDiagCursor:
                resizeWhenFDiagCursor(e->pos());
            break;
            default: break;
        }

		e->accept();
	}
}

void CustomWindow::resizeWhenVerCursor (const QPoint &p)
{
    auto yMouse = p.y();
    auto wWidth = geometry().width();
    auto wHeight = geometry().height();

    if (mResizeVerTop)
    {
        int newY = geometry().y() + yMouse;
        int newHeight = wHeight - yMouse;

        if (newHeight > minimumSizeHint().height())
        {
            resize(wWidth, newHeight);
            move(geometry().x(), newY);
        }
    }
    else
        resize(wWidth, yMouse+1);
}

void CustomWindow::resizeWhenHorCursor (const QPoint &p)
{
    auto xMouse = p.x();
    auto wWidth = geometry().width();
    auto wHeight = geometry().height();

    if (mResizeHorLeft)
    {
        int newX = geometry().x() + xMouse;
        int newWidth = wWidth - xMouse;

        if (newWidth > minimumSizeHint().width())
        {
            resize(newWidth, wHeight);
            move(newX, geometry().y());
        }
    }
    else
        resize(xMouse, wHeight);
}

void CustomWindow::resizeWhenBDiagCursor(const QPoint &p)
{
    auto xMouse = p.x();
    auto yMouse = p.y();
    auto wWidth = geometry().width();
    auto wHeight = geometry().height();
    auto newX = 0;
    auto newWidth = 0;
    auto newY = 0;
    auto newHeight = 0;

    if (mResizeDiagTopRight)
    {
        newX = geometry().x();
        newWidth = xMouse;
        newY = geometry().y() + yMouse;
        newHeight = wHeight - yMouse;
    }
    else
    {
        newX = geometry().x() + xMouse;
        newWidth = wWidth - xMouse;
        newY = geometry().y();
        newHeight = yMouse;
    }

    if (newWidth >= minimumSizeHint().width() and newHeight >= minimumSizeHint().height())
    {
        resize(newWidth, newHeight);
        move(newX, newY);
    }
    else if (newWidth >= minimumSizeHint().width())
    {
        resize(newWidth, wHeight);
        move(newX, geometry().y());
    }
    else if (newHeight >= minimumSizeHint().height())
    {
        resize(wWidth, newHeight);
        move(geometry().x(), newY);
    }
}

void CustomWindow::resizeWhenFDiagCursor(const QPoint &p)
{
    auto xMouse = p.x();
    auto yMouse = p.y();
    auto geoX = geometry().x();
    auto geoY = geometry().y();
    auto wWidth = geometry().width();
    auto wHeight = geometry().height();

    if (mResizeDiagTopLeft)
    {
        int newX = geoX + xMouse;
        int newWidth = wWidth - xMouse;
        int newY = geoY + yMouse;
        int newHeight = wHeight - yMouse;

        if (newWidth >= minimumSizeHint().width() and newHeight >= minimumSizeHint().height())
        {
            resize(newWidth, newHeight);
            move(newX, newY);
        }
        else if (newWidth >= minimumSizeHint().width())
        {
            resize(newWidth, wHeight);
            move(newX, geoY);
        }
        else if (newHeight >= minimumSizeHint().height())
        {
            resize(wWidth, newHeight);
            move(geoX, newY);
        }
    }
    else
        resize(xMouse+1, yMouse+1);
}

void CustomWindow::setCentralWidget(QWidget *widget, const QString &widgetName)
{
    //We do connect in the old fashion way since it will be decided in execution time
    connect(widget, SIGNAL(cancelled()), this, SLOT(close()));

    ui->centralLayout->addWidget(widget);
	ui->LTitle->setText(widgetName);
}

void CustomWindow::setTitlebarMode(const TitleMode &flag)
{
    mTitleMode = flag;

    switch (mTitleMode)
    {
        case TitleMode::CleanTitle:
            ui->tbMenu->setHidden(true);
            ui->pbMin->setHidden(true);
            ui->pbMax->setHidden(true);
            ui->pbClose->setHidden(true);
            break;
        case TitleMode::OnlyCloseButton:
            ui->tbMenu->setHidden(true);
            ui->pbMin->setHidden(true);
            ui->pbMax->setHidden(true);
            break;
        case TitleMode::MenuOff:
            ui->tbMenu->setHidden(true);
            break;
        case TitleMode::MaxMinOff:
            ui->pbMin->setHidden(true);
            ui->pbMax->setHidden(true);
            break;
        case TitleMode::FullScreenMode:
            ui->pbMax->setHidden(true);
            showMaximized();
            break;
        case TitleMode::MaximizeModeOff:
            ui->pbMax->setHidden(true);
            break;
        case TitleMode::MinimizeModeOff:
            ui->pbMin->setHidden(true);
            break;
        case TitleMode::FullTitle:
            ui->tbMenu->setVisible(true);
            ui->pbMin->setVisible(true);
            ui->pbMax->setVisible(true);
            ui->pbClose->setVisible(true);
            break;
    }
    ui->LTitle->setVisible(true);
}

void CustomWindow::setTitlebarMenu(QMenu *menu, const QString &icon)
{
    ui->tbMenu->setMenu(menu);
    ui->tbMenu->setIcon(QIcon(icon));
}

void CustomWindow::maximizeBtnClicked()
{
    if (isFullScreen() or isMaximized())
    {
        ui->pbMax->setIcon(QIcon(":/ui/images/app_max.png"));
        setWindowState(windowState() & ~Qt::WindowFullScreen & ~Qt::WindowMaximized);
    }
    else
    {
        ui->pbMax->setIcon(QIcon(":/ui/images/app_rest.png"));
        setWindowState(windowState() | Qt::WindowFullScreen | Qt::WindowMaximized);
    }
}

void CustomWindow::minimizeBtnClicked()
{
    if (isMinimized())
        setWindowState(windowState() & ~Qt::WindowMinimized);
    else
        setWindowState(windowState() | Qt::WindowMinimized);
}
