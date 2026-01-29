#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_tasks.count();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_tasks.count())
        return QVariant();

    Task *task = m_tasks.at(index.row());

    switch (role)
    {
    case NameRole:
        return task->getName();
    case DateRole:
        return task->getDate();
    case ProgressRole:
        return task->getProgress();
    case RunningRole:
        return task->isRunning();
    case TaskPtrRole:
        return QVariant::fromValue(task);
    case Qt::DisplayRole:
        return task->getName();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TaskModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DateRole] = "date";
    roles[ProgressRole] = "progress";
    roles[RunningRole] = "running";
    roles[TaskPtrRole] = "taskPtr";
    return roles;
}

void TaskModel::addTask(const QString &name)
{
    Task *task = new Task(name, this);

    // Подключаем сигналы для автообновления
    connect(task, &Task::progressChanged, this, &TaskModel::onTaskDataChanged);
    connect(task, &Task::dataChanged, this, &TaskModel::onTaskDataChanged);

    // Добавляем в конец списка
    auto newRow = m_tasks.count();
    beginInsertRows(QModelIndex(), newRow, newRow);
    m_tasks.append(task);
    endInsertRows();
}

void TaskModel::removeTask(int row)
{
    if (row < 0 || row >= m_tasks.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    Task *task = m_tasks.takeAt(row);
    endRemoveRows();

    delete task;
}

Task* TaskModel::getTask(int row) const
{
    if (row < 0 || row >= m_tasks.count())
        return nullptr;
    return m_tasks.at(row);
}

bool TaskModel::hasTaskWithName(const QString &name) const
{
    for (const auto *task : m_tasks)
    {
        if (task->getName().compare(name, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

void TaskModel::onTaskDataChanged()
{
    auto *task = qobject_cast<Task*>(sender());
    if (!task)
        return;

    int row = m_tasks.indexOf(task);
    if (row != -1)
    {
        QModelIndex idx = index(row);
        emit dataChanged(idx, idx);
    }
}
