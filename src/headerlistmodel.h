#ifndef HEADERLISTMODEL_H
#define HEADERLISTMODEL_H

#include <QObject>
#include <QObject>
#include <QString>
#include <QAbstractListModel>
class HeaderListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    HeaderListModel();

    Q_INVOKABLE bool setData(int row, int column, const QVariant value);
    Q_INVOKABLE QVariant data(int row, int column);
    Q_INVOKABLE bool insertHeader(int row, int column, const QVariant value);

    void removeHeader(int row);
    void removeHeaders(int row, int count);
    void insertHeader(std::string headerName);
    void insertHeaders(std::list<std::string> &headers);
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    enum roleNames
    {
        HeaderListNameRole = Qt::UserRole+1,
    };
    template<typename T>
    bool contains(const T & element)
    {
        // Find the iterator if element in list
        auto it = std::find(m_headerList.begin(), m_headerList.end(), element);
        //return if iterator points to end or not. It points to end then it means element
        // does not exists in list
        return it != m_headerList.end();
    }
protected:
    virtual QHash<int, QByteArray> roleNames() const override;
private:
    std::list<std::string> m_headerList;
    QHash<int, QByteArray> m_roleNames;
};

#endif // HEADERLISTMODEL_H
