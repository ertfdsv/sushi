#include "mainwindow.h"
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QDebug>
#include <QPalette>
#include <QGridLayout>
#include <QSizePolicy>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QMessageBox>

class PlateArea : public QWidget
{
public:
    PlateArea(QWidget *parent = nullptr) : QWidget(parent), m_hasRice(false), m_hasFish(false), m_sushiMade(false), m_hasIngredient(false), m_isMoving(false), m_moveOffset(0) {
        setAcceptDrops(true);
        setMinimumSize(280, 120);
        setStyleSheet("background: #f5e6d3; border-radius: 10px; border: 3px solid #8B4513;");
    }

    void clear() { m_hasRice = false; m_hasFish = false; m_sushiMade = false; m_hasIngredient = false; m_riceText.clear(); m_fishText.clear(); m_isMoving = false; m_moveOffset = 0; update(); }
    void addIngredient(const QString &ing) {
        m_hasIngredient = true;
        if (ing.contains("米饭")) {
            m_hasRice = true;
            m_riceText = ing;
        } else {
            m_hasFish = true;
            m_fishText = ing;
        }
        update();
    }
    bool hasIngredients() const { return m_hasIngredient; }
    bool canMakeSushi() const { return m_hasRice && m_hasFish; }
    void setSushiMade(bool made) { m_sushiMade = made; update(); }
    void setMoving(bool moving) { m_isMoving = moving; m_moveOffset = 0; }
    void setMoveOffset(int offset) { m_moveOffset = offset; }
    int getMoveOffset() const { return m_moveOffset; }
    QString getFishText() const { return m_fishText; }
    QString getRiceText() const { return m_riceText; }
    bool isMoving() const { return m_isMoving; }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
            update();
        }
    }

    void dragLeaveEvent(QDragLeaveEvent *event) override {
        update();
    }

    void dropEvent(QDropEvent *event) override {
        if (event->mimeData()->hasText()) {
            QString text = event->mimeData()->text();
            addIngredient(text);
            event->acceptProposedAction();
        }
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int sushiX = width()/2 - 50 - m_moveOffset;
        int sushiY = height()/2 - 30;

        if (m_sushiMade && !m_isMoving) {
            sushiX = width()/2 - 50;

            QString sushiIcon = "🍣";
            if (m_fishText.contains("三文鱼")) sushiIcon = "🍣";
            else if (m_fishText.contains("虾") && !m_fishText.contains("甜虾")) sushiIcon = "🍤";
            else if (m_fishText.contains("蟹肉")) sushiIcon = "🦀";
            else if (m_fishText.contains("金枪鱼")) sushiIcon = "🐟";
            else if (m_fishText.contains("甜虾")) sushiIcon = "🦐";

            p.setBrush(QBrush(QColor(255, 255, 255)));
            p.drawEllipse(sushiX, sushiY, 100, 60);
            p.setBrush(QBrush(QColor(250, 100, 100)));
            p.drawEllipse(sushiX + 10, sushiY + 10, 80, 40);
            QFont emojiFont("Segoe UI Emoji", 32);
            p.setFont(emojiFont);
            p.drawText(QRect(sushiX, sushiY, 100, 60), Qt::AlignCenter, sushiIcon);

            p.setPen(QPen(QColor(139, 69, 19), 2));
            for (int i = 0; i < 8; ++i) {
                int lineX = 15 + i * 35 - m_moveOffset;
                if (lineX > sushiX - 20 && lineX < sushiX + 120) continue;
                p.drawLine(lineX, 8, lineX, height() - 8);
            }
        } else if (m_isMoving) {
            QString sushiIcon = "🍣";
            if (m_fishText.contains("三文鱼")) sushiIcon = "🍣";
            else if (m_fishText.contains("虾") && !m_fishText.contains("甜虾")) sushiIcon = "🍤";
            else if (m_fishText.contains("蟹肉")) sushiIcon = "🦀";
            else if (m_fishText.contains("金枪鱼")) sushiIcon = "🐟";
            else if (m_fishText.contains("甜虾")) sushiIcon = "🦐";

            p.setBrush(QBrush(QColor(255, 255, 255)));
            p.drawEllipse(sushiX, sushiY, 100, 60);
            p.setBrush(QBrush(QColor(250, 100, 100)));
            p.drawEllipse(sushiX + 10, sushiY + 10, 80, 40);
            QFont emojiFont("Segoe UI Emoji", 32);
            p.setFont(emojiFont);
            p.drawText(QRect(sushiX, sushiY, 100, 60), Qt::AlignCenter, sushiIcon);

            p.setOpacity(0.7);
            QFont textFont("Microsoft YaHei", 10);
            p.setFont(textFont);
            p.setPen(QColor(139, 69, 19));
            p.drawText(rect(), Qt::AlignBottom | Qt::AlignCenter, "正在上餐中...");
            p.setOpacity(1.0);

            p.setPen(QPen(QColor(139, 69, 19), 2));
            for (int i = 0; i < 8; ++i) {
                int lineX = 15 + i * 35 - m_moveOffset;
                if (lineX > sushiX - 20 && lineX < sushiX + 120) continue;
                p.drawLine(lineX, 8, lineX, height() - 8);
            }
        } else {
            p.setPen(QPen(QColor(139, 69, 19), 2));
            for (int i = 0; i < 8; ++i) {
                int lineX = 15 + i * 35;
                p.drawLine(lineX, 8, lineX, height() - 8);
            }

            QFont emojiFont("Segoe UI Emoji", 20);
            p.setFont(emojiFont);

            int centerX = width() / 2;
            int centerY = height() / 2;

            if (m_hasRice && m_hasFish) {
                p.drawText(QPoint(centerX - 80, centerY + 8), m_riceText);
                p.drawText(QPoint(centerX + 10, centerY + 8), m_fishText);
            } else if (m_hasRice) {
                p.drawText(QPoint(centerX - 40, centerY + 8), m_riceText);
                p.drawText(rect(), Qt::AlignCenter, "🍙 + ?");
            } else if (m_hasFish) {
                p.drawText(QPoint(centerX - 40, centerY + 8), m_fishText);
                p.drawText(rect(), Qt::AlignCenter, "? + 🍙");
            } else {
                p.drawText(rect(), Qt::AlignCenter, "🍙 拖拽食材到此处");
            }
        }
    }

