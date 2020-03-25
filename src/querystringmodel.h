#ifndef QUERYSTRINGMODEL_H
#define QUERYSTRINGMODEL_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include "mtypes.h"

class QueryStringModel  : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_INVOKABLE bool setData(int row, int column, const QVariant value);
    Q_INVOKABLE QVariant getData(int row, int column);
    Q_INVOKABLE void insertKeyValue(QString key, QString value);
    Q_INVOKABLE void removeKeyValue(int row);
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Q_INVOKABLE QString generateQueryString();
    Q_INVOKABLE void generateQueryStringFromRaw(QString rawBody);
    Q_INVOKABLE void copyKeyValueToClipboard();

    QueryStringModel();

    void removeHeaders(int row, int count);

    void insertKeyValues(QList<QPair<QString, QString> > &keyVals);
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    enum roleNames
    {
        CheckedRole =  Qt::UserRole+1,
        KeyRole = Qt::UserRole+2,
        ValueRole = Qt::UserRole+3
    };
protected:
    virtual QHash<int, QByteArray> roleNames() const override;
private:
    QList<QPair<QString,QString>> keyValues;
    QHash<int, QByteArray> m_roleNames;
};

#endif // QUERYSTRINGMODEL_H
