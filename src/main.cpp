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

        timerLabel = new QLabel("25:00");
        timerLabel->setAlignment(Qt::AlignCenter);
        timerLabel->setStyleSheet("font-size: 72px; "
                                  "font-weight: bold; "
                                  "color: #2c3e50; "
                                  "background-color: #ecf0f1; "
                                  "border-radius: 20px; "
                                  "padding: 40px; ");
        mainLayout->addWidget(timerLabel);
        mainLayout->addStretch();
    }

private:
    QLabel *timerLabel;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PomodoroTimer window;
    window.show();

    return app.exec();
}