private:
    bool m_hasRice;
    bool m_hasFish;
    bool m_sushiMade;
    bool m_hasIngredient;
    QString m_riceText;
    QString m_fishText;
    int m_moveOffset;
    bool m_isMoving;
};

IngredientButton::IngredientButton(const QString &text, QWidget *parent) : QPushButton(text, parent), m_selected(false) {
    setAttribute(Qt::WA_Hover);
}

bool IngredientButton::isSelected() const { return m_selected; }

void IngredientButton::setSelected(bool selected) {
    m_selected = selected;
    if (m_selected) {
        setStyleSheet("background-color: #3498db; color: white;");
    } else {
        setStyleSheet("");
    }
}

void IngredientButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData();
        mimeData->setText(this->text());
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction);
    }
    QPushButton::mousePressEvent(event);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_score(0),
    m_gold(100), m_totalEarned(0), m_timeLeft(120), m_gameOver(false), m_completedOrders(0), m_sushiPrice(0), m_isSubmitting(false) {
    m_ingredients << "🍚 米饭" << "🍣 三文鱼" << "🍤 虾" << "🦀 蟹肉" << "🐟 金枪鱼" << "🦐 甜虾";
    m_madeSushi = "";
    m_plateArea = nullptr;
    m_gameWidget = nullptr;
    m_moveTimer = nullptr;
    setupWelcomeUI();
}

