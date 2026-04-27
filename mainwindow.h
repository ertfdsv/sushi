#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QDrag>
#include <QMimeData>

class IngredientButton : public QPushButton
{
    Q_OBJECT
public:
    IngredientButton(const QString &text, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class PlateArea : public QWidget
{
    Q_OBJECT
public:
    PlateArea(QWidget *parent = nullptr);
    QString currentIngredient() const;
    void clear();
signals:
    void ingredientDropped(const QString &text);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    QString m_current;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onGenerateOrder();
    void onIngredientDropped(const QString &text);
    void onAddRiceClicked();
    void onAddSalmonClicked();
    void assembleSushi();

private:
    void setupUI();
    void generateOrder();
    void updateIngredientDisplay();

    QStringList m_ingredients;
    QLabel *m_orderLabel;
    QPushButton *m_generateBtn;
    PlateArea *m_plateArea;
    int m_currentOrderIndex;
    int m_score;

    QPushButton *m_addRiceBtn;
    QPushButton *m_addSalmonBtn;
    QLabel *m_currentIngredients;
    int m_totalScore;
    QStringList m_assembledIngredients;
};

#endif
