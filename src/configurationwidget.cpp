#include "configurationwidget.h"

ConfigurationWidget::ConfigurationWidget(QWidget *parent) : QWidget(parent)
{
    proxyGroupBox = new QGroupBox("Proxy Settings",this);
    proxyLayout = new QVBoxLayout();
    noProxyRadioButton = new QRadioButton("No Proxy",this);
    connect(noProxyRadioButton, &QRadioButton::toggled, this, &ConfigurationWidget::systemProxyRadioButtonToggled);
    systemProxyRadioButton = new QRadioButton("Use System Settings(Fiddler)",this);
    connect(systemProxyRadioButton, &QRadioButton::toggled, this, &ConfigurationWidget::systemProxyRadioButtonToggled);

    manualProxyRadioButton = new QRadioButton("Manual Proxy Settings",this);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    // You may want to use QRegularExpression for new code with Qt 5 (not mandatory).
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    proxyHostEdit = new QLineEdit(this);
    proxyHostEdit->setValidator(ipValidator);;
    proxyHostEdit->setPlaceholderText("Proxy Host/IP");
    connect(proxyHostEdit, &QLineEdit::textChanged, this, &ConfigurationWidget::proxyHostChanged);
    proxyPortEdit = new QLineEdit(this);
    proxyPortEdit->setValidator( new QIntValidator(0, 65535, this) );
    proxyPortEdit->setPlaceholderText("Proxy Port");

    proxyLayout->addWidget(noProxyRadioButton);
    proxyLayout->addWidget(systemProxyRadioButton);
    proxyLayout->addWidget(manualProxyRadioButton);
    proxyLayout->addWidget(proxyHostEdit);
    proxyLayout->addWidget(proxyPortEdit);
    proxyLayout->addStretch(1);
    proxyGroupBox->setLayout(proxyLayout);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(proxyGroupBox);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    detectProxyChangeTimer = new QTimer(this);
    connect(detectProxyChangeTimer, &QTimer::timeout, this, &ConfigurationWidget::systemProxyRadioButtonToggled);
    int HttpClientProxySettings = Configurations::get().getProxySetting();
    if(HttpClientProxySettings == 0){
        noProxyRadioButton->setChecked(true);
    }else if(HttpClientProxySettings == 1){
        systemProxyRadioButton->setChecked(true);//emits toggle
    }else if(HttpClientProxySettings == 1){
        manualProxyRadioButton->setChecked(true);
    }else{
        systemProxyRadioButton->setChecked(true);//emits toggle
    }

}

void ConfigurationWidget::systemProxyRadioButtonToggled()
{
    detectProxyChangeTimer->stop();
    if(noProxyRadioButton->isChecked()){
        proxyHostChanged("");
        proxyPortChanged("");
        proxyHostEdit->setEnabled(false);
        proxyPortEdit->setEnabled(false);
        Configurations::get().setProxySetting(0);
    }else if(systemProxyRadioButton->isChecked()){
        proxyHostEdit->setEnabled(false);
        proxyPortEdit->setEnabled(false);
        getSystemProxy();
        Configurations::get().setProxySetting(1);
    }else if(manualProxyRadioButton->isChecked()){
        proxyHostEdit->setEnabled(true);
        proxyPortEdit->setEnabled(true);
        proxyHostChanged(proxyHostEdit->text());
        proxyPortChanged(proxyPortEdit->text());
        Configurations::get().setProxySetting(2);
    }
    detectProxyChangeTimer->start(2000);
}

void ConfigurationWidget::proxyHostChanged(QString proxyHost)
{
    Configurations::get().setProxyHost(proxyHost.toStdString());
}

void ConfigurationWidget::proxyPortChanged(QString proxyPort)
{
    Configurations::get().setProxyPort(proxyPort.toStdString());
}

void ConfigurationWidget::getSystemProxy()
{
    bool proxyDetected = false;
    QNetworkProxyQuery npq(QUrl("http://www.google.com"));
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    for(auto proxy : listOfProxies){
        Configurations::get().setProxyHost(proxy.hostName().toStdString());
        Configurations::get().setProxyPort(std::to_string(proxy.port()));
        proxyDetected = true;
    }
    if(proxyDetected)
        QNetworkProxyFactory::setUseSystemConfiguration(true);//if application uses qnetworkaccessmanager
    else
        QNetworkProxyFactory::setUseSystemConfiguration(false);//if application uses qnetworkaccessmanager
}
