#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <qdeclarative.h>
#include <QStringList>
#include <QUrl>
#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlTableModel>

QT_BEGIN_HEADER

QT_MODULE(Declarative)

class QDeclarativeContext;
class QModelIndex;

class TableModel : public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)

    Q_PROPERTY(QString database READ database WRITE setDatabase NOTIFY databaseChanged)
    Q_PROPERTY(QString table    READ table    WRITE setTable    NOTIFY tableChanged)
    Q_PROPERTY(QString filter   READ filter   WRITE setFilter   NOTIFY filterChanged)
    Q_PROPERTY(int count        READ count                      NOTIFY countChanged)

public:
    Q_INVOKABLE void createTable(const QString& sql);

    TableModel(QObject *parent = 0);
    ~TableModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    int count() const { return rowCount(QModelIndex()); }

    QString database() const;
    void setDatabase(const QString &database);

    QString table() const;
    void setTable(const QString &table);

    QString filter() const;
    void setFilter(const QString &table);

    virtual void classBegin();
    virtual void componentComplete();

Q_SIGNALS:
    void databaseChanged();
    void tableChanged();
    void filterChanged();
    void countChanged();

private Q_SLOTS:
    void refresh();
    void inserted(const QModelIndex &index, int start, int end);
    void removed(const QModelIndex &index, int start, int end);
    void handleDataChanged(const QModelIndex &start, const QModelIndex &end);

private:
    Q_DISABLE_COPY(TableModel)
    QSqlDatabase    db;
    QSqlTableModel* model;
    QString _database;
    QString _table;
    QString _filter;
    int _count;
};


QML_DECLARE_TYPE(TableModel)

QT_END_HEADER

#endif // TABLEMODEL_H
