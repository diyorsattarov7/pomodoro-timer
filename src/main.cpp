#include <QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

class PomodoroTimer : public QMainWindow
{
    Q_OBJECT
public:
    PomodoroTimer(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("Pomodoro Timer");
        setFixedSize(450, 400);
        setUpUI();
        setupTimer();
        resetTimer();
    }

private slots:
    void toggleTimer()
    {
        if (timer->isActive())
        {
            timer->stop();
            startStopBtn->setText("Start");
        }
        else
        {
            timer->start(1000);
            startStopBtn->setText("Pause");
        }
    }

    void updateDisplay()
    {
        remainingTime--;

        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        timerLabel->setText(QString("%1:%2")
                                .arg(minutes, 2, 10, QChar('0'))
                                .arg(seconds, 2, 10, QChar('0')));

        if (remainingTime <= 0)
        {
            timer->stop();
            onTimerFinished();
        }
    }

    void onTimerFinished()
    {
        isWorkSession = !isWorkSession;
        statusLabel->setText(isWorkSession ? "Work Session" : "Break Time");
        statusLabel->setStyleSheet(
            isWorkSession
                ? "color: #e74c3c; font-weight: bold; font-size: 18px;"
                : "color: #27ae60; font-weight: bold; font-size: 18px;");

        resetTimer();
        startStopBtn->setText("Start");
    }

    void resetTimer()
    {
        timer->stop();
        remainingTime = isWorkSession ? workDuration * 60 : breakDuration * 60;

        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        timerLabel->setText(QString("%1:%2")
                                .arg(minutes, 2, 10, QChar('0'))
                                .arg(seconds, 2, 10, QChar('0')));

        startStopBtn->setText("Start");
    }

private:
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

        connect(startStopBtn, &QPushButton::clicked, this,
                &PomodoroTimer::toggleTimer);

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

        connect(resetBtn, &QPushButton::clicked, this,
                &PomodoroTimer::resetTimer);

        buttonLayout->addWidget(startStopBtn);
        buttonLayout->addWidget(resetBtn);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addStretch();
    }

    void setupTimer()
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &PomodoroTimer::updateDisplay);
    }

    QTimer *timer;
    QLabel *timerLabel;
    QLabel *statusLabel;
    QPushButton *startStopBtn;

    int workDuration = 25;
    int breakDuration = 5;
    int remainingTime = 0;
    bool isWorkSession = true;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PomodoroTimer window;
    window.show();

    return app.exec();
}

#include "main.moc"
