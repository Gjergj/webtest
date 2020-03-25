#include "utilwindow.h"

UtilWindow::UtilWindow(QWidget *parent) : QWidget(parent)
{
    percentEncodedUrlLabel = new QLabel("To/From Percent Encoding",this);
    plainUrlLabel = new QLabel("Plain Url",this);
    plainUrlEdit = new QLineEdit(this);
    percentEncodedUrlEdit = new QLineEdit(this);
    urlLayout = new QVBoxLayout();
    doNotEncodelLabel = new QLabel("Characters to not encode");
    doNotEncodeEdit = new QLineEdit(":/?=");


    urlLayout->addWidget(plainUrlLabel);
    urlLayout->addWidget(plainUrlEdit);

    urlLayout->addWidget(doNotEncodelLabel);
    urlLayout->addWidget(doNotEncodeEdit);

    urlLayout->addWidget(percentEncodedUrlLabel);
    urlLayout->addWidget(percentEncodedUrlEdit);



    textToBeHashedLabel = new QLabel("Plain Text",this);
    textToBeHashedEdit = new QLineEdit(this);

    Md4Label = new QLabel("Md4 Hash",this);
    Md4Edit = new QLineEdit(this);

    Md5Label = new QLabel("Md5 Hash",this);
    Md5Edit  = new QLineEdit(this);

    Sha1Label = new QLabel("SHA1 Hash",this);
    Sha1Edit = new QLineEdit(this);

    Sha224Label = new QLabel("Sha22 Hash",this);
    Sha224Edit = new QLineEdit(this);

    Sha256Label = new QLabel("Sha256 Hash",this);
    Sha256Edit = new QLineEdit(this);

    Sha384Label = new QLabel("Sha384 Hash",this);
    Sha384Edit = new QLineEdit(this);

    Sha512Label = new QLabel("Sha512 Hash",this);
    Sha512Edit = new QLineEdit(this);

    Sha3_224Label = new QLabel("Sha3_224 Hash",this);
    Sha3_224Edit = new QLineEdit(this);

    Sha3_256Label = new QLabel("Sha3_256 Hash",this);
    Sha3_256Edit = new QLineEdit(this);

    Sha3_384Label = new QLabel("Sha3_384 Hash",this);
    Sha3_384Edit = new QLineEdit(this);

    Sha3_512Label = new QLabel("Sha3_512 Hash",this);
    Sha3_512Edit = new QLineEdit(this);

    hashToBase64 = new QRadioButton("To Base 64",this);
    hashToBase64->setChecked(true);
    hashToHex = new QRadioButton("To hex",this);
    hashResultTypeLayout = new QHBoxLayout();
    hashResultTypeLayout->addWidget(hashToBase64);
    hashResultTypeLayout->addWidget(hashToHex,5);

    leftSideVerticalLayout = new QVBoxLayout();


    hashLayout = new QVBoxLayout();
    hashLayout->addLayout(hashResultTypeLayout);
    hashLayout->addWidget(textToBeHashedLabel);
    hashLayout->addWidget(textToBeHashedEdit);
    hashLayout->addWidget(Md4Label);
    hashLayout->addWidget(Md4Edit);
    hashLayout->addWidget(Md5Label);
    hashLayout->addWidget(Md5Edit);
    hashLayout->addWidget(Sha1Label);
    hashLayout->addWidget(Sha1Edit);
    hashLayout->addWidget(Sha224Label);
    hashLayout->addWidget(Sha224Edit);
    hashLayout->addWidget(Sha256Label);
    hashLayout->addWidget(Sha256Edit);
    hashLayout->addWidget(Sha384Label);
    hashLayout->addWidget(Sha384Edit);
    hashLayout->addWidget(Sha512Label);
    hashLayout->addWidget(Sha512Edit);
    hashLayout->addWidget(Sha3_224Label);
    hashLayout->addWidget(Sha3_224Edit);
    hashLayout->addWidget(Sha3_256Label);
    hashLayout->addWidget(Sha3_256Edit);
    hashLayout->addWidget(Sha3_384Label);
    hashLayout->addWidget(Sha3_384Edit);
    hashLayout->addWidget(Sha3_512Label);
    hashLayout->addWidget(Sha3_512Edit);
    hashLayout->addStretch(5);

    base64PlainLabel = new QLabel("Plain Text",this);
    plainBase64Edit = new QLineEdit(this);
    base64Label = new QLabel("Base64",this);
    base64Edit = new QLineEdit(this);
    base64GroupBox = new QGroupBox("To/From base64");
    base64Layout = new QVBoxLayout();
    base64Layout->addWidget(base64PlainLabel);
    base64Layout->addWidget(plainBase64Edit);
    base64Layout->addWidget(base64Label);
    base64Layout->addWidget(base64Edit);

    base64GroupBox->setLayout(base64Layout);

    urlUtilsGroupBox = new QGroupBox(tr("Convert URL to percent encoding and vice versa."));
    urlUtilsGroupBox->setLayout(urlLayout);
    leftSideVerticalLayout->addWidget(urlUtilsGroupBox);
    leftSideVerticalLayout->addWidget(base64GroupBox);
    leftSideVerticalLayout->addStretch(1);

    hashUtilsGroupBox = new QGroupBox(tr("Hash Utilties"));
    hashUtilsGroupBox->setLayout(hashLayout);

    mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftSideVerticalLayout);
    mainLayout->addWidget(hashUtilsGroupBox,1);

    setLayout(mainLayout);

    connect(plainUrlEdit, &QLineEdit::textEdited,this,&UtilWindow::toPercentEncoding);
    connect(percentEncodedUrlEdit, &QLineEdit::textEdited,this,&UtilWindow::fromPercentEncoding);

    connect(textToBeHashedEdit, &QLineEdit::textEdited,this,&UtilWindow::performHashes);
    connect(hashToBase64, &QRadioButton::clicked,this,&UtilWindow::performHashes);
    connect(hashToHex, &QRadioButton::clicked,this,&UtilWindow::performHashes);

    connect(plainBase64Edit, &QLineEdit::textEdited,this,&UtilWindow::toBase64);
    connect(base64Edit, &QLineEdit::textEdited,this,&UtilWindow::fromBase64);
}

