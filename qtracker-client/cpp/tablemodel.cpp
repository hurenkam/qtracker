#include <QSqlError>
#include <QSqlQuery>
#include "tablemodel.h"

//#define ENABLE_DEBUG
#include "helpers.h"

TableModel::TableModel(QObject *parent)
    : QAbstractListModel(parent)
    , model(0)
    , _database("")
    , _table("")
    , _filter("")
{
    ENTER("")
}

TableModel::~TableModel()
{
    ENTER("")
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    ENTER("")
    if (!model) return 0;
    return model->rowCount(parent);
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    ENTER("")
    if (!model) return 0;
    return model->columnCount(parent);
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    ENTER(index << role)
    QModelIndex myindex();

    if (role > Qt::UserRole)
        return model->data(this->index(index.row(),role - Qt::UserRole -1),Qt::DisplayRole);
    else
        return model->data(index,role);
}

QString TableModel::database() const
{
    ENTER("")
    return _database;
}

void TableModel::setDatabase(const QString &database)
{
    ENTER(database)

    _database = database;

    if (model) delete model;
    model = 0;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database);
    bool result = db.open();
    if (!result) { EXIT("unable to open db" << db.lastError().text()) return; }

    if (!_table.isEmpty() && !_table.isNull())
        setTable(_table);

    emit databaseChanged();

    EXIT("")
}

QString TableModel::table() const
{
    ENTER("")
    return _table;
}

void TableModel::setTable(const QString &table)
{
    ENTER(table)
    _table = table;

    if (_database.isEmpty() || _database.isNull()) return;

    if (model) delete model;
    model = new QSqlTableModel(0,db);
    model->setTable(table);

    if (!_filter.isEmpty() && !_filter.isNull())
        setFilter(_filter);
    else
        model->select();


    QHash<int, QByteArray> roles;
    for (int i=0; i< columnCount(QModelIndex()); ++i)
    {
        roles[Qt::UserRole+i+1] = model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString().toLatin1();
    }
    setRoleNames(roles);


    connect(model, SIGNAL(rowsInserted(const QModelIndex&,int,int))
            , this, SLOT(inserted(const QModelIndex&,int,int)));
    connect(model, SIGNAL(rowsRemoved(const QModelIndex&,int,int))
            , this, SLOT(removed(const QModelIndex&,int,int)));
    connect(model, SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&))
            , this, SLOT(handleDataChanged(const QModelIndex&,const QModelIndex&)));
    connect(model, SIGNAL(modelReset()), this, SLOT(refresh()));
    connect(model, SIGNAL(layoutChanged()), this, SLOT(refresh()));

    emit tableChanged();
    EXIT("")
}

QString TableModel::filter() const
{
    ENTER("")
    return _filter;
}

void TableModel::setFilter(const QString &filter)
{
    ENTER(filter)
    _filter = filter;

    if (_database.isEmpty() || _database.isNull()) return;
    if (_table.isEmpty() || _table.isNull()) return;

    model->setFilter(_filter);
    model->select();
    emit filterChanged();

    EXIT("")
}

void TableModel::classBegin()
{
}

void TableModel::componentComplete()
{
}

void TableModel::refresh()
{
    if (_count) {
        emit beginRemoveRows(QModelIndex(), 0, _count);
        _count = 0;
        emit endRemoveRows();
    }
    int newcount = model->rowCount();
    if (newcount) {
        emit beginInsertRows(QModelIndex(), 0, newcount-1);
        _count = newcount;
        emit endInsertRows();
    }
}

void TableModel::inserted(const QModelIndex &index, int start, int end)
{
    emit beginInsertRows(QModelIndex(), start, end);
    _count = model->rowCount();
    emit endInsertRows();
}

void TableModel::removed(const QModelIndex &index, int start, int end)
{
    emit beginRemoveRows(QModelIndex(), start, end);
    _count = model->rowCount();
    emit endRemoveRows();
}

void TableModel::handleDataChanged(const QModelIndex &start, const QModelIndex &end)
{
    emit dataChanged(index(start.row(),0), index(end.row(),0));
}

void TableModel::createTable(const QString& sql)
{
    if (!db.isOpen()) return;
    if (_table.isEmpty() || _table.isNull()) return;

    QSqlQuery query("CREATE TABLE IF NOT EXISTS "+_table+" (" + sql + ")",db);
    query.exec();
}
