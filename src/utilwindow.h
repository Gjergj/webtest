#ifndef UTILWINDOW_H
#define UTILWINDOW_H

#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QBoxLayout>
#include <QUrl>
#include <QList>
#include <QPair>
#include <QCryptographicHash>
#include <QRadioButton>
#include <QGroupBox>
class UtilWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UtilWindow(QWidget *parent = 0);

signals:

public slots:
    void toPercentEncoding();
    void fromPercentEncoding();
    void performHashes();
    void toBase64();
    void fromBase64();
private:
    QGroupBox *urlGroupBox;
    QVBoxLayout *urlLayout;
    QLineEdit *plainUrlEdit;
    QLineEdit *percentEncodedUrlEdit;
    QLabel *plainUrlLabel;
    QLabel *percentEncodedUrlLabel;

    QLabel *doNotEncodelLabel;
    QLineEdit *doNotEncodeEdit;
    QGroupBox *urlUtilsGroupBox;

    //hash
    QVBoxLayout *hashLayout;
    QLabel *textToBeHashedLabel;
    QLineEdit *textToBeHashedEdit;

    QLabel *Md4Label;
    QLineEdit *Md4Edit;

    QLabel *Md5Label;
    QLineEdit *Md5Edit;

    QLabel *Sha1Label;
    QLineEdit *Sha1Edit;

    QLabel *Sha224Label;
    QLineEdit *Sha224Edit;

    QLabel *Sha256Label;
    QLineEdit *Sha256Edit;

    QLabel *Sha384Label;
    QLineEdit *Sha384Edit;

    QLabel *Sha512Label;
    QLineEdit *Sha512Edit;

    QLabel *Sha3_224Label;
    QLineEdit *Sha3_224Edit;

    QLabel *Sha3_256Label;
    QLineEdit *Sha3_256Edit;

    QLabel *Sha3_384Label;
    QLineEdit *Sha3_384Edit;

    QLabel *Sha3_512Label;
    QLineEdit *Sha3_512Edit;

    QHBoxLayout *hashResultTypeLayout;
    QRadioButton *hashToBase64;
    QRadioButton *hashToHex;
    QGroupBox *hashUtilsGroupBox;


    QLabel *base64PlainLabel;
    QLineEdit *plainBase64Edit;
    QLabel *base64Label;
    QLineEdit *base64Edit;
    QGroupBox *base64GroupBox;
    QVBoxLayout *base64Layout;


    QHBoxLayout *mainLayout;
    QVBoxLayout *leftSideVerticalLayout;

};

#endif // UTILWINDOW_H