void UtilWindow::toPercentEncoding(){
    percentEncodedUrlEdit->setText(QUrl::toPercentEncoding(plainUrlEdit->text(),QByteArray::fromStdString(doNotEncodeEdit->text().toStdString())));
}

void UtilWindow::fromPercentEncoding(){
    plainUrlEdit->setText(QUrl::fromPercentEncoding(QByteArray::fromStdString(percentEncodedUrlEdit->text().toStdString())));
}
void UtilWindow::toBase64(){
    QByteArray ba;
    ba.append(plainBase64Edit->text());
    base64Edit->setText(ba.toBase64());
}
void UtilWindow::fromBase64(){
    QByteArray ba;
     ba.append(base64Edit->text());
     plainBase64Edit->setText(QByteArray::fromBase64(ba));}

void UtilWindow::performHashes(){
    if(hashToBase64->isChecked()){
        QByteArray hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Md4);
        Md4Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Md5);
        Md5Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha1);
        Sha1Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha224);
        Sha224Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha256);
        Sha256Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha384);
        Sha384Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha512);
        Sha512Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_224);
        Sha3_224Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_256);
        Sha3_256Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_384);
        Sha3_384Edit->setText(hash.toBase64());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_512);
        Sha3_512Edit->setText(hash.toBase64());
    }
    if(hashToHex->isChecked()){
        QByteArray hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Md4);
        Md4Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Md5);
        Md5Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha1);
        Sha1Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha224);
        Sha224Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha256);
        Sha256Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha384);
        Sha384Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha512);
        Sha512Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_224);
        Sha3_224Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_256);
        Sha3_256Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_384);
        Sha3_384Edit->setText(hash.toHex());
        hash = QCryptographicHash::hash(QByteArray::fromStdString(textToBeHashedEdit->text().toStdString()), QCryptographicHash::Sha3_512);
        Sha3_512Edit->setText(hash.toHex());
    }
}
