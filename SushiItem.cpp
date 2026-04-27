#include "SushiItem.h"

SushiItem::SushiItem(const QString &name, int score, const QString &path)
    : m_name(name), m_score(score), m_imagePath(path)
{
}

SushiItem::~SushiItem()
{
}

QString SushiItem::getName() const
{
    return m_name;
}

int SushiItem::getScore() const
{
    return m_score;
}

QString SushiItem::getImagePath() const
{
    return m_imagePath;
}

void SushiItem::showInfo() const
{
    qDebug() << "食材:" << m_name << "分数:" << m_score << "图片:" << m_imagePath;
}

Rice::Rice() : SushiItem("🍚 米饭", 5, "images/rice.png")
{
}

void Rice::showInfo() const
{
    qDebug() << "===== 米饭 =====";
    qDebug() << "名称:" << getName();
    qDebug() << "分数:" << getScore() << "分";
    qDebug() << "图片路径:" << getImagePath();
    qDebug() << "描述: 寿司的基础食材";
}

Salmon::Salmon() : SushiItem("🍣 三文鱼", 15, "images/salmon.png")
{
}

void Salmon::showInfo() const
{
    qDebug() << "===== 三文鱼 =====";
    qDebug() << "名称:" << getName();
    qDebug() << "分数:" << getScore() << "分";
    qDebug() << "图片路径:" << getImagePath();
    qDebug() << "描述: 新鲜美味的刺身";
}
