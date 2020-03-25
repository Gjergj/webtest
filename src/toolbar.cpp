#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent) : QToolBar(parent)
{

}

ToolBar::ToolBar(const QString &title, QWidget *parent) : QToolBar(title, parent)
{

}
void ToolBar::mousePressEvent(QMouseEvent *event){
    emit toolBarPressed(event);
}

void ToolBar::mouseMoveEvent(QMouseEvent *event){
    if ( (event->buttons() & Qt::LeftButton) && dragEnabled) {
        emit toolBarMoved(event);
    }
}
void ToolBar::mouseDoubleClickEvent(QMouseEvent *event){
    if ( (event->buttons() & Qt::LeftButton) && dragEnabled){
        emit toolBarDoubleClicked();
    }
}
bool ToolBar::eventFilter(QObject* obj, QEvent* e)
{
    if ( (e->type() == QEvent::Enter) && obj->objectName().contains("action") )
    {
        dragEnabled = false;
        // a widget in the toolbar was entered
        // you may cast obj to a widget and do sth with it here
        return true;
    }
    else if ((e->type() == QEvent::Leave) && obj->objectName().contains("action"))
    {
        dragEnabled = true;
        // a widget in the toolbar was left
        return true;
    }
    return QWidget::eventFilter(obj, e);
}
