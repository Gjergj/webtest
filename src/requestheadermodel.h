#ifndef REQUESTHEADERMODEL_H
#define REQUESTHEADERMODEL_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include "mtypes.h"
class RequestHeaderModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_INVOKABLE bool setData(int row, int column, const QVariant value);
    Q_INVOKABLE QVariant getData(int row, int column);

    RequestHeaderModel(RequestCtx *CurrentRequest);
    void removeHeader(int row);
    void removeHeaders(int row, int count);
    void insertHeader(std::string headerName, std::string headerValue);
    void insertHeaders(std::list<std::pair<std::string, std::string> > &headers);
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    enum roleNames
    {
        HeaderNameRole = Qt::UserRole+1,
        HeaderValueRole = Qt::UserRole+2,
        HeaderActiveRole = Qt::UserRole+3
    };
protected:
    virtual QHash<int, QByteArray> roleNames() const override;
private:
    RequestCtx *m_request;
    QHash<int, QByteArray> m_roleNames;
};

#endif // REQUESTHEADERMODEL_H
