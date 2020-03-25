#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent)
{

    setStyleSheet(R"(
                  QWidget{ background-color: #333333}

                  QLineEdit {
                  font-size:16px;
                  height: 20px;
                  padding:5px 5px 1px 1px;
                  /*width:30px;*/
                  border:none;
                  background-color: #ffffff
                  /*border-bottom:1px solid #2196F3;*/
                  }

                  QPushButton { background-color: #333333;
                  border: none;
                  border-radius: 0px;
                  height: 20px;
                  line-height: 20px;
                  outline: 0;
                  padding: 0 2rem;
                  text-transform: uppercase;
                  vertical-align: middle;
                  font: bold 10px;
                  min-width: 5em;
                  color: #ffffff
                  }
                  QPushButton:pressed {
                  background-color: #2196F3;
                  border-style: inset;
                  }
                  QLabel {
                  color:#999;
                  font-size:10px;
                  font-weight:normal;
                  position:absolute;
                  left:5px;
                  top:10px;
                  }
                  QToolBar {/*
                  background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 lightgray, stop:1 darkgray);*/
                  background-color: #E3F2FD;
                  border: none;
                  border-radius: 2px;
                  height: 36px;
                  line-height: 36px;
                  outline: 0;
                  padding: 0 2rem;
                  text-transform: uppercase;
                  vertical-align: middle;
                  font: bold 14px;
                  min-width: 10em;
                  }/*
                  QToolBar::handle {
                  image: url(handle.png);
                  }*/
                  QToolButton { /* all types of tool button */
                  border: 0px solid #8f8f91;
                  border-radius: 0px;
                  font: bold 12px;
                  /*background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);*/
                  }
                  QToolButton:pressed {
                  background-color: #2196F3;
                  border-style: inset;
                  }
                  QToolButton[popupMode="1"] { /* only for MenuButtonPopup */
                  padding-right: 20px; /* make way for the popup button */
                  }
                  QToolButton:pressed {
                  /*background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);*/
                  }
                  QMenu {
                  background-color: #64B5F6; /* sets background of the menu */
                  border: 1px solid black;
                  }
                  QMenu::item {
                  /* sets background of menu item. set this to something non-transparent
                  if you want menu color and menu item color to be different */
                  background-color: transparent;
                  }

                  QMenu::item:selected { /* when user selects item using mouse or keyboard */
                  background-color: #2196F3;
                  }
                  QRadioButton::indicator {
                  width: 13px;
                  height: 13px;
                  color: #2196F3
                  }
                  QRadioButton::indicator::checked {

                  }
                  QSplitter::handle:horizontal {
                  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                  stop:0 #eee, stop:1 #ccc);
                  border: 1px solid #777;
                  width: 5px;
                  margin-top: 2px;
                  margin-bottom: 2px;
                  /*margin-left: 2px;
                  margin-right: 2px;*/
                  border-radius: 4px;
                  }
                  QSplitter::handle:vertical {
                  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                  stop:0 #eee, stop:1 #ccc);
                  border: 1px solid #777;
                  width: 5px;
                  /*margin-top: 2px;
                  margin-bottom: 2px;*/
                  margin-left: 2px;
                  margin-right: 2px;
                  border-radius: 4px;
                  })");
    findTextLabel = new QLabel("Find:");
    findTextEdit = new QLineEdit(this);
    connect(findTextEdit, &QLineEdit::textChanged,this,[this](QString searchText){
        onSearchTextChanged(searchText);
    });
    findTextEdit->setFocus();
    findButton = new QPushButton("Find Next");
    connect(findButton, &QPushButton::clicked, this,[this](){
        emit findText();
    } );
    connect(findTextEdit, &QLineEdit::returnPressed, this, [this](){
        emit findText();
    });
    findPreviousButton = new QPushButton("Find Previous");
    connect(findPreviousButton, &QPushButton::clicked, this,[this](){
        emit findPreviousText();
    } );

    findLayout = new QHBoxLayout();
    findLayout->addWidget(findTextLabel);
    findLayout->addWidget(findTextEdit);
    findLayout->addWidget(findButton);
    findLayout->addWidget(findPreviousButton);
    findLayout->addSpacerItem(new QSpacerItem(3,1));

    replaceTextLabel = new QLabel("Replace with: ");
    replaceTextEdit = new QLineEdit(this);
    replaceButton = new QPushButton("Replace");
    connect(replaceButton, &QPushButton::clicked, this,[this](){
        emit replaceText();
    } );
    replaceAndFindButton = new QPushButton("Replace & Find");
    replaceAllButton = new QPushButton("Replace All");
    connect(replaceAllButton, &QPushButton::clicked, this,[this](){
        emit replaceAllText();
    } );
    replaceLayout = new QHBoxLayout();
    replaceLayout->addWidget(replaceTextLabel);
    replaceLayout->addWidget(replaceTextEdit);
    replaceLayout->addWidget(replaceButton);
    //replaceLayout->addWidget(replaceAndFindButton);//todo implement replace find
    replaceLayout->addWidget(replaceAllButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(2);
    mainLayout->setMargin(0);
    mainLayout->addLayout(findLayout);
    mainLayout->addLayout(replaceLayout);
    setLayout(mainLayout);
}

void SearchWidget::clearFindTextEdit()
{
    findTextEdit->clear();
}

QString SearchWidget::getFindText()
{
    return findTextEdit->text();
}

QString SearchWidget::getReplaceText()
{
    return replaceTextEdit->text();
}

void SearchWidget::setFindText(QString findText)
{
    findTextEdit->setText(findText);
}
void SearchWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
//void SearchWidget::keyPressEvent(QKeyEvent* f_event)
//{
//    qDebug() << " SearchWidget Key_Enter";
//    if(f_event->key() == Qt::Key_Enter)
//    {
//        qDebug() << " SearchWidget Key_Enter";
//        f_event->accept();
//       emit findText();
//    }else{
//        f_event->accept();
//        SearchWidget::keyPressEvent(f_event);

//    }
//}

void SearchWidget::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    findTextEdit->setFocus();
}
bool SearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        SearchWidget::keyPressEvent(keyEvent);
        return true;
    }else{
        return QWidget::eventFilter(obj, event);
    }
}
