#include "XmlItemModel.h"
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

XmlItemModel::XmlItemModel(QAbstractItemModel *parent) : QAbstractItemModel(parent)
{

}

XmlItemModel::~XmlItemModel()
{

}

void XmlItemModel::SetDomDocument(QDomElement &rootElement)
{
    m_rootElement = rootElement;
    layoutChanged();
}

QModelIndex XmlItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.row() == -1 && parent.column() == -1)
    {
        // 首层节点绑定关系
        if ((!m_rootElement.isNull())
                &&(row < m_rootElement.childNodes().size()))
        {
            QDomElement node = m_rootElement.childNodes().at(row).toElement();

            cout << QString("createIndex(%1,%2,%3,point = %4)")
                    .arg(row).arg(column).arg(node.tagName() + ":" + node.attribute("name"))
                    .arg((unsigned int)&node);
            return createIndex(row, column, &node);
        }
    }
    else
    {
        // 其它层节点绑定关系
        if (parent.internalPointer() != nullptr)
        {
            QDomElement* pNode = reinterpret_cast<QDomElement *>(parent.internalPointer());
            if (pNode->childNodes().size() > row)
            {
                QDomElement node = pNode->childNodes().at(row).toElement();
                cout << QString("createIndex(%1,%2,%3,point = %4)")
                        .arg(row).arg(column).arg(node.tagName() + ":" + node.attribute("name"))
                        .arg((unsigned int)&node);
                return createIndex(row, column, &node);
            }
        }
    }
    return QModelIndex();
}

QModelIndex XmlItemModel::parent(const QModelIndex &child) const
{
    cout << QString("Index(%1,%2,point = %3) called to find parent")
            .arg(child.row()).arg(child.column()).arg(QString::number((unsigned int)child.internalPointer()));
    if (child.internalPointer() != nullptr)
    {
        QDomElement* pNode = reinterpret_cast<QDomElement*>(child.internalPointer());
        qDebug() << child.row() << child.parent().row();
        qDebug() << pNode;
        if (!pNode->parentNode().isNull())
        {
            QDomElement parentElement = pNode->parentNode().toElement();
            if((parentElement.parentNode().isNull())
                    ||(!parentElement.parentNode().isElement()))
            {
                if(m_rootElement.isNull())
                {
                    cout << QString("Index(%1,%2,%3) -> parentIndex(null)")
                            .arg(child.row()).arg(child.column()).arg(pNode->tagName() + ":" + pNode->attribute("name"));
                    return QModelIndex();
                }

                for(int i = 0; i < m_rootElement.childNodes().size(); i++)
                {
                    if(parentElement == m_rootElement.childNodes().at(i).toElement())
                    {
                        cout << QString("Index(%1,%2,%3) -> parentIndex(%4,%5,%6)")
                                .arg(child.row()).arg(child.column()).arg(pNode->tagName() + ":" + pNode->attribute("name"))
                                .arg(i).arg(0).arg(parentElement.tagName() + ":" + parentElement.attribute("name"));
                        return createIndex(i, 0, &parentElement);
                    }
                }
            }
            else
            {
                QDomElement element = parentElement.parentNode().toElement();
                for(int i = 0; i < element.childNodes().size(); i++)
                {
                    if(parentElement == element.childNodes().at(i).toElement())
                    {
                        cout << QString("Index(%1,%2,%3) -> parentIndex(%4,%5,%6)")
                                .arg(child.row()).arg(child.column()).arg(pNode->tagName() + ":" + pNode->attribute("name"))
                                .arg(i).arg(0).arg(parentElement.tagName() + ":" + parentElement.attribute("name"));
                        return createIndex(i, 0, &parentElement);
                    }
                }
            }
        }
    }
    cout << QString("Index(child.internalPointer() is nullptr) -> parentIndex(null)");
    return QModelIndex();
}

int XmlItemModel::rowCount(const QModelIndex &parent) const
{
    if(m_rootElement.isNull())
        return 0;

    cout << parent.row() << parent.data() << QString::number((unsigned int)parent.internalPointer());
    if((!parent.isValid())
            ||(parent.internalPointer() == nullptr))
    {
        return m_rootElement.childNodes().size();
    }
    else
    {
        QDomElement* pNode = reinterpret_cast<QDomElement*>(parent.internalPointer());
        cout << pNode->tagName();
        return pNode->childNodes().size();
    }
}

int XmlItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant XmlItemModel::data(const QModelIndex &index, int role) const
{
    // 节点内容：左对齐，显示行列号
    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        if((!index.isValid())
                ||(index.internalPointer() == nullptr))
        {
            return QString("row=%1,col=%2 invalid").arg(index.row()).arg(index.column());
        }
        else
        {
            QDomElement* pNode = reinterpret_cast<QDomElement*>(index.internalPointer());

            return pNode->tagName() + ":" + pNode->attribute("name");
        }
    }
    else
    {
        return QVariant();
    }
}

bool XmlItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(QAbstractItemModel::setData(index,value,role) == false)
        return false;

    return true;
}
