#include "task.h"
#include <QRandomGenerator>

Task::Task(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_date(QDateTime::currentDateTime())
    , m_progress(0)
    , m_running(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Task::updateProgress);
}

void Task::start()
{
    if (!m_running && m_progress < MAX_PROGRESS)
    {
        m_running = true;
        m_timer->start(getRandomInterval());
        emit dataChanged();
    }
}

void Task::stop()
{
    if (m_running)
    {
        m_running = false;
        m_timer->stop();
        emit dataChanged();
    }
}

void Task::updateProgress()
{
    if (m_progress < MAX_PROGRESS)
    {
        // Увеличиваем прогресс
        m_progress = qMin(m_progress + getRandomIncrement(), 100);

        emit progressChanged(m_progress);

        if (m_progress >= 100)
            stop();
        else
            m_timer->setInterval(getRandomInterval());
    }
}

int Task::getRandomInterval() const
{
    return QRandomGenerator::global()->bounded(MIN_TIMER_INTERVAL, MAX_TIMER_INTERVAL);
}

int Task::getRandomIncrement() const
{
    return QRandomGenerator::global()->bounded(MIN_PROGRESS_INCREMENT, MAX_PROGRESS_INCREMENT);
}
