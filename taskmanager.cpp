#include "taskmanager.h"

TaskManager::TaskManager(QWidget *parent)
    : QMainWindow(parent)
{
    m_model = new TaskModel(this);
    m_proxyModel = new TaskProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_delegate = new TaskDelegate(this);

    setupUI();

    connect(m_delegate, &TaskDelegate::startStopClicked,
            this, &TaskManager::onStartStopClicked);
}

void TaskManager::setupUI()
{
    setWindowTitle("Менеджер задач");
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGIN, MAIN_LAYOUT_MARGIN,
                                     MAIN_LAYOUT_MARGIN, MAIN_LAYOUT_MARGIN);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);

    // Заголовок
    auto *titleLabel = new QLabel("Интерактивный список задач", this);
    titleLabel->setStyleSheet(
        "font-size: 24px; "
        "font-weight: bold; "
        "color: #333; "
        "margin-bottom: 10px;"
        );
    m_mainLayout->addWidget(titleLabel);

    // Панели управления
    m_mainLayout->addLayout(createControlPanel());
    m_mainLayout->addLayout(createFilterPanel());

    // Список задач
    setupTaskList();
    m_mainLayout->addWidget(m_listView);

    // Применяем стили
    applyStyles();
}

QHBoxLayout* TaskManager::createControlPanel()
{
    auto *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(10);

    // Поле ввода
    m_taskInput = new QLineEdit(this);
    m_taskInput->setPlaceholderText("Введите название новой задачи...");
    m_taskInput->setMinimumHeight(INPUT_HEIGHT);
    m_taskInput->setStyleSheet(
        "QLineEdit {"
        "    border: 2px solid #ddd;"
        "    border-radius: 20px;"
        "    padding: 5px 15px;"
        "    font-size: 13px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "}"
        );
    controlLayout->addWidget(m_taskInput, 3);

    // Кнопка добавления
    m_addButton = new QPushButton("Добавить задачу", this);
    m_addButton->setMinimumSize(ADD_BUTTON_WIDTH, BUTTON_HEIGHT);
    m_addButton->setFocusPolicy(Qt::NoFocus);
    m_addButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 20px;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0b7dda;"
        "}"
        );
    connect(m_addButton, &QPushButton::clicked, this, &TaskManager::addTask);
    controlLayout->addWidget(m_addButton);

    // Кнопка удаления
    m_deleteButton = new QPushButton("Удалить выбранные", this);
    m_deleteButton->setMinimumSize(DELETE_BUTTON_WIDTH, BUTTON_HEIGHT);
    m_deleteButton->setFocusPolicy(Qt::NoFocus);
    m_deleteButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #f44336;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 20px;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #da190b;"
        "}"
        );
    connect(m_deleteButton, &QPushButton::clicked, this, &TaskManager::deleteSelectedTasks);
    controlLayout->addWidget(m_deleteButton);

    return controlLayout;
}

QHBoxLayout* TaskManager::createFilterPanel()
{
    auto *filterLayout = new QHBoxLayout();

    auto *filterLabel = new QLabel("Фильтр:", this);
    filterLabel->setStyleSheet("font-weight: bold; font-size: 13px;");
    filterLayout->addWidget(filterLabel);

    m_filterCombo = new QComboBox(this);
    m_filterCombo->addItems({"Все задачи", "Активные", "Неактивные"});
    m_filterCombo->setMinimumHeight(FILTER_COMBO_HEIGHT);
    m_filterCombo->setFocusPolicy(Qt::StrongFocus);
    m_filterCombo->setStyleSheet(
        "QComboBox {"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "    padding: 5px 15px;"
        "    background-color: white;"
        "    font-size: 13px;"
        "}"
        "QComboBox:focus {"
        "    border: 1px solid #2196F3;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "    outline: none;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "    background-color: white;"
        "    selection-background-color: #E3F2FD;"
        "    selection-color: #1976D2;"
        "    padding: 4px;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "    border-radius: 6px;"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "    background-color: #F5F5F5;"
        "}"
        "QComboBox QAbstractItemView::item:selected {"
        "    background-color: #E3F2FD;"
        "    color: #1976D2;"
        "}"
        );
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TaskManager::onFilterChanged);
    filterLayout->addWidget(m_filterCombo);
    filterLayout->addStretch();

    return filterLayout;
}

void TaskManager::setupTaskList()
{
    m_listView = new QListView(this);
    m_listView->setModel(m_proxyModel);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    m_listView->setFocusPolicy(Qt::NoFocus);
    m_listView->setMouseTracking(true);
    m_listView->setStyleSheet(
        "QListView {"
        "    background-color: #f0f0f0;"
        "    border: none;"
        "    border-radius: 15px;"
        "    outline: none;"
        "}"
        "QListView::item {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QListView::item:hover {"
        "    background: transparent;"
        "}"
        // Скроллбар
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #f0f0f0;"
        "    width: 8px;"
        "    margin: 0px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #bbb;"
        "    min-height: 20px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #999;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );
}

void TaskManager::applyStyles()
{
    m_centralWidget->setStyleSheet("background-color: #fafafa;");
}

void TaskManager::addTask()
{
    const QString taskName = m_taskInput->text().trimmed();

    if (taskName.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите название задачи!");
        return;
    }

    if (m_model->hasTaskWithName(taskName))
    {
        QMessageBox::warning(this, "Ошибка", "Задача с таким названием уже существует!");
        return;
    }

    m_model->addTask(taskName);
    m_taskInput->clear();
}

void TaskManager::deleteSelectedTasks()
{
    // Проверяем наличие выбранных и активных задач
    bool hasSelected = false;
    bool hasActive = false;

    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        auto *task = m_model->getTask(i);
        if (task && task->isSelected())
        {
            hasSelected = true;
            if (task->isRunning())
                hasActive = true;
        }
    }

    if (!hasSelected)
        return;

    // Запрашиваем подтверждение для активных задач
    if (hasActive)
    {
        const auto reply = QMessageBox::question(
            this,
            "Удаление",
            "Удалить активные задачи?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No)
            return;
    }

    m_model->removeSelectedTasks();
}

void TaskManager::onFilterChanged(int index)
{
    m_proxyModel->setFilterType(static_cast<TaskProxyModel::FilterType>(index));
}

void TaskManager::onStartStopClicked(const QModelIndex &proxyIndex)
{
    // Преобразуем индекс прокси в индекс исходной модели
    const QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);
    Task *task = m_model->getTask(sourceIndex.row());

    if (!task)
        return;

    if (task->isRunning())
        task->stop();
    else if (task->getProgress() < Task::MAX_PROGRESS)
        task->start();
}
