#pragma once

#include <QSortFilterProxyModel>

/**
 * @class TaskProxyModel
 * @brief Прокси-модель для фильтрации и сортировки задач
 *
 * TaskProxyModel предоставляет возможность фильтрации и сортировки задач
 * без изменения исходной модели данных. Поддерживает три режима фильтрации:
 * все задачи, только активные (выполняющиеся) и только неактивные.
 * Автоматически сортирует задачи по дате создания (новые сверху).
 *
 * @note Наследует QSortFilterProxyModel для прозрачной работы с исходной моделью
 */
class TaskProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /**
     * @enum FilterType
     * @brief Типы фильтрации задач
     */
    enum FilterType {
        All = 0,      ///< Показать все задачи
        Active = 1,   ///< Показать только выполняющиеся задачи
        Inactive = 2  ///< Показать только остановленные задачи
    };
    Q_ENUM(FilterType)

    /**
     * @brief Конструктор прокси-модели
     * @param parent Родительский объект
     */
    explicit TaskProxyModel(QObject *parent = nullptr);

    /**
     * @brief Установить тип фильтрации
     * @param type Тип фильтра
     *
     * Применяет указанный фильтр и автоматически обновляет представление.
     */
    void setFilterType(FilterType type);

    /**
     * @brief Получить текущий тип фильтрации
     * @return Активный тип фильтра
     */
    FilterType filterType() const { return m_filterType; }

protected:
    /**
     * @brief Проверить, соответствует ли строка текущему фильтру
     * @param source_row Индекс строки в исходной модели
     * @param source_parent Родительский индекс в исходной модели
     * @return true если строка проходит фильтр
     *
     * Виртуальный метод QSortFilterProxyModel, определяющий логику фильтрации.
     * Проверяет статус выполнения задачи и сравнивает с текущим типом фильтра.
     */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    /**
     * @brief Сравнить две строки для сортировки
     * @param source_left Индекс первого элемента
     * @param source_right Индекс второго элемента
     * @return true если левый элемент должен быть раньше правого
     *
     * Сортирует задачи по дате создания в порядке убывания (новые сверху).
     */
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    FilterType m_filterType;  ///< Текущий тип фильтра
};
