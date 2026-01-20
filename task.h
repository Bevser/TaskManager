#pragma once

#include <QString>
#include <QDateTime>
#include <QTimer>
#include <QObject>

/**
 * @class Task
 * @brief Представляет задачу с автоматическим выполнением и отслеживанием прогресса
 *
 * Класс Task предоставляет функциональность для управления задачами,
 * которые выполняются асинхронно с автоматическим обновлением прогресса.
 * Каждая задача имеет название, дату создания и прогресс выполнения от 0 до 100%.
 *
 */
class Task : public QObject {
    Q_OBJECT

    /// Минимальный интервал обновления таймера (мс)
    static constexpr int MIN_TIMER_INTERVAL = 50;

    /// Максимальный интервал обновления таймера (мс)
    static constexpr int MAX_TIMER_INTERVAL = 1000;

    /// Минимальное увеличение прогресса за один шаг
    static constexpr int MIN_PROGRESS_INCREMENT = 1;

    /// Максимальное увеличение прогресса за один шаг
    static constexpr int MAX_PROGRESS_INCREMENT = 4;

public:
    /// Максимальное значение прогресса
    static constexpr int MAX_PROGRESS = 100;

    /**
     * @brief Конструктор задачи
     * @param name Название задачи
     * @param parent Родительский QObject (по умолчанию nullptr)
     */
    explicit Task(const QString &name, QObject *parent = nullptr);

    /**
     * @brief Получить название задачи
     * @return Константная ссылка на название
     */
    QString getName() const { return m_name; }

    /**
     * @brief Получить дату создания задачи
     * @return Дата и время создания
     */
    QDateTime getDate() const { return m_date; }

    /**
     * @brief Получить текущий прогресс выполнения
     * @return Прогресс в диапазоне [0, 100]
     */
    int getProgress() const { return m_progress; }

    /**
     * @brief Проверить, выполняется ли задача в данный момент
     * @return true если задача запущена, false в противном случае
     */
    bool isRunning() const { return m_running; }

    /**
     * @brief Проверить, выбрана ли задача в UI
     * @return true если задача выбрана, false в противном случае
     */
    bool isSelected() const { return m_isSelected; }

    /**
     * @brief Установить состояние выбора задачи
     * @param sel true для выбора, false для снятия выбора
     */
    void setSelected(bool sel) { m_isSelected = sel; }

    /**
     * @brief Запустить выполнение задачи
     *
     * Запускает таймер с рандомным интервалом для автоматического
     * обновления прогресса. Не имеет эффекта, если задача уже запущена
     * или выполнена на 100%.
     */
    void start();

    /**
     * @brief Остановить выполнение задачи
     *
     * Останавливает таймер и сохраняет текущий прогресс.
     * Задачу можно будет запустить снова с текущего прогресса.
     */
    void stop();

signals:
    /**
     * @brief Сигнал об изменении прогресса
     * @param progress Новое значение прогресса [0, 100]
     */
    void progressChanged(int progress);

    /**
     * @brief Сигнал об изменении любых данных задачи
     *
     * Универсальный сигнал для обновления UI
     */
    void dataChanged();

private slots:
    /**
     * @brief Слот обновления прогресса
     *
     * Вызывается таймером для увеличения прогресса на рандомное значение.
     * Автоматически останавливает задачу при достижении 100%.
     */
    void updateProgress();

private:
    /**
     * @brief Получить рандомный интервал для таймера
     * @return Интервал в миллисекундах
     */
    int getRandomInterval() const;

    /**
     * @brief Получить рандомное приращение прогресса
     * @return Значение приращения
     */
    int getRandomIncrement() const;

    QString m_name;             ///< Название задачи
    QDateTime m_date;           ///< Дата и время создания
    int m_progress;             ///< Текущий прогресс [0, 100]
    bool m_running;             ///< Флаг выполнения
    bool m_isSelected;          ///< Флаг выбора в UI
    QTimer *m_timer{nullptr};   ///< Таймер для обновления прогресса
};