void MainWindow::setupWelcomeUI() {
    setWindowTitle("开寿司");
    setFixedSize(500, 600);
    setStyleSheet("background: #e0f0f9;");

    QWidget *welcomeWidget = new QWidget(this);
    setCentralWidget(welcomeWidget);

    QVBoxLayout *layout = new QVBoxLayout(welcomeWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(40);

    QLabel *title = new QLabel("开寿司", welcomeWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font: bold 72px \"Microsoft YaHei\"; color: #2c3e50; letter-spacing: 30px; background: transparent;");

    QLabel *subtitle = new QLabel("KAI-SUSHI", welcomeWidget);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font: 20px Arial; color: #3498db; letter-spacing: 8px; background: transparent;");

    QPushButton *startBtn = new QPushButton("开始营业", welcomeWidget);
    startBtn->setFixedSize(200, 60);
    startBtn->setCursor(Qt::PointingHandCursor);
    startBtn->setStyleSheet("QPushButton { font: bold 22px \"Microsoft YaHei\"; color: white; background: #3498db; border: none; border-radius: 30px; } QPushButton:hover { background: #2980b9; }");

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(startBtn);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartGame);
}

void MainWindow::onStartGame() {
    setupGameUI();
}

void MainWindow::setupGameUI() {
    setWindowTitle("🍣 寿司制作小游戏");
    setFixedSize(600, 850);

    m_gameWidget = new QWidget(this);
    setCentralWidget(m_gameWidget);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(232, 240, 249));
    m_gameWidget->setAutoFillBackground(true);
    m_gameWidget->setPalette(pal);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_gameWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel *title = new QLabel("🍣 开寿司", m_gameWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font: bold 28px \"Microsoft YaHei\"; color: #2c3e50;");
    mainLayout->addWidget(title);

    QFrame *line1 = new QFrame(m_gameWidget);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line1);

    QWidget *statusWidget = new QWidget(m_gameWidget);
    QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);
    statusLayout->setSpacing(80);
    statusLayout->setAlignment(Qt::AlignCenter);

    m_goldLabel = new QLabel(QString("💰 %1 金币").arg(m_gold), statusWidget);
    m_goldLabel->setAlignment(Qt::AlignCenter);
    m_goldLabel->setStyleSheet("font: bold 16px \"Microsoft YaHei\"; color: #c0392b; background: #fef9e7; padding: 6px 20px; border-radius: 12px;");
    statusLayout->addWidget(m_goldLabel);

    m_timeLabel = new QLabel(QString("⏱️ %1 秒").arg(m_timeLeft), statusWidget);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setStyleSheet("font: bold 16px \"Microsoft YaHei\"; color: #c0392b; background: #fef9e7; padding: 6px 20px; border-radius: 12px;");
    statusLayout->addWidget(m_timeLabel);

    mainLayout->addWidget(statusWidget);

    QFrame *line2 = new QFrame(m_gameWidget);
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line2);

    QWidget *customerWidget = new QWidget(m_gameWidget);
    customerWidget->setStyleSheet("background: white; border-radius: 12px; border: 2px solid #3498db; padding: 10px;");
    QVBoxLayout *customerLayout = new QVBoxLayout(customerWidget);

    QLabel *customerTitle = new QLabel("👤 顾客想要:", customerWidget);
    customerTitle->setAlignment(Qt::AlignCenter);
    customerTitle->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: #2980b9;");
    customerLayout->addWidget(customerTitle);

    m_customerLabel = new QLabel("三文鱼寿司", customerWidget);
    m_customerLabel->setAlignment(Qt::AlignCenter);
    m_customerLabel->setStyleSheet("font: bold 24px \"Microsoft YaHei\"; color: #e74c3c;");
    customerLayout->addWidget(m_customerLabel);

    mainLayout->addWidget(customerWidget);

    QFrame *line3 = new QFrame(m_gameWidget);
    line3->setFrameShape(QFrame::HLine);
    line3->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line3);

    QLabel *matLabel = new QLabel("🍙 寿司帘（拖拽食材到此处）", m_gameWidget);
    matLabel->setAlignment(Qt::AlignCenter);
    matLabel->setStyleSheet("font: bold 12px \"Microsoft YaHei\"; color: #8B4513;");
    mainLayout->addWidget(matLabel);

    m_plateArea = new PlateArea(m_gameWidget);
    mainLayout->addWidget(m_plateArea);

    QFrame *line4 = new QFrame(m_gameWidget);
    line4->setFrameShape(QFrame::HLine);
    line4->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line4);

    QWidget *ingWidget = new QWidget(m_gameWidget);
    ingWidget->setStyleSheet("background: white; border-radius: 12px; border: 2px solid #27ae60; padding: 10px;");
    QVBoxLayout *ingLayout = new QVBoxLayout(ingWidget);
    ingLayout->setSpacing(6);

    QLabel *ingTitle = new QLabel("🥬 选择食材", ingWidget);
    ingTitle->setAlignment(Qt::AlignCenter);
    ingTitle->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: #27ae60;");
    ingLayout->addWidget(ingTitle);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    for (int i = 0; i < m_ingredients.size(); ++i) {
        const QString &ing = m_ingredients.at(i);
        IngredientButton *btn = new IngredientButton(ing, ingWidget);
        btn->setMinimumSize(150, 40);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btn->setStyleSheet("QPushButton { font: 13px \"Microsoft YaHei\"; color: #2c3e50; background: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; padding: 5px; } QPushButton:hover { background: #3498db; color: white; }");
        m_ingredientButtons.append(btn);
        gridLayout->addWidget(btn, i / 3, i % 3);
    }
    ingLayout->addLayout(gridLayout);

    mainLayout->addWidget(ingWidget);

    QFrame *line5 = new QFrame(m_gameWidget);
    line5->setFrameShape(QFrame::HLine);
    line5->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line5);

    m_resultLabel = new QLabel("", m_gameWidget);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; padding: 10px; border-radius: 8px;");
    m_resultLabel->setMinimumHeight(40);
    mainLayout->addWidget(m_resultLabel);

    QFrame *line6 = new QFrame(m_gameWidget);
    line6->setFrameShape(QFrame::HLine);
    line6->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line6);

    QWidget *actionWidget = new QWidget(m_gameWidget);
    QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setSpacing(20);
    actionLayout->setAlignment(Qt::AlignCenter);

    m_startMakingBtn = new QPushButton("🍣 制作", actionWidget);
    m_startMakingBtn->setFixedSize(120, 50);
    m_startMakingBtn->setStyleSheet("QPushButton { font: 16px \"Microsoft YaHei\"; background: #27ae60; color: white; border: none; border-radius: 25px; } QPushButton:hover { background: #1e8449; }");
    actionLayout->addWidget(m_startMakingBtn);

    m_submitBtn = new QPushButton("📦 提交", actionWidget);
    m_submitBtn->setFixedSize(120, 50);
    m_submitBtn->setStyleSheet("QPushButton { font: 16px \"Microsoft YaHei\"; background: #3498db; color: white; border: none; border-radius: 25px; } QPushButton:hover { background: #2980b9; }");
    actionLayout->addWidget(m_submitBtn);

    m_clearBtn = new QPushButton("🗑️ 清空", actionWidget);
    m_clearBtn->setFixedSize(120, 50);
    m_clearBtn->setStyleSheet("QPushButton { font: 16px \"Microsoft YaHei\"; background: #95a5a6; color: white; border: none; border-radius: 25px; } QPushButton:hover { background: #7f8c8d; }");
    actionLayout->addWidget(m_clearBtn);

    mainLayout->addWidget(actionWidget);

    m_gameTimer = new QTimer(this);
    m_moveTimer = new QTimer(this);

    connect(m_startMakingBtn, &QPushButton::clicked, this, &MainWindow::onStartMaking);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearSelection);
    connect(m_gameTimer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    connect(m_submitBtn, &QPushButton::clicked, this, &MainWindow::onSubmitOrder);
    connect(m_moveTimer, &QTimer::timeout, this, &MainWindow::onMoveAnimation);

    generateCustomerOrder();
    m_gameTimer->start(1000);
}

