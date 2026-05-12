#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QTimer>
#include <QWidget>

class PlateArea;

class IngredientButton : public QPushButton
{
    Q_OBJECT
public:
    IngredientButton(const QString &text, QWidget *parent = nullptr);
    bool isSelected() const;
    void setSelected(bool selected);
protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    bool m_selected;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onIngredientButtonClicked();
    void onStartMaking();
    void onClearSelection();
    void onTimerTimeout();
    void onSubmitOrder();
    void onStartGame();
    void onMoveAnimation();

private:
    void setupWelcomeUI();
    void setupGameUI();
    void generateCustomerOrder();
    bool checkRecipe();
    QString getRecipeName();
    int getSushiPrice();
    bool matchCustomerOrder();
    void showGameOverDialog();

    QStringList m_ingredients;
    QLabel *m_customerLabel;
    QList<IngredientButton*> m_ingredientButtons;
    QLabel *m_resultLabel;
    QPushButton *m_startMakingBtn;
    QPushButton *m_clearBtn;
    QPushButton *m_submitBtn;
    PlateArea *m_plateArea;
    QWidget *m_gameWidget;
    QTimer *m_moveTimer;

    int m_gold;
    int m_totalEarned;
    QLabel *m_goldLabel;

    QTimer *m_gameTimer;
    int m_timeLeft;
    QLabel *m_timeLabel;
    bool m_gameOver;

    QString m_customerOrder;
    QString m_madeSushi;
    int m_score;
    int m_completedOrders;
    int m_sushiPrice;
    bool m_isSubmitting;
};

#endif
