#include "taskproxymodel.h"
#include "taskmodel.h"
#include <QDatetime>

TaskProxyModel::TaskProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent),
    m_filterType(All)
{
    // Включаем динамическую сортировку
    setDynamicSortFilter(true);

    // Устанавливаем сортировку по колонке 0 в порядке убывания
    sort(0, Qt::DescendingOrder);
}

void TaskProxyModel::setFilterType(FilterType type)
{
    if (m_filterType == type)
        return;

    m_filterType = type;
    invalidateFilter();
}

bool TaskProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // Режим "Все задачи" - показываем всё
    if (m_filterType == All)
        return true;

    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    const bool isRunning = sourceModel()->data(index, TaskModel::RunningRole).toBool();

    // Фильтрация по статусу выполнения
    switch (m_filterType) {
    case Active:
        return isRunning;
    case Inactive:
        return !isRunning;
    default:
        return true;
    }
}

bool TaskProxyModel::lessThan(const QModelIndex &source_left,
                               const QModelIndex &source_right) const
{
    // Получаем даты создания для сравнения
    const QDateTime leftDate = sourceModel()->data(source_left, TaskModel::DateRole).toDateTime();
    const QDateTime rightDate = sourceModel()->data(source_right, TaskModel::DateRole).toDateTime();

    // Сортируем по убыванию даты (новые сверху)
    return leftDate < rightDate;
}