void MainWindow::generateCustomerOrder() {
    QStringList fishIngredients;
    fishIngredients << "🍣 三文鱼" << "🍤 虾" << "🦀 蟹肉" << "🐟 金枪鱼" << "🦐 甜虾";
    int index = QRandomGenerator::global()->bounded(fishIngredients.size());
    m_customerOrder = fishIngredients.at(index);
    m_customerLabel->setText(m_customerOrder + "寿司");
}

void MainWindow::onIngredientButtonClicked() {
    if (m_gameOver) return;

    QStringList selected;
    int count = 0;

    for (int i = 0; i < m_ingredientButtons.size(); ++i) {
        IngredientButton *btn = m_ingredientButtons.at(i);
        if (btn->isSelected()) {
            if (count >= 3) {
                btn->setSelected(false);
                continue;
            }
            selected.append(btn->text());
            count++;
        }
    }

    m_resultLabel->setText(selected.isEmpty() ? "" : "已选: " + selected.join(" + "));
    m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: #34495e; background: #f8f9fa; padding: 10px; border-radius: 8px;");
}

bool MainWindow::checkRecipe() {
    if (!m_plateArea->canMakeSushi()) return false;
    return true;
}

QString MainWindow::getRecipeName() {
    QString fishText = m_plateArea->getFishText();
    if (fishText.contains("三文鱼")) return "三文鱼寿司";
    if (fishText.contains("虾") && !fishText.contains("甜虾")) return "虾寿司";
    if (fishText.contains("蟹肉")) return "蟹肉寿司";
    if (fishText.contains("金枪鱼")) return "金枪鱼寿司";
    if (fishText.contains("甜虾")) return "甜虾寿司";
    return "寿司";
}

