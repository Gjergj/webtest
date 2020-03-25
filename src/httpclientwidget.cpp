#include "httpclientwidget.h"
#include "QToolButton"
HttpClientWidget::HttpClientWidget(Notepad *notepadEditor, QWidget *parent)
    : QWidget(parent),
      _notepadEditor(notepadEditor)
{
    tabs = new QTabWidget(this);
    connect(tabs, &QTabWidget::tabCloseRequested, this, &HttpClientWidget::closeTab);
    tabs->setTabsClosable(true);
    tabs->setUsesScrollButtons(true);
    tabs->addTab(new HttpClientWindowv2(_notepadEditor, this),"HttpClient");

    tb = new QToolButton();
    tb->setIcon(QIcon(":/img/tab-new-384.png"));
    connect(tb, &QToolButton::clicked, this, &HttpClientWidget::addWidget);
    tabs->setCornerWidget(tb, Qt::TopLeftCorner);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(tabs);
    setLayout(mainLayout);
}

void HttpClientWidget::closeTab(int tabIndex)
{
    //todo possible leak
    if(tabs->count() == 1){
        return;
    }
    tabs->removeTab(tabIndex);
}

void HttpClientWidget::addWidget()
{
    tabs->addTab(new HttpClientWindowv2(_notepadEditor, this),"HttpClient");
}
