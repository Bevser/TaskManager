#pragma once

#include <QAbstractListModel>
#include "task.h"

/**
 * @class TaskModel
 * @brief Модель данных для управления списком задач
 *
 * TaskModel представляет собой модель на основе QAbstractListModel,
 * которая хранит и управляет коллекцией задач. Модель автоматически
 * обновляет представление при изменении данных задач и поддерживает
 * автоматическую сортировку по дате создания.
 */
class TaskModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * @enum TaskRoles
     * @brief Пользовательские роли для доступа к данным задач
     */
    enum TaskRoles {
        NameRole = Qt::UserRole + 1,    ///< Название задачи (QString)
        DateRole,                       ///< Дата создания (QDateTime)
        ProgressRole,                   ///< Прогресс выполнения (int)
        RunningRole,                    ///< Статус выполнения (bool)
        TaskPtrRole                     ///< Указатель на объект Task (Task*)
    };

    /**
     * @brief Конструктор модели
     * @param parent Родительский объект
     */
    explicit TaskModel(QObject *parent = nullptr);

    /**
     * @brief Получить количество задач в модели
     * @param parent Родительский индекс (не используется в списковой модели)
     * @return Количество задач
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Получить данные для указанной роли и индекса
     * @param index Индекс элемента
     * @param role Роль данных
     * @return Данные в формате QVariant
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Получить имена ролей для QML
     * @return Хеш-таблица ролей
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Добавить новую задачу
     * @param name Название задачи
     *
     * Создаёт новую задачу с указанным названием и добавляет её в модель.
     * Автоматически подключает сигналы задачи для обновления представления.
     * После добавления выполняется сортировка по дате.
     */
    void addTask(const QString &name);

    /**
     * @brief Удалить задачу по индексу
     * @param row Индекс строки для удаления
     *
     * Удаляет задачу из модели и освобождает память.
     */
    void removeTask(int row);

    /**
     * @brief Получить указатель на задачу по индексу
     * @param row Индекс строки
     * @return Указатель на задачу или nullptr если индекс невалидный
     */
    Task* getTask(int row) const;

    /**
     * @brief Проверить наличие задачи с указанным именем
     * @param name Название задачи для поиска
     * @return true если задача с таким именем существует
     *
     * Поиск выполняется без учёта регистра.
     */
    bool hasTaskWithName(const QString &name) const;

private slots:
    /**
     * @brief Слот для обработки изменений в задаче
     *
     * Вызывается при изменении данных задачи (прогресс, статус и т.д.).
     * Автоматически оповещает представление об обновлении через dataChanged.
     */
    void onTaskDataChanged();

private:
    QList<Task*> m_tasks{};  ///< Список задач
};
