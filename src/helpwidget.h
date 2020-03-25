#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
class HelpWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HelpWindow(QWidget *parent = 0);

signals:

public slots:
private:
    QVBoxLayout *mainLayout;
    QLabel *appVersionLabel;
    QLabel *appDeveloperLabel;
};

#endif // HELPWINDOW_H
