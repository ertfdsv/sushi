#ifndef SUSHIITEM_H
#define SUSHIITEM_H

#include <QString>
#include <QDebug>

class SushiItem
{
protected:
    QString m_name;
    int m_score;
    QString m_imagePath;

public:
    SushiItem(const QString &name = "", int score = 0, const QString &path = "");
    virtual ~SushiItem();
    QString getName() const;
    int getScore() const;
    QString getImagePath() const;
    virtual void showInfo() const;
};

class Rice : public SushiItem
{
public:
    Rice();
    void showInfo() const override;
};

class Salmon : public SushiItem
{
public:
    Salmon();
    void showInfo() const override;
};

#endif
