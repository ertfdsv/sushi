#include "mainwindow.h"
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QDebug>

IngredientButton::IngredientButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {}

void IngredientButton::mousePressEvent(QMouseEvent *event) {
    QDrag *drag = new QDrag(this);
    QMimeData *mime = new QMimeData();
    mime->setText(text());
    drag->setMimeData(mime);
    drag->exec();
}

PlateArea::PlateArea(QWidget *parent) : QWidget(parent) { setAcceptDrops(true); }
QString PlateArea::currentIngredient() const { return m_current; }
void PlateArea::clear() { m_current.clear(); }

void PlateArea::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasText()) event->acceptProposedAction();
}

void PlateArea::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasText()) {
        m_current = event->mimeData()->text();
        emit ingredientDropped(m_current);
        event->acceptProposedAction();
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_score(0), m_totalScore(0) {
    m_ingredients << "🍣 三文鱼" << "🍤 虾" << "🦀 蟹肉" << "🐟 金枪鱼"
                  << "🥑 牛油果" << "🥒 黄瓜" << "🍚 鸡蛋" << "🦐 甜虾";
    setupUI();
    generateOrder();
}

void MainWindow::setupUI() {
    setWindowTitle("🍣 寿司制作小游戏");
    setFixedSize(850, 700);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #e0f0f9;");
    setCentralWidget(central);

    QVBoxLayout *main = new QVBoxLayout(central);
    main->setContentsMargins(30, 30, 30, 30);
    main->setSpacing(12);

    QLabel *title = new QLabel("开寿司", this);
    title->setStyleSheet("font: bold 44px 'Source Han Serif SC'; color: #2c3e50; text-align: center; letter-spacing: 12px;");
    title->setAlignment(Qt::AlignCenter);
    main->addWidget(title);

    QLabel *subtitle = new QLabel("KAI-SUSHI", this);
    subtitle->setStyleSheet("font: 18px Arial; color: #7f8c8d; text-align: center; letter-spacing: 8px;");
    subtitle->setAlignment(Qt::AlignCenter);
    main->addWidget(subtitle);

    QFrame *line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("color: #bdc3c7;");
    main->addWidget(line1);

    QLabel *orderTitle = new QLabel("📋 当前订单", this);
    orderTitle->setStyleSheet("font: 14px 'Noto Sans SC'; color: #7f8c8d; text-align: center;");
    orderTitle->setAlignment(Qt::AlignCenter);
    main->addWidget(orderTitle);

    m_orderLabel = new QLabel("", this);
    m_orderLabel->setStyleSheet("font: bold 32px 'Source Han Serif SC'; color: #e74c3c; text-align: center; padding: 15px; background: white; border-radius: 10px; border: 2px solid #f1c40f;");
    m_orderLabel->setAlignment(Qt::AlignCenter);
    m_orderLabel->setMinimumHeight(80);
    main->addWidget(m_orderLabel);

    QLabel *scoreLabel = new QLabel(QString("⭐ %1").arg(m_score), this);
    scoreLabel->setStyleSheet("font: bold 16px 'Noto Sans SC'; color: #27ae60; text-align: center;");
    scoreLabel->setAlignment(Qt::AlignCenter);
    main->addWidget(scoreLabel);

    QFrame *line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("color: #bdc3c7;");
    main->addWidget(line2);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    btnLayout->setAlignment(Qt::AlignCenter);

    m_addRiceBtn = new QPushButton("🍚 添加米饭", this);
    m_addRiceBtn->setFixedSize(130, 45);
    m_addRiceBtn->setStyleSheet("QPushButton { font: 15px 'Noto Sans SC'; background: #27ae60; color: white; border: none; border-radius: 20px; } QPushButton:hover { background: #1e8449; }");
    btnLayout->addWidget(m_addRiceBtn);

    m_addSalmonBtn = new QPushButton("🍣 添加三文鱼", this);
    m_addSalmonBtn->setFixedSize(130, 45);
    m_addSalmonBtn->setStyleSheet("QPushButton { font: 15px 'Noto Sans SC'; background: #e67e22; color: white; border: none; border-radius: 20px; } QPushButton:hover { background: #d35400; }");
    btnLayout->addWidget(m_addSalmonBtn);
    main->addLayout(btnLayout);

    m_currentIngredients = new QLabel("当前食材: 空", this);
    m_currentIngredients->setStyleSheet("font: 14px 'Noto Sans SC'; color: #34495e; text-align: center; padding: 10px; background: white; border-radius: 8px;");
    m_currentIngredients->setAlignment(Qt::AlignCenter);
    main->addWidget(m_currentIngredients);

    QLabel *totalScoreLabel = new QLabel(QString("💰 %1").arg(m_totalScore), this);
    totalScoreLabel->setStyleSheet("font: bold 16px 'Noto Sans SC'; color: #9b59b6; text-align: center;");
    totalScoreLabel->setAlignment(Qt::AlignCenter);
    main->addWidget(totalScoreLabel);

    QFrame *line3 = new QFrame(this);
    line3->setFrameShape(QFrame::HLine);
    line3->setStyleSheet("color: #bdc3c7;");
    main->addWidget(line3);

    QHBoxLayout *center = new QHBoxLayout();
    center->setSpacing(30);

    QWidget *ingArea = new QWidget(this);
    ingArea->setStyleSheet("background: white; border-radius: 12px; border: 2px solid #3498db;");
    ingArea->setFixedWidth(220);
    QVBoxLayout *ingLayout = new QVBoxLayout(ingArea);
    ingLayout->setContentsMargins(15, 15, 15, 15);
    ingLayout->setSpacing(10);

    QLabel *ingTitle = new QLabel("🥬 食材区", this);
    ingTitle->setStyleSheet("font: bold 15px 'Noto Sans SC'; color: #3498db; text-align: center;");
    ingTitle->setAlignment(Qt::AlignCenter);
    ingLayout->addWidget(ingTitle);

    for (const QString &ing : m_ingredients) {
        IngredientButton *btn = new IngredientButton(ing, this);
        btn->setFixedSize(190, 45);
        btn->setStyleSheet("QPushButton { font: 14px 'Noto Sans SC'; color: #2c3e50; background: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; } QPushButton:hover { background: #3498db; color: white; }");
        ingLayout->addWidget(btn);
    }
    center->addWidget(ingArea);

    m_plateArea = new PlateArea(this);
    m_plateArea->setStyleSheet("background: white; border-radius: 12px; border: 2px dashed #f39c12;");
    m_plateArea->setFixedSize(220, 320);
    QVBoxLayout *plateLayout = new QVBoxLayout(m_plateArea);
    plateLayout->setContentsMargins(15, 15, 15, 15);
    QLabel *plateTitle = new QLabel("🍱 餐盘", this);
    plateTitle->setStyleSheet("font: bold 15px 'Noto Sans SC'; color: #f39c12; text-align: center;");
    plateTitle->setAlignment(Qt::AlignCenter);
    plateLayout->addWidget(plateTitle);
    center->addWidget(m_plateArea);
    main->addLayout(center);

    QFrame *line4 = new QFrame(this);
    line4->setFrameShape(QFrame::HLine);
    line4->setStyleSheet("color: #bdc3c7;");
    main->addWidget(line4);

    m_generateBtn = new QPushButton("🎲 随机订单", this);
    m_generateBtn->setFixedSize(180, 50);
    m_generateBtn->setStyleSheet("QPushButton { font: 18px 'Noto Sans SC'; color: white; background: #e67e22; border: none; border-radius: 25px; } QPushButton:hover { background: #d35400; }");
    main->addWidget(m_generateBtn, 0, Qt::AlignCenter);

    connect(m_generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateOrder);
    connect(m_plateArea, &PlateArea::ingredientDropped, this, &MainWindow::onIngredientDropped);
    connect(m_addRiceBtn, &QPushButton::clicked, this, &MainWindow::onAddRiceClicked);
    connect(m_addSalmonBtn, &QPushButton::clicked, this, &MainWindow::onAddSalmonClicked);
}

void MainWindow::generateOrder() {
    m_currentOrderIndex = QRandomGenerator::global()->bounded(m_ingredients.size());
    m_orderLabel->setText(m_ingredients[m_currentOrderIndex] + "寿司");
}

void MainWindow::onGenerateOrder() { generateOrder(); }

void MainWindow::onIngredientDropped(const QString &text) {
    if (text == m_ingredients[m_currentOrderIndex]) {
        m_score += 10;
        qDebug() << "正确！+10分，得分:" << m_score;
        generateOrder();
    } else {
        m_score = qMax(0, m_score - 5);
        qDebug() << "错误！-5分，得分:" << m_score;
    }
    m_plateArea->clear();
}

void MainWindow::onAddRiceClicked() {
    m_assembledIngredients.append("🍚 米饭");
    m_totalScore += 5;
    qDebug() << "[组装] 米饭+5，总分:" << m_totalScore;
    updateIngredientDisplay();
}

void MainWindow::onAddSalmonClicked() {
    m_assembledIngredients.append("🍣 三文鱼");
    m_totalScore += 15;
    qDebug() << "[组装] 三文鱼+15，总分:" << m_totalScore;
    updateIngredientDisplay();
    assembleSushi();
}

void MainWindow::assembleSushi() {
    if (m_assembledIngredients.contains("🍚 米饭") && m_assembledIngredients.contains("🍣 三文鱼")) {
        m_totalScore += 10;
        qDebug() << "🎉 组装成功！+10，总分:" << m_totalScore;
        m_assembledIngredients.clear();
        updateIngredientDisplay();
    }
}

void MainWindow::updateIngredientDisplay() {
    m_currentIngredients->setText(m_assembledIngredients.isEmpty() ? "当前食材: 空" : "当前食材: " + m_assembledIngredients.join(" + "));
}
