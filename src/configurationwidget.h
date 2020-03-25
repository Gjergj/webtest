#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QNetworkProxyQuery>
#include <QNetworkProxy>
#include <QTimer>
#include <QTableView>
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QItemDelegate>
#include "configurations.h"
class ConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationWidget(QWidget *parent = 0);

signals:

public slots:
private slots:
    void systemProxyRadioButtonToggled();
    void proxyHostChanged(QString proxyHost);
    void proxyPortChanged(QString proxyPort);
private:
    void getSystemProxy();
    QGroupBox *proxyGroupBox;
    QVBoxLayout *proxyLayout;
    QRadioButton *systemProxyRadioButton;
    QRadioButton *manualProxyRadioButton;
    QRadioButton *noProxyRadioButton;
    QLineEdit *proxyHostEdit;
    QLineEdit *proxyPortEdit;


    QVBoxLayout *mainLayout;
    QTimer *detectProxyChangeTimer;

    QString proxHost_;
    uint16_t proxyPort_;
};

#endif // CONFIGURATIONWIDGET_H
