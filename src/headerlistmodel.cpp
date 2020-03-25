#include "headerlistmodel.h"
#include "QDebug"

HeaderListModel::HeaderListModel()
{
    m_roleNames[HeaderListNameRole]="headerListNameRole";
    m_headerList.push_back("");
    m_headerList.push_back("Accept");
    m_headerList.push_back("Accept-Charset");
    m_headerList.push_back("Accept-Encoding");
    m_headerList.push_back("Accept-Language");
    m_headerList.push_back("Accept-Datetime");
    m_headerList.push_back("Authorization");
    m_headerList.push_back("Cache-Control");
    m_headerList.push_back("Connection");
    m_headerList.push_back("Cookie");
    m_headerList.push_back("Content-Length");
    m_headerList.push_back("Content-MD5");
    m_headerList.push_back("Content-Type");
    m_headerList.push_back("Date");
    m_headerList.push_back("Expect");
    m_headerList.push_back("Forwarded");
    m_headerList.push_back("From");
    m_headerList.push_back("Host");
    m_headerList.push_back("If-Match");
    m_headerList.push_back("If-Modified-Since");
    m_headerList.push_back("If-None-Match");
    m_headerList.push_back("If-Range");
    m_headerList.push_back("If-Unmodified-Since");
    m_headerList.push_back("Max-Forwards");
    m_headerList.push_back("Origin");
    m_headerList.push_back("Pragma");
    m_headerList.push_back("Proxy-Authorization");
    m_headerList.push_back("Range");
    m_headerList.push_back("Referer");
    m_headerList.push_back("TE");
    m_headerList.push_back("User-Agent");
    m_headerList.push_back("Upgrade");
    m_headerList.push_back("Via");
    m_headerList.push_back("Warning");
    m_headerList.push_back("X-Requested-With");
    m_headerList.push_back("DNT");
    m_headerList.push_back("X-Forwarded-For");
    m_headerList.push_back("X-Forwarded-Host");
    m_headerList.push_back("X-Forwarded-Proto");
    m_headerList.push_back("Front-End-Https");
    m_headerList.push_back("X-Http-Method-Override");
    m_headerList.push_back("X-ATT-DeviceId");
    m_headerList.push_back("X-Wap-Profile");
    m_headerList.push_back("Proxy-Connection");
    m_headerList.push_back("X-UIDH");
    m_headerList.push_back("X-Csrf-Token");
    m_headerList.push_back("X-Request-ID");
    m_headerList.push_back("X-Correlation-ID");
}
bool HeaderListModel::setData(int row, int column, const QVariant value)
{
    int role = Qt::UserRole + 1 + column;
    return setData(index(row,0), value, role);
}

QVariant HeaderListModel::data(int row, int column)
{
    int role = Qt::UserRole + 1 + column;
    return data(index(row,0), role);
}

bool HeaderListModel::insertHeader(int row, int column, const QVariant value)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    insertHeader(value.toString().toStdString());
    return true;
}

void HeaderListModel::removeHeader(int row)
{
    removeRows(row,1);
}

void HeaderListModel::removeHeaders(int row, int count)
{
    removeRows(row,count);
}

void HeaderListModel::insertHeaders(std::list<std::string> &headers)
{
    m_headerList.insert(m_headerList.end(),headers.begin(),headers.end());//todo nuk therret begininsert , end insert
}

bool HeaderListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    size_t row = index.row();
    if(row<0 || row >= m_headerList.size())
    {
        return false;
    }
    switch(role)
    {
    case HeaderListNameRole:{
        auto it = m_headerList.begin();
        std::advance(it, row);
        *it = value.toString().toStdString();
        emit dataChanged(index, index);
        return true;
    }
    }
    return false;
}

int HeaderListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headerList.size();
}

QVariant HeaderListModel::data(const QModelIndex &index, int role) const
{
    size_t row = index.row();
    if(row<0 || row >= m_headerList.size())
    {
        return QVariant();
    }

    auto it = m_headerList.begin();
    std::advance(it, row);
    switch(role)
    {
    case HeaderListNameRole:{
        return QString::fromStdString((*it));

    }
    }
    return QVariant();
}

bool HeaderListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row, row + count - 1);
    auto rowIt  = m_headerList.begin();
    std::advance(rowIt, row);
    for(int i = 0; i < count; i++){
        rowIt = m_headerList.erase(rowIt);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags HeaderListModel::flags(const QModelIndex &index) const
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

QHash<int, QByteArray> HeaderListModel::roleNames() const
{
    return m_roleNames;
}

void HeaderListModel::insertHeader(std::string headerName)
{
    beginInsertRows(QModelIndex(),m_headerList.size(), m_headerList.size());
    m_headerList.push_back(headerName);
    endInsertRows();
}
