#pragma once

#include <QStyledItemDelegate>

/**
 * @class TaskDelegate
 * @brief Кастомный делегат для отрисовки и обработки взаимодействия с задачами
 *
 * TaskDelegate отвечает за визуальное представление задач в списке,
 * включая отрисовку даты, названия, прогресса и кнопки управления.
 * Также обрабатывает клики по кнопке и выделение элементов.
 */
class TaskDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    /// Отступы элемента от границ (пиксели)
    static constexpr int ITEM_MARGIN = 5;

    /// Вертикальный отступ элемента
    static constexpr int ITEM_VERTICAL_MARGIN = 2;

    /// Высота элемента списка
    static constexpr int ITEM_HEIGHT = 74;

    /// Ширина области даты
    static constexpr int DATE_WIDTH = 120;

    /// Отступ даты от левого края
    static constexpr int DATE_LEFT_MARGIN = 15;

    /// Отступ названия от области даты
    static constexpr int NAME_LEFT_MARGIN = 145;

    /// Отступ справа для кнопки и прогресса
    static constexpr int RIGHT_CONTENT_MARGIN = 250;

    /// Размер виджета кругового прогресса
    static constexpr int PROGRESS_SIZE = 45;

    /// Отступ прогресса от правого края
    static constexpr int PROGRESS_RIGHT_OFFSET = 200;

    /// Вертикальный отступ прогресса
    static constexpr int PROGRESS_TOP_OFFSET = 12;

    /// Внутренний отступ круга прогресса
    static constexpr int PROGRESS_INNER_MARGIN = 5;

    /// Толщина линии прогресса
    static constexpr int PROGRESS_LINE_WIDTH = 6;

    /// Ширина кнопки
    static constexpr int BUTTON_WIDTH = 90;

    /// Высота кнопки
    static constexpr int BUTTON_HEIGHT = 36;

    /// Отступ кнопки от правого края
    static constexpr int BUTTON_RIGHT_OFFSET = 110;

    /// Вертикальный отступ кнопки
    static constexpr int BUTTON_TOP_OFFSET = 19;

    /// Радиус скругления углов элемента
    static constexpr int BORDER_RADIUS = 12;

    /// Радиус скругления кнопки
    static constexpr int BUTTON_BORDER_RADIUS = 18;

public:
    /**
     * @brief Конструктор делегата
     * @param parent Родительский объект
     */
    explicit TaskDelegate(QObject *parent = nullptr);

    /**
     * @brief Деструктор по умолчанию
     */
    ~TaskDelegate() override = default;

    /**
     * @brief Отрисовка элемента задачи
     * @param painter Объект QPainter для рисования
     * @param option Опции стиля элемента
     * @param index Индекс элемента в модели
     *
     * Отрисовывает фон, дату, название, круговой прогресс и кнопку управления.
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    /**
     * @brief Получить рекомендуемый размер элемента
     * @param option Опции стиля
     * @param index Индекс элемента
     * @return Размер элемента
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    /**
     * @brief Обработка событий редактора (клики мыши)
     * @param event Событие
     * @param model Модель данных
     * @param option Опции стиля
     * @param index Индекс элемента
     * @return true если событие обработано
     *
     * Обрабатывает клики по кнопке "Старт/Стоп" и выделение элементов.
     */
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

signals:
    /**
     * @brief Сигнал о клике на кнопку "Старт/Стоп"
     * @param index Индекс задачи, на которой кликнули
     */
    void startStopClicked(const QModelIndex &index);

private:
    /**
     * @brief Получить прямоугольник кнопки
     * @param option Опции стиля элемента
     * @return Область кнопки
     */
    QRect getButtonRect(const QStyleOptionViewItem &option) const;

    /**
     * @brief Отрисовать фон элемента
     * @param painter Объект рисования
     * @param rect Область элемента
     * @param isSelected Выбран ли элемент
     * @param isHovered Наведён ли курсор
     */
    void drawBackground(QPainter *painter, const QRect &rect,
                        bool isSelected, bool isHovered) const;

    /**
     * @brief Отрисовать дату создания
     * @param painter Объект рисования
     * @param rect Область элемента
     * @param date Дата для отображения
     */
    void drawDate(QPainter *painter, const QRect &rect,
                  const QDateTime &date) const;

    /**
     * @brief Отрисовать название задачи
     * @param painter Объект рисования
     * @param rect Область элемента
     * @param name Название задачи
     */
    void drawName(QPainter *painter, const QRect &rect,
                  const QString &name) const;

    /**
     * @brief Отрисовать круговой прогресс
     * @param painter Объект рисования
     * @param rect Область элемента
     * @param progress Процент выполнения [0-100]
     */
    void drawProgress(QPainter *painter, const QRect &rect,
                      int progress) const;

    /**
     * @brief Отрисовать кнопку управления
     * @param painter Объект рисования
     * @param buttonRect Область кнопки
     * @param running Выполняется ли задача
     * @param progress Прогресс задачи
     */
    void drawButton(QPainter *painter, const QRect &buttonRect,
                    bool running, int progress) const;
};
