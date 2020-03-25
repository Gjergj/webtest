#include "urlencodedbodymodel.h"
#include "QDebug"
#include <QApplication>
#include <QClipboard>
UrlencodedBodyModel::UrlencodedBodyModel()
{
    m_roleNames[CheckedRole]="checked";
    m_roleNames[KeyRole]="key";
    m_roleNames[ValueRole]="value";
}

bool UrlencodedBodyModel::setData(int row, int column, const QVariant value)
{
    int role = Qt::UserRole + 1 + column;
    return setData(index(row,0), value, role);
}

QVariant UrlencodedBodyModel::getData(int row, int column)
{
    int role = Qt::UserRole + 1 + column;
    return data(index(row,0), role);
}

void UrlencodedBodyModel::removeHeader(int row)
{
    removeRows(row,1);
}

void UrlencodedBodyModel::removeHeaders(int row, int count)
{
    removeRows(row,count);
}

void UrlencodedBodyModel::insertHeaders(QList<QPair<QString,QString>> &keyVals)
{
    keyValues.append(keyVals);//todo nuk therret begininsert , end insert
}

bool UrlencodedBodyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    size_t row = index.row();
    if(row<0 || row >= keyValues.size())
    {
        return false;
    }
    switch(role)
    {
    case KeyRole:{
        keyValues[row].first = value.toString();
        emit dataChanged(index, index);
        return true;
    }
    case ValueRole:{
        keyValues[row].second = value.toString();
        emit dataChanged(index, index);

        return true;
    }
    }
    return false;
}

int UrlencodedBodyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return keyValues.size();
}

QVariant UrlencodedBodyModel::data(const QModelIndex &index, int role) const
{
    size_t row = index.row();
    if(row<0 || row >= keyValues.size())
    {
        return QVariant();
    }
    QPair<QString,QString> keyValue = keyValues.at(row);
    switch(role)
    {
    case KeyRole:{
        return keyValue.first;
    }
    case ValueRole:
        return keyValue.second;

    }
    return QVariant();
}

bool UrlencodedBodyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row, row + count - 1);
    auto rowIt  = keyValues.begin();
    std::advance(rowIt, row);
    for(int i = 0; i < count; i++){
        rowIt = keyValues.erase(rowIt);
    }
    endRemoveRows();
    return true;
}

QString UrlencodedBodyModel::generateUrlEncodedBody()
{
    QString body;
    if(keyValues.length() != 0){
        for(auto it : keyValues){
            body += it.first + "=" + it.second;
            body += "&";
        }
        body.chop(1);//remove last &
    }
    return body;
}

void UrlencodedBodyModel::generateUrlEncodedBodyFromRaw(QString rawBody)
{
    QStringList keyVals = rawBody.split('&', QString::SkipEmptyParts);
    if(keyVals.length() != 0){
        removeHeaders(0,rowCount());
        foreach (const QString &str, keyVals) {
            QStringList keys = str.split('=', QString::SkipEmptyParts);
            if(keys.length() != 2){
                return;
            }else{
                insertKeyValue(keys.at(0),keys.at(1));
            }
        }
    }
}

void UrlencodedBodyModel::copyKeyValueToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(generateUrlEncodedBody());
}

Qt::ItemFlags UrlencodedBodyModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable;
    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    }
    else
    {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemNeverHasChildren;
    }
}

QHash<int, QByteArray> UrlencodedBodyModel::roleNames() const
{
    return m_roleNames;
}

void UrlencodedBodyModel::insertKeyValue(QString key, QString value)
{
    beginInsertRows(QModelIndex(),keyValues.size(), keyValues.size());
    keyValues.push_back(QPair<QString,QString>(key,value));
    endInsertRows();
}
