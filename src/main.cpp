#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

class PomodoroTimer : public QMainWindow
{
public:
    PomodoroTimer(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("Pomodoro Timer");
        setFixedSize(450, 400);
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PomodoroTimer window;
    window.show();

    return app.exec();
}