int MainWindow::getSushiPrice() {
    QString fishText = m_plateArea->getFishText();
    if (fishText.contains("三文鱼")) return 30;
    if (fishText.contains("虾") && !fishText.contains("甜虾")) return 25;
    if (fishText.contains("蟹肉")) return 28;
    if (fishText.contains("金枪鱼")) return 35;
    if (fishText.contains("甜虾")) return 22;
    return 20;
}

bool MainWindow::matchCustomerOrder() {
    int spaceIndex = m_customerOrder.indexOf(" ");
    if (spaceIndex == -1) return false;
    QString orderFish = m_customerOrder.mid(spaceIndex + 1);
    return m_madeSushi.contains(orderFish);
}

void MainWindow::showGameOverDialog() {
    QString message = QString("今日完成订单：%1 个\n\n总营业额：%2 金币").arg(m_completedOrders).arg(m_totalEarned);
    QMessageBox::information(this, "🍣 游戏结束", message);
}

void MainWindow::onMoveAnimation() {
    if (m_plateArea) {
        int currentOffset = m_plateArea->getMoveOffset() + 10;
        m_plateArea->setMoveOffset(currentOffset);
        m_plateArea->update();
    }
}

void MainWindow::onStartMaking() {
    if (m_gameOver) {
        m_resultLabel->setText("游戏已结束");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #e74c3c; padding: 10px; border-radius: 8px;");
        return;
    }

    if (!m_plateArea->hasIngredients()) {
        m_resultLabel->setText("请先拖拽食材到寿司帘");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #e74c3c; padding: 10px; border-radius: 8px;");
        return;
    }

    if (!m_plateArea->canMakeSushi()) {
        m_resultLabel->setText("需要米饭+鱼肉两种食材");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #e74c3c; padding: 10px; border-radius: 8px;");
        return;
    }

    if (!m_madeSushi.isEmpty()) {
        m_resultLabel->setText("已制作过寿司，请提交或清空后再制作");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #f39c12; padding: 10px; border-radius: 8px;");
        return;
    }

    m_madeSushi = getRecipeName();
    m_sushiPrice = getSushiPrice();
    m_plateArea->setSushiMade(true);

    QString sushiIcon = "🍣";
    QString fishText = m_plateArea->getFishText();
    if (fishText.contains("三文鱼")) sushiIcon = "🍣";
    else if (fishText.contains("虾") && !fishText.contains("甜虾")) sushiIcon = "🍤";
    else if (fishText.contains("蟹肉")) sushiIcon = "🦀";
    else if (fishText.contains("金枪鱼")) sushiIcon = "🐟";
    else if (fishText.contains("甜虾")) sushiIcon = "🦐";

    m_resultLabel->setText("制作成功！" + sushiIcon + " " + m_madeSushi + "（待提交）");
    m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #3498db; padding: 10px; border-radius: 8px;");
}

