#include "helpwidget.h"

HelpWindow::HelpWindow(QWidget *parent) : QWidget(parent)
{
    appVersionLabel = new QLabel("webtest v.0.29",this);
    appDeveloperLabel = new QLabel("Zhvilluar nga Gjergj Ramku",this);
    mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(appVersionLabel);
    mainLayout->addWidget(appDeveloperLabel);
    mainLayout->addStretch(5);
    setLayout(mainLayout);
}
