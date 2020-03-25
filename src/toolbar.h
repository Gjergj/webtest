#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QMouseEvent>
#include <QDebug>
#include <QMainWindow>
class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = 0);
    ToolBar(const QString &title, QWidget *parent = 0);

signals:
    void toolBarPressed(QMouseEvent *event);
    void toolBarMoved(QMouseEvent *event);
    void toolBarDoubleClicked();
public slots:
private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    bool eventFilter(QObject* obj, QEvent* e);
    QPoint mpos;
    bool dragEnabled = true;
};

#endif // TOOLBAR_H