void MainWindow::onClearSelection() {
    for (int i = 0; i < m_ingredientButtons.size(); ++i) {
        m_ingredientButtons.at(i)->setSelected(false);
    }
    if (m_plateArea) {
        m_plateArea->clear();
    }
    m_resultLabel->setText("");
    m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; padding: 10px; border-radius: 8px;");
    m_madeSushi = "";
    m_sushiPrice = 0;
    m_isSubmitting = false;
}

void MainWindow::onTimerTimeout() {
    m_timeLeft--;
    m_timeLabel->setText(QString("⏱️ %1 秒").arg(m_timeLeft));

    if (m_timeLeft <= 0) {
        m_gameTimer->stop();
        m_gameOver = true;
        m_timeLabel->setText("⏱️ 时间到！");
        m_resultLabel->setText("游戏结束！金币: " + QString::number(m_gold) + " 得分: " + QString::number(m_score));
        m_resultLabel->setStyleSheet("font: bold 16px \"Microsoft YaHei\"; color: white; background: #c0392b; padding: 12px; border-radius: 8px;");

        m_startMakingBtn->setEnabled(false);
        m_clearBtn->setEnabled(false);
        m_submitBtn->setEnabled(false);

        for (int i = 0; i < m_ingredientButtons.size(); ++i) {
            m_ingredientButtons.at(i)->setEnabled(false);
        }

        showGameOverDialog();
    }
}

void MainWindow::onSubmitOrder() {
    if (m_gameOver) {
        m_resultLabel->setText("游戏已结束");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #e74c3c; padding: 10px; border-radius: 8px;");
        return;
    }

    if (m_madeSushi.isEmpty()) {
        m_resultLabel->setText("请先制作寿司");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #e67e22; padding: 10px; border-radius: 8px;");
        return;
    }

    if (m_isSubmitting) {
        m_resultLabel->setText("正在上餐中，请稍候...");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #f39c12; padding: 10px; border-radius: 8px;");
        return;
    }

    if (matchCustomerOrder()) {
        m_isSubmitting = true;
        int bonusGold = 15;

        m_gold += m_sushiPrice + bonusGold;
        m_totalEarned += m_sushiPrice + bonusGold;
        m_score += 20;
        m_completedOrders++;

        m_goldLabel->setText(QString("💰 %1 金币").arg(m_gold));

        m_resultLabel->setText("交付成功！+" + QString::number(m_sushiPrice) + "金币（寿司）+" + QString::number(bonusGold) + "金币（奖励）！总计：" + QString::number(m_totalEarned) + "金币");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #27ae60; padding: 10px; border-radius: 8px;");

        m_plateArea->setMoving(true);
        m_moveTimer->start(30);

        QTimer::singleShot(1500, this, [=]() {
            m_moveTimer->stop();
            m_plateArea->setMoving(false);
            m_plateArea->setMoveOffset(0);
            m_madeSushi = "";
            m_sushiPrice = 0;
            m_isSubmitting = false;
            onClearSelection();
            generateCustomerOrder();
        });
    } else {
        m_resultLabel->setText("这不是顾客想要的！想要：" + m_customerOrder + "寿司");
        m_resultLabel->setStyleSheet("font: bold 14px \"Microsoft YaHei\"; color: white; background: #f39c12; padding: 10px; border-radius: 8px;");
    }
}
