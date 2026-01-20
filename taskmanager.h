#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QMessageBox>
#include <QLabel>
#include "taskmodel.h"
#include "taskproxymodel.h"
#include "taskdelegate.h"

/**
 * @class TaskManager
 * @brief Главное окно приложения для управления задачами
 *
 * TaskManager представляет собой главное окно приложения, которое объединяет
 * модель данных (TaskModel), прокси-модель для фильтрации (TaskFilterModel)
 * и визуальное представление (QListView с TaskDelegate). Предоставляет
 * пользовательский интерфейс для создания, удаления и фильтрации задач.
 *
 */
class TaskManager : public QMainWindow
{
    Q_OBJECT

    /// Минимальная ширина окна
    static constexpr int MIN_WINDOW_WIDTH = 900;

    /// Минимальная высота окна
    static constexpr int MIN_WINDOW_HEIGHT = 600;

    /// Отступы главного контейнера
    static constexpr int MAIN_LAYOUT_MARGIN = 20;

    /// Расстояние между элементами в контейнере
    static constexpr int MAIN_LAYOUT_SPACING = 15;

    /// Высота поля ввода
    static constexpr int INPUT_HEIGHT = 40;

    /// Минимальная ширина кнопки добавления
    static constexpr int ADD_BUTTON_WIDTH = 140;

    /// Минимальная ширина кнопки удаления
    static constexpr int DELETE_BUTTON_WIDTH = 150;

    /// Высота кнопок управления
    static constexpr int BUTTON_HEIGHT = 40;

    /// Высота комбобокса фильтра
    static constexpr int FILTER_COMBO_HEIGHT = 35;

public:
    /**
     * @brief Конструктор главного окна
     * @param parent Родительский виджет
     */
    explicit TaskManager(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Добавить новую задачу
     *
     * Считывает название из поля ввода, проверяет на пустоту и дубликаты,
     * затем добавляет задачу в модель. После успешного добавления очищает поле ввода.
     */
    void addTask();

    /**
     * @brief Удалить выбранные задачи
     *
     * Находит все задачи с установленным флагом выбора и удаляет их.
     * Если среди выбранных есть активные задачи, запрашивает подтверждение.
     */
    void deleteSelectedTasks();

    /**
     * @brief Обработать изменение фильтра
     * @param index Индекс выбранного пункта в комбобоксе
     *
     * Применяет соответствующий фильтр через прокси-модель.
     */
    void onFilterChanged(int index);

    /**
     * @brief Обработать клик на кнопку "Старт/Стоп"
     * @param index Индекс задачи в прокси-модели
     *
     * Преобразует индекс прокси в индекс исходной модели и
     * запускает или останавливает соответствующую задачу.
     */
    void onStartStopClicked(const QModelIndex &index);

private:
    /**
     * @brief Настроить пользовательский интерфейс
     *
     * Создаёт и настраивает все виджеты, layout'ы и их стили.
     * Подключает сигналы к слотам.
     */
    void setupUI();

    /**
     * @brief Создать панель управления
     * @return Layout с элементами управления (поле ввода, кнопки)
     */
    QHBoxLayout* createControlPanel();

    /**
     * @brief Создать панель фильтрации
     * @return Layout с элементами фильтрации
     */
    QHBoxLayout* createFilterPanel();

    /**
     * @brief Настроить список задач
     *
     * Настраивает QListView с моделью, прокси и делегатом.
     */
    void setupTaskList();

    /**
     * @brief Применить стили к окну
     *
     * Применяет CSS-подобные стили к виджетам приложения.
     */
    void applyStyles();

    // Виджеты
    QWidget *m_centralWidget{nullptr};      ///< Центральный виджет
    QVBoxLayout *m_mainLayout{nullptr};     ///< Главный вертикальный layout
    QLineEdit *m_taskInput{nullptr};        ///< Поле ввода названия задачи
    QPushButton *m_addButton{nullptr};      ///< Кнопка добавления задачи
    QPushButton *m_deleteButton{nullptr};   ///< Кнопка удаления выбранных задач
    QComboBox *m_filterCombo{nullptr};      ///< Комбобокс выбора фильтра
    QListView *m_listView{nullptr};         ///< Список задач

    // Model/View/Delegate
    TaskModel *m_model{nullptr};            ///< Модель данных задач
    TaskProxyModel *m_proxyModel{nullptr}; ///< Прокси-модель для фильтрации и сортировки
    TaskDelegate *m_delegate{nullptr};      ///< Делегат для отрисовки задач
};

