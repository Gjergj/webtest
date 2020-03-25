#include "requestheadermodel.h"
#include "QDebug"
RequestHeaderModel::RequestHeaderModel(RequestCtx *CurrentRequest)
{
    m_request = CurrentRequest;
    m_roleNames[HeaderNameRole]="headerName";
    m_roleNames[HeaderValueRole]="headerValue";
    m_roleNames[HeaderActiveRole]="headerActive";
}

bool RequestHeaderModel::setData(int row, int column, const QVariant value)
{
    int role = 0;
    //    if(column == 0){//strange hack
    //        role = Qt::UserRole + column;
    //    }else{
    role = Qt::UserRole + 1 + column;
    //    }
    return setData(index(row,0), value, role);
}

QVariant RequestHeaderModel::getData(int row, int column)
{
    int role = Qt::UserRole + 1 + column;
    return data(index(row,0), role);
}

void RequestHeaderModel::removeHeader(int row)
{
    removeRows(row,1);
}

void RequestHeaderModel::removeHeaders(int row, int count)
{
    removeRows(row,count);
}

void RequestHeaderModel::insertHeaders(std::list<std::pair<std::string, std::string>> &headers)
{
    m_request->headers.insert(m_request->headers.end(),headers.begin(),headers.end());//todo nuk therret begininsert , end insert
}

bool RequestHeaderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    size_t row = index.row();
    if(row<0 || row >= m_request->headers.size())
    {
        return false;
    }
    switch(role)
    {
    case HeaderNameRole:{
        auto it = m_request->headers.begin();
        std::advance(it, row);
        it->first = value.toString().toStdString();
        emit dataChanged(index, index);
        return true;
    }
    case HeaderValueRole:{
        auto it = m_request->headers.begin();
        std::advance(it, row);
        if(it !=  m_request->headers.end() ){
            it->second = value.toString().toStdString();
            emit dataChanged(index, index);
        }
        return true;
    }
    case HeaderActiveRole:
        return true;
    }
    return false;
}

int RequestHeaderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_request->headers.size();
}

QVariant RequestHeaderModel::data(const QModelIndex &index, int role) const
{
    size_t row = index.row();
    if(row<0 || row >= m_request->headers.size())
    {
        return QVariant();
    }

    auto it = m_request->headers.begin();
    std::advance(it, row);
    switch(role)
    {
    case HeaderNameRole:{
        return QString::fromStdString((*it).first);
    }
    case HeaderValueRole:
        return QString::fromStdString((*it).second);
    case HeaderActiveRole:
        return true;
    }
    return QVariant();
}

bool RequestHeaderModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(),row, row + count - 1);
    auto rowIt  = m_request->headers.begin();
    std::advance(rowIt, row);
    for(int i = 0; i < count; i++){
        rowIt = m_request->headers.erase(rowIt);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags RequestHeaderModel::flags(const QModelIndex &index) const
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

QHash<int, QByteArray> RequestHeaderModel::roleNames() const
{
    return m_roleNames;
}

void RequestHeaderModel::insertHeader(std::string headerName, std::string headerValue)
{
    beginInsertRows(QModelIndex(),m_request->headers.size(), m_request->headers.size());
    m_request->headers.push_back(std::make_pair(headerName,headerValue));
    endInsertRows();
}
