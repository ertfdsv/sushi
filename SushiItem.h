#ifndef SUSHIITEM_H
#define SUSHIITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include <QDebug>

class SushiItem : public QGraphicsItem
{
protected:
    qreal m_x, m_y;
    qreal m_width, m_height;
    QString m_name;
    int m_score;

public:
    SushiItem(qreal x, qreal y, qreal width, qreal height,
              const QString &name = "", int score = 0, QGraphicsItem *parent = nullptr);
    SushiItem();
    virtual ~SushiItem();

    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override = 0;

    void setPosition(qreal x, qreal y);
    QPointF getPosition() const;
    QString getName() const;
    int getScore() const;

    virtual void showInfo() const;
};

class Rice : public SushiItem
{
public:
    Rice(qreal x, qreal y, QGraphicsItem *parent = nullptr);
    Rice();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void showInfo() const override;
};

class Salmon : public SushiItem
{
public:
    Salmon(qreal x, qreal y, QGraphicsItem *parent = nullptr);
    Salmon();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void showInfo() const override;
};

class Cucumber : public SushiItem
{
public:
    Cucumber(qreal x, qreal y, QGraphicsItem *parent = nullptr);
    Cucumber();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void showInfo() const override;
};

class Nori : public SushiItem
{
public:
    Nori(qreal x, qreal y, QGraphicsItem *parent = nullptr);
    Nori();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void showInfo() const override;
};

#endif
