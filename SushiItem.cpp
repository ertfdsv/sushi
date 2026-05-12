#include "SushiItem.h"

SushiItem::SushiItem(qreal x, qreal y, qreal width, qreal height,
                     const QString &name, int score, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_x(x), m_y(y), m_width(width), m_height(height),
    m_name(name), m_score(score)
{
    setPos(x, y);
}

SushiItem::SushiItem() : QGraphicsItem(nullptr), m_x(0), m_y(0), m_width(0), m_height(0), m_name(""), m_score(0) {}

SushiItem::~SushiItem() {}

QRectF SushiItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void SushiItem::setPosition(qreal x, qreal y)
{
    m_x = x;
    m_y = y;
    setPos(x, y);
}

QPointF SushiItem::getPosition() const
{
    return QPointF(m_x, m_y);
}

QString SushiItem::getName() const
{
    return m_name;
}

int SushiItem::getScore() const
{
    return m_score;
}

void SushiItem::showInfo() const
{
    qDebug() << "名称:" << m_name << "分数:" << m_score;
}

Rice::Rice(qreal x, qreal y, QGraphicsItem *parent)
    : SushiItem(x, y, 80, 20, "🍚 米饭", 5, parent) {}

Rice::Rice() : SushiItem(0, 0, 80, 20, "🍚 米饭", 5) {}

void Rice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QColor(255, 255, 240));
    painter->setPen(QColor(200, 200, 190));
    painter->drawEllipse(0, 0, m_width, m_height);

    painter->setPen(QColor(220, 220, 210));
    for (int i = 0; i < 5; i++) {
        painter->drawLine(10 + i*15, 5, 12 + i*15, 15);
    }
}

void Rice::showInfo() const
{
    qDebug() << "===== 米饭 =====";
    qDebug() << "名称:" << m_name;
    qDebug() << "分数:" << m_score;
    qDebug() << "图片路径: images/rice.png";
    qDebug() << "描述: 寿司的基础食材，软糯可口";
}

Salmon::Salmon(qreal x, qreal y, QGraphicsItem *parent)
    : SushiItem(x, y, 70, 15, "🍣 三文鱼", 15, parent) {}

Salmon::Salmon() : SushiItem(0, 0, 70, 15, "🍣 三文鱼", 15) {}

void Salmon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QLinearGradient gradient(0, 0, 0, m_height);
    gradient.setColorAt(0, QColor(255, 140, 120));
    gradient.setColorAt(1, QColor(255, 100, 80));
    painter->setBrush(gradient);
    painter->setPen(QColor(200, 80, 60));
    painter->drawEllipse(0, 0, m_width, m_height);

    painter->setBrush(QColor(255, 180, 160));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(5, 3, 20, 6);
}

void Salmon::showInfo() const
{
    qDebug() << "===== 三文鱼 =====";
    qDebug() << "名称:" << m_name;
    qDebug() << "分数:" << m_score;
    qDebug() << "图片路径: images/salmon.png";
    qDebug() << "描述: 新鲜美味的刺身，口感细腻";
}

Cucumber::Cucumber(qreal x, qreal y, QGraphicsItem *parent)
    : SushiItem(x, y, 60, 12, "🥒 黄瓜", 8, parent) {}

Cucumber::Cucumber() : SushiItem(0, 0, 60, 12, "🥒 黄瓜", 8) {}

void Cucumber::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QLinearGradient gradient(0, 0, m_width, 0);
    gradient.setColorAt(0, QColor(80, 180, 80));
    gradient.setColorAt(0.5, QColor(100, 200, 100));
    gradient.setColorAt(1, QColor(80, 180, 80));
    painter->setBrush(gradient);
    painter->setPen(QColor(50, 120, 50));
    painter->drawRoundedRect(0, 0, m_width, m_height, 6, 6);

    painter->setPen(QColor(60, 150, 60));
    painter->drawLine(m_width/2, 0, m_width/2, m_height);
}

void Cucumber::showInfo() const
{
    qDebug() << "===== 黄瓜 =====";
    qDebug() << "名称:" << m_name;
    qDebug() << "分数:" << m_score;
    qDebug() << "图片路径: images/cucumber.png";
    qDebug() << "描述: 清爽脆嫩的蔬菜";
}

Nori::Nori(qreal x, qreal y, QGraphicsItem *parent)
    : SushiItem(x, y, 90, 30, "🍘 海苔", 3, parent) {}

Nori::Nori() : SushiItem(0, 0, 90, 30, "🍘 海苔", 3) {}

void Nori::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QColor(10, 30, 10));
    painter->setPen(QColor(20, 50, 20));
    painter->drawRect(0, 0, m_width, m_height);

    painter->setPen(QColor(30, 60, 30));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            painter->drawPoint(10 + i*30, 5 + j*6);
        }
    }
}

void Nori::showInfo() const
{
    qDebug() << "===== 海苔 =====";
    qDebug() << "名称:" << m_name;
    qDebug() << "分数:" << m_score;
    qDebug() << "图片路径: images/nori.png";
    qDebug() << "描述: 包裹寿司的紫菜";
}
