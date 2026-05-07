#include "mainwindow.h"
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QDebug>

// 食材按钮类实现
IngredientButton::IngredientButton(const QString &text, QWidget *parent) : QPushButton(text, parent), m_selected(false) {}

bool IngredientButton::isSelected() const { return m_selected; }

void IngredientButton::setSelected(bool selected) {
    m_selected = selected;
    if (m_selected) {
        setStyleSheet("background-color: lightblue;");
    } else {
        setStyleSheet("");
    }
}

void IngredientButton::mousePressEvent(QMouseEvent *event) {
    m_selected = !m_selected;
    if (m_selected) {
        setStyleSheet("background-color: lightblue;");
    } else {
        setStyleSheet("");
    }
    QPushButton::mousePressEvent(event);
}

// 餐盘区域类实现
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

// 主窗口实现
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

    QLabel *ingTitle = new QLabel("🥬 食材区（点击选择，最多3种）", this);
    ingTitle->setStyleSheet("font: bold 15px 'Noto Sans SC'; color: #3498db; text-align: center;");
    ingTitle->setAlignment(Qt::AlignCenter);
    ingLayout->addWidget(ingTitle);

    for (int i = 0; i < m_ingredients.size(); ++i) {
        const QString &ing = m_ingredients.at(i);
        IngredientButton *btn = new IngredientButton(ing, this);
        btn->setFixedSize(190, 45);
        btn->setStyleSheet("QPushButton { font: 14px 'Noto Sans SC'; color: #2c3e50; background: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; } QPushButton:hover { background: #3498db; color: white; }");
        m_ingredientButtons.append(btn);
        connect(btn, &QPushButton::clicked, this, &MainWindow::onIngredientButtonClicked);
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

    // 制作结果提示标签
    m_resultLabel = new QLabel("", this);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    m_resultLabel->setStyleSheet("font: bold 18px 'Noto Sans SC'; text-align: center;");
    main->addWidget(m_resultLabel);

    // 开始制作和清空选择按钮
    QHBoxLayout *actionBtnLayout = new QHBoxLayout();
    actionBtnLayout->setSpacing(30);
    actionBtnLayout->setAlignment(Qt::AlignCenter);

    m_startMakingBtn = new QPushButton("开始制作", this);
    m_startMakingBtn->setFixedSize(150, 45);
    actionBtnLayout->addWidget(m_startMakingBtn);

    m_clearBtn = new QPushButton("清空选择", this);
    m_clearBtn->setFixedSize(150, 45);
    actionBtnLayout->addWidget(m_clearBtn);

    main->addLayout(actionBtnLayout);

    m_generateBtn = new QPushButton("🎲 随机订单", this);
    m_generateBtn->setFixedSize(180, 50);
    m_generateBtn->setStyleSheet("QPushButton { font: 18px 'Noto Sans SC'; color: white; background: #e67e22; border: none; border-radius: 25px; } QPushButton:hover { background: #d35400; }");
    main->addWidget(m_generateBtn, 0, Qt::AlignCenter);

    connect(m_generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateOrder);
    connect(m_plateArea, &PlateArea::ingredientDropped, this, &MainWindow::onIngredientDropped);
    connect(m_addRiceBtn, &QPushButton::clicked, this, &MainWindow::onAddRiceClicked);
    connect(m_addSalmonBtn, &QPushButton::clicked, this, &MainWindow::onAddSalmonClicked);
    connect(m_startMakingBtn, &QPushButton::clicked, this, &MainWindow::onStartMaking);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearSelection);
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

// 食材按钮点击处理
void MainWindow::onIngredientButtonClicked() {
    m_selectedIngredients.clear();
    int count = 0;
    
    for (IngredientButton *btn : m_ingredientButtons) {
        if (btn->isSelected()) {
            if (count >= 3) {
                btn->setSelected(false);
                continue;
            }
            m_selectedIngredients.append(btn->text());
            count++;
        }
    }
    
    m_currentIngredients->setText(m_selectedIngredients.isEmpty() ? "当前食材: 空" : "当前食材: " + m_selectedIngredients.join(" + "));
}

// 配方检查函数
bool MainWindow::checkRecipe() {
    if (m_selectedIngredients.size() != 2) return false;
    
    bool hasRice = m_selectedIngredients.contains("🍚 米饭") || m_selectedIngredients.contains("🍚 鸡蛋");
    bool hasFish = m_selectedIngredients.contains("🍣 三文鱼") || m_selectedIngredients.contains("🍤 虾") || 
                   m_selectedIngredients.contains("🦀 蟹肉") || m_selectedIngredients.contains("🐟 金枪鱼") ||
                   m_selectedIngredients.contains("🦐 甜虾");
    
    return hasRice && hasFish;
}

// 获取配方名称
QString MainWindow::getRecipeName() {
    if (m_selectedIngredients.contains("🍣 三文鱼")) return "三文鱼寿司";
    if (m_selectedIngredients.contains("🍤 虾")) return "虾寿司";
    if (m_selectedIngredients.contains("🦀 蟹肉")) return "蟹肉寿司";
    if (m_selectedIngredients.contains("🐟 金枪鱼")) return "金枪鱼寿司";
    if (m_selectedIngredients.contains("🦐 甜虾")) return "甜虾寿司";
    return "寿司";
}

// 开始制作
void MainWindow::onStartMaking() {
    if (m_selectedIngredients.isEmpty()) {
        m_resultLabel->setText("请先选择食材");
        m_resultLabel->setStyleSheet("color: red;");
        return;
    }
    
    if (m_selectedIngredients.size() != 2) {
        m_resultLabel->setText("需要选择2种食材（米饭+鱼肉）");
        m_resultLabel->setStyleSheet("color: red;");
        return;
    }
    
    if (checkRecipe()) {
        m_resultLabel->setText("制作成功！" + getRecipeName());
        m_resultLabel->setStyleSheet("color: green;");
        m_totalScore += 20;
        qDebug() << "制作成功！+20分，总分:" << m_totalScore;
    } else {
        m_resultLabel->setText("制作失败！配方不正确");
        m_resultLabel->setStyleSheet("color: red;");
    }
}

// 清空选择
void MainWindow::onClearSelection() {
    for (IngredientButton *btn : m_ingredientButtons) {
        btn->setSelected(false);
    }
    m_selectedIngredients.clear();
    m_currentIngredients->setText("当前食材: 空");
    m_resultLabel->setText("");
}
