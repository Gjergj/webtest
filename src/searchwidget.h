#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = 0);
    void clearFindTextEdit();
    QString getFindText();
    QString getReplaceText();
    void setFindText(QString findText);
signals:
    void findText();
    void findPreviousText();
    void onSearchTextChanged(QString searchText);
    void replaceText();
    void replaceAllText();
public slots:

private:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *);
    void focusInEvent(QFocusEvent* e);
    //void keyPressEvent(QKeyEvent* f_event);
    QVBoxLayout *mainLayout;
    QHBoxLayout *findLayout;
    QLineEdit *findTextEdit;
    QLabel *findTextLabel;
    QPushButton *findButton;
    QPushButton *findPreviousButton;

    QHBoxLayout *replaceLayout;
    QLabel *replaceTextLabel;
    QPushButton *replaceButton;
    QPushButton *replaceAndFindButton;
    QPushButton *replaceAllButton;
    QLineEdit *replaceTextEdit;

};

#endif // SEARCHWIDGET_H
