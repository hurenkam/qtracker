#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "tablemodel.h"

//#define ENABLE_DEBUG
#include "helpers.h"

TableModel::TableModel(QObject *parent)
    : QAbstractListModel(parent)
    , db(Database::Db())
    , model(0)
    //, _database("")
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
    if (!model) { LOG("TableModel::refresh: no model set") return 0; }

    //return model->rowCount(parent);
    return model->rowCount(QModelIndex());
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    ENTER("")
    if (!model) { LOG("TableModel::refresh: no model set") return 0; }

    return model->columnCount(parent);
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    ENTER(index << role)
    QModelIndex myindex();

    if (!model) { LOG("TableModel::refresh: no model set") return QVariant(); }

    if (role > Qt::UserRole)
        return model->data(this->index(index.row(),role - Qt::UserRole -1),Qt::DisplayRole);
    else
        return model->data(index,role);
}
/*
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
    emit countChanged();

    EXIT("")
}
*/
QString TableModel::table() const
{
    ENTER("")
    return _table;
}

void TableModel::setTable(const QString &table)
{
    ENTER(table)
    _table = table;

    //if (_database.isEmpty() || _database.isNull()) return;

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
    emit countChanged();

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

    //if (_database.isEmpty() || _database.isNull()) return;
    if (_table.isEmpty() || _table.isNull()) return;
    if (!model) { LOG("TableModel::setFilter: no model set") return; }


    model->setFilter(_filter);
    model->select();
    emit filterChanged();
    emit countChanged();


    EXIT("")
}
/*
QVariant TableModel::platform() const
{
    ENTER("")
    int result = -1;
#if   defined(Q_OS_SYMBIAN)
    result = 0;
#elif defined(Q_WS_MAEMO_5)
    result = 1;
#elif defined(Q_WS_SIMULATOR)
    result = 2;
#endif
    return QVariant(result);
}

void TableModel::setPlatform(const QVariant &newplatform)
{
    ENTER(newplatform.toInt())
    EXIT("")
}
*/
void TableModel::classBegin()
{
    ENTER("")
}

void TableModel::componentComplete()
{
    ENTER("")
}

void TableModel::refresh()
{
    ENTER("")
    if (!model) { LOG("TableModel::refresh: no model set") return; }

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
    EXIT("")
}

void TableModel::inserted(const QModelIndex &index, int start, int end)
{
    ENTER("")
    if (!model) { LOG("TableModel::inserted: no model set") return; }

    emit beginInsertRows(QModelIndex(), start, end);
    _count = model->rowCount();
    emit endInsertRows();
    EXIT("")
}

void TableModel::removed(const QModelIndex &index, int start, int end)
{
    ENTER("")
    if (!model) { LOG("TableModel::removed: no model set") return; }

    emit beginRemoveRows(QModelIndex(), start, end);
    _count = model->rowCount();
    emit endRemoveRows();
    EXIT("")
}

void TableModel::handleDataChanged(const QModelIndex &start, const QModelIndex &end)
{
    ENTER("")
    emit dataChanged(index(start.row(),0), index(end.row(),0));
    EXIT("")
}

void TableModel::createTable(const QString& sql)
{
    ENTER("CREATE TABLE IF NOT EXISTS " << _table << "(" << sql << ")")
    if (!db.isOpen()) return;
    if (_table.isEmpty() || _table.isNull()) return;

    QSqlQuery query("CREATE TABLE IF NOT EXISTS "+_table+" (" + sql + ")",db);
    query.exec();
    EXIT("")
}

Q_INVOKABLE QVariantMap TableModel::get(const QVariant& index) {
    ENTER("")
    QVariantMap result;
    if (!model) { LOG("TableModel::set: no model set") return result; }

    for (int i = 0; i< model->columnCount(QModelIndex()); ++i) {
        result[model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString().toLatin1()] =
                model->data(this->index(index.toInt(),i),Qt::DisplayRole);
    }
    return result;
}

Q_INVOKABLE void TableModel::set(const QVariant& index, const QVariantMap& value) {
    ENTER("")
    if (!model) { LOG("TableModel::set: no model set") return; }

    QSqlRecord record = model->record();
    for (int i =0; i<value.keys().count(); i++)
    {
        record.setValue(value.keys()[i],value.values()[i]);
    }
    model->setRecord(index.toInt(),record);
    model->submit();
    QModelIndex mi=createIndex(index.toInt(),0);
    emit dataChanged(mi,mi);
    EXIT("")
}

Q_INVOKABLE QVariant TableModel::append(const QVariantMap& value) {
    ENTER("")
    if (!model) { LOG("TableModel::append: no model set") return QVariant(); }

    QSqlRecord record = model->record();

    for (int i =0; i<value.keys().count(); i++)
    {
        LOG("key: " << value.keys()[i] << " value: " << value.values()[i].toString())
        record.setValue(value.keys()[i],value.values()[i]);
    }

    LOG("record: " << record)
    model->insertRecord(-1,record);
    model->submit();
    emit countChanged();
    return model->query().lastInsertId();
}

Q_INVOKABLE void TableModel::exec(const QVariant& cmd)
{
    ENTER("")
    QSqlQuery query(db);
    QString sql(cmd.toString());
    bool result = query.exec (sql);
    if (!result) { EXIT("unable to execute cmd" << cmd.toString() << db.lastError().text()) return; }
    EXIT("")
}
