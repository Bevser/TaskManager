#include "taskdelegate.h"
#include "taskmodel.h"
#include "task.h"
#include <QPainter>
#include <QMouseEvent>

TaskDelegate::TaskDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void TaskDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Получаем данные из модели
    const QString name = index.data(TaskModel::NameRole).toString();
    const QDateTime date = index.data(TaskModel::DateRole).toDateTime();
    const int progress = index.data(TaskModel::ProgressRole).toInt();
    const bool running = index.data(TaskModel::RunningRole).toBool();

    const QRect itemRect = option.rect.adjusted(ITEM_MARGIN, ITEM_VERTICAL_MARGIN,
                                                -ITEM_MARGIN, -ITEM_VERTICAL_MARGIN);

    // Отрисовка компонентов
    const bool isSelected = option.state & QStyle::State_Selected;
    const bool isHovered = option.state & QStyle::State_MouseOver;

    drawBackground(painter, itemRect, isSelected, isHovered);
    drawDate(painter, itemRect, date);
    drawName(painter, itemRect, name);

    if (progress > 0)
        drawProgress(painter, itemRect, progress);

    const QRect buttonRect = getButtonRect(option);
    drawButton(painter, buttonRect, running, progress);

    painter->restore();
}

QSize TaskDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(800, ITEM_HEIGHT);
}

bool TaskDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        auto *mouseEvent = static_cast<QMouseEvent*>(event);
        const auto buttonRect = getButtonRect(option);

        if (buttonRect.contains(mouseEvent->pos()))
        {
            // Клик по кнопке
            emit startStopClicked(index);
            return true;
        }

        return false;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QRect TaskDelegate::getButtonRect(const QStyleOptionViewItem &option) const
{
    const QRect rect = option.rect;
    return QRect(rect.right() - BUTTON_RIGHT_OFFSET,
                 rect.top() + BUTTON_TOP_OFFSET,
                 BUTTON_WIDTH,
                 BUTTON_HEIGHT);
}

void TaskDelegate::drawBackground(QPainter *painter, const QRect &rect,
                                  bool isSelected, bool isHovered) const
{
    if (isSelected)
    {
        painter->setBrush(QColor("#e3f2fd"));
        painter->setPen(QPen(QColor("#2196F3"), 2));
    }
    else
    {
        painter->setBrush(Qt::white);
        painter->setPen(QPen(QColor("#e0e0e0"), 1));

        if (isHovered)
            painter->setPen(QPen(QColor("#2196F3"), 1));
    }

    painter->drawRoundedRect(rect, BORDER_RADIUS, BORDER_RADIUS);
}

void TaskDelegate::drawDate(QPainter *painter, const QRect &rect,
                            const QDateTime &date) const
{
    const auto dateRect = rect.adjusted(DATE_LEFT_MARGIN, 0, 0, 0);

    painter->setPen(QColor("#7f8c8d"));
    painter->setFont(QFont("Arial", 10));
    painter->drawText(dateRect.left(), dateRect.top(),
                      DATE_WIDTH, dateRect.height(),
                      Qt::AlignVCenter | Qt::AlignLeft,
                      date.toString("dd.MM.yyyy hh:mm"));
}

void TaskDelegate::drawName(QPainter *painter, const QRect &rect,
                            const QString &name) const
{
    const QRect nameRect = rect.adjusted(NAME_LEFT_MARGIN, 0,
                                         -RIGHT_CONTENT_MARGIN, 0);

    painter->setPen(QColor("#2c3e50"));
    QFont boldFont("Arial", 12);
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft, name);
}

void TaskDelegate::drawProgress(QPainter *painter, const QRect &rect,
                                int progress) const
{
    const QRect progressRect(rect.right() - PROGRESS_RIGHT_OFFSET,
                             rect.top() + PROGRESS_TOP_OFFSET,
                             PROGRESS_SIZE,
                             PROGRESS_SIZE);

    const QRect innerRect = progressRect.adjusted(PROGRESS_INNER_MARGIN,
                                                  PROGRESS_INNER_MARGIN,
                                                  -PROGRESS_INNER_MARGIN,
                                                  -PROGRESS_INNER_MARGIN);

    // Фоновый круг
    painter->setPen(QPen(QColor(220, 220, 220), PROGRESS_LINE_WIDTH));
    painter->drawArc(innerRect, 0, 360 * 16);

    // Прогресс
    if (progress > 0)
    {
        painter->setPen(QPen(QColor(76, 175, 80), PROGRESS_LINE_WIDTH));
        const int angle = std::clamp(progress, 0, 100) * 360 * 16 / 100;
        painter->drawArc(innerRect, 90 * 16, -angle);
    }

    // Текст процента
    painter->setPen(Qt::black);
    QFont progressFont("Arial", 9);
    progressFont.setBold(true);
    painter->setFont(progressFont);
    painter->drawText(progressRect, Qt::AlignCenter,
                      QString::number(progress) + "%");
}

void TaskDelegate::drawButton(QPainter *painter, const QRect &buttonRect,
                              bool running, int progress) const
{
    QString buttonText;
    QColor buttonColor;

    if (running)
    {
        buttonText = "Стоп";
        buttonColor = QColor("#e74c3c");
    }
    else
    {
        buttonText = (progress >= 100) ? "Готово" : "Старт";
        buttonColor = (progress >= 100) ? QColor("#95a5a6") : QColor("#2ecc71");
    }

    painter->setBrush(buttonColor);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(buttonRect, BUTTON_BORDER_RADIUS, BUTTON_BORDER_RADIUS);

    painter->setPen(Qt::white);
    QFont buttonFont("Arial", 11);
    buttonFont.setBold(true);
    painter->setFont(buttonFont);
    painter->drawText(buttonRect, Qt::AlignCenter, buttonText);
}
