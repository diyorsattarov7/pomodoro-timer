#include <QTWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>

class PomodoroTimer : public QMainWindow
{
public:
    PomodoroTimer(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("Pomodoro Timer");
        setFixedSize(450, 400);
        setUpUI();
    }

    void setUpUI()
    {
        QWidget *centralWidget = new QWidget;
        setCentralWidget(centralWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(20, 20, 20, 20);

        statusLabel = new QLabel("Work Session");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet(
            "color: #e74c3c; font-weight: bold; font-size: 18px;");
        mainLayout->addWidget(statusLabel);

        timerLabel = new QLabel("25:00");
        timerLabel->setAlignment(Qt::AlignCenter);
        timerLabel->setStyleSheet("font-size: 72px; "
                                  "font-weight: bold; "
                                  "color: #2c3e50; "
                                  "background-color: #ecf0f1; "
                                  "border-radius: 20px; "
                                  "padding: 40px; ");
        mainLayout->addWidget(timerLabel);

        QHBoxLayout *buttonLayout = new QHBoxLayout;

        startStopBtn = new QPushButton("Start");
        startStopBtn->setFixedHeight(50);
        startStopBtn->setStyleSheet("QPushButton { "
                                    "background-color: #27ae60; "
                                    "color: white; "
                                    "border: none; "
                                    "border-radius: 8px; "
                                    "font-size: 16px; "
                                    "font-weight: bold; "
                                    "} "
                                    "QPushButton:hover { "
                                    "background-color: #229954;"
                                    "} "
                                    "QPushButton:pressed { "
                                    "background-color: #1e8449; "
                                    "} ");

        QPushButton *resetBtn = new QPushButton("Reset");
        resetBtn->setFixedHeight(50);
        resetBtn->setStyleSheet("QPushButton { "
                                "background-color: #e74c3c; "
                                "color: white; "
                                "border: none; "
                                "border-radius: 8px; "
                                "font-size: 16px; "
                                "font-weight: bold; "
                                "} "
                                "QPushButton:hover { "
                                "background-color: #c0392b; "
                                "} "
                                "QPushButton:pressed { "
                                "background-color: #a93226; "
                                "}");
        buttonLayout->addWidget(startStopBtn);
        buttonLayout->addWidget(resetBtn);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addStretch();
    }

private:
    QLabel *timerLabel;
    QLabel *statusLabel;
    QPushButton *startStopBtn;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PomodoroTimer window;
    window.show();

    return app.exec();
}
