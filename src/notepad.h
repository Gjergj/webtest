#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include "texteditor.h"
class Notepad : public QWidget
{
    Q_OBJECT
public:
    explicit Notepad(QWidget *parent = 0);
    void appendTextToEditor(QString text);
signals:

public slots:

private slots:
    void selectionSizeChanged();
    void columnNoChanged();
    void jsonResponeBodyBeautify(bool beautify);
private:
    TextEditor *editor;
    QVBoxLayout *mainLayout;
    QLabel *selectionLabel;
    QLabel *selectionText;
    QLabel *lineLabel;
    QLabel *lineText;
    QLabel *columnLabel;
    QLabel *columnText;
    QCheckBox *jsonBeautifyCheckbox;
    QHBoxLayout *statusLayout;
};

#endif // NOTEPAD_H
