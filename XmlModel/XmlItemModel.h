#ifndef XMLITEMMODEL_H
#define XMLITEMMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QStandardItemModel>

class XmlItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit XmlItemModel(QAbstractItemModel *parent = nullptr);
    virtual ~XmlItemModel();

    void SetDomDocument(QDomElement &rootElement);

    // 构造父节点下子节点的索引
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    // 通过子节点索引获取父节点索引
    virtual QModelIndex parent(const QModelIndex &child) const override;
    // 获取父节点下子节点的行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // 获取父节点下子节点列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    // 获取节点数据：包括DisplayRole|TextAlignmentRole等
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // 设置节点数据
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
signals:

private:
    QDomElement m_rootElement;
};

#endif // XMLITEMMODEL_H
