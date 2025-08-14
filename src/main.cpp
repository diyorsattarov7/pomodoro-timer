// clang-format off
#include <QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <chrono>

using namespace std::chrono_literals;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(int workMinutes, int breakMinutes, QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle(tr("Pomodoro Settings"));

        auto *layout = new QFormLayout(this);

        workSpin_ = new QSpinBox(this);
        breakSpin_ = new QSpinBox(this);
        workSpin_->setRange(1, 120);
        breakSpin_->setRange(1, 60);
        workSpin_->setSuffix(tr(" min"));
        breakSpin_->setSuffix(tr(" min"));
        workSpin_->setValue(workMinutes);
        breakSpin_->setValue(breakMinutes);

        layout->addRow(tr("Work duration:"), workSpin_);
        layout->addRow(tr("Break duration:"), breakSpin_);

        auto *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(box);
    }

    [[nodiscard]] int workMinutes() const { return workSpin_->value(); }
    [[nodiscard]] int breakMinutes() const { return breakSpin_->value(); }

private:
    QSpinBox *workSpin_{}, *breakSpin_{};
};

class PomodoroTimer : public QMainWindow
{
    Q_OBJECT
public:
    PomodoroTimer(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle(tr("Pomodoro Timer"));
        buildUi();
        restoreSettings();
        connect(&timer_, &QTimer::timeout, this, &PomodoroTimer::onTick);
        updateUi();
    }

    ~PomodoroTimer() override { saveSettings(); }

private:
    enum class Phase
    {
        Work,
        Break
    };

    // ----- UI -----
    QLabel *statusLabel_{};
    QLabel *timerLabel_{};
    QPushButton *startPause_{};
    QAction *actStartPause_{};
    QAction *actReset_{};
    QAction *actSettings_{};

    // ----- State -----
    Phase phase_{Phase::Work};
    int workSecs_{25 * 60};
    int breakSecs_{5 * 60};
    qint64 targetMs_{0};
    bool running_{false};
    QTimer timer_;
    QElapsedTimer monotonic_;

    // ----- UI construction -----
    void buildUi()
    {
        auto *central = new QWidget(this);
        auto *main = new QVBoxLayout(central);
        main->setSpacing(15);
        main->setContentsMargins(20, 20, 20, 20);

        auto *top = new QHBoxLayout;
        top->addStretch();
        auto *settingsBtn = new QPushButton(QStringLiteral("⚙"), central);
        settingsBtn->setFixedSize(30, 30);
        settingsBtn->setToolTip(tr("Settings"));
        settingsBtn->setCursor(Qt::PointingHandCursor);
        settingsBtn->setStyleSheet("QPushButton{border:none;background:#ecf0f1;"
                                   "border-radius:15px;font-size:16px}"
                                   "QPushButton:hover{background:#bdc3c7}");
        connect(settingsBtn, &QPushButton::clicked, this, &PomodoroTimer::openSettings);
        top->addWidget(settingsBtn);
        main->addLayout(top);

        statusLabel_ = new QLabel(central);
        statusLabel_->setAlignment(Qt::AlignCenter);
        statusLabel_->setStyleSheet("font-weight:bold;font-size:18px");
        main->addWidget(statusLabel_);

        timerLabel_ = new QLabel(central);
        timerLabel_->setAlignment(Qt::AlignCenter);
        timerLabel_->setStyleSheet(
            "font-size:72px;font-weight:bold;color:#2c3e50;"
            "background:#ecf0f1;border-radius:20px;padding:40px");
        main->addWidget(timerLabel_);

        auto *buttons = new QHBoxLayout;

        startPause_ = new QPushButton(tr("Start"), central);
        startPause_->setFixedHeight(50);
        startPause_->setStyleSheet(
            "QPushButton{background:#27ae60;color:white;border:none;border-"
            "radius:8px;font-size:16px;font-weight:bold}"
            "QPushButton:hover{background:#229954}"
            "QPushButton:pressed{background:#1e8449}");
        connect(startPause_, &QPushButton::clicked, this, &PomodoroTimer::toggle);

        auto *reset = new QPushButton(tr("Reset"), central);
        reset->setFixedHeight(50);
        reset->setStyleSheet(
            "QPushButton{background:#e74c3c;color:white;border:none;border-"
            "radius:8px;font-size:16px;font-weight:bold}"
            "QPushButton:hover{background:#c0392b}"
            "QPushButton:pressed{background:#a93226}");
        connect(reset, &QPushButton::clicked, this, &PomodoroTimer::reset);

        buttons->addWidget(startPause_);
        buttons->addWidget(reset);
        main->addLayout(buttons);
        main->addStretch();

        setCentralWidget(central);

        actStartPause_ = new QAction(tr("Start/Pause"), this);
        actReset_ = new QAction(tr("Reset"), this);
        actSettings_ = new QAction(tr("Settings…"), this);
        actSettings_->setShortcut(QKeySequence::Preferences);
        addAction(actStartPause_);
        addAction(actReset_);
        addAction(actSettings_);
        actStartPause_->setShortcut(Qt::Key_Space);
        actReset_->setShortcut(Qt::Key_R);
        connect(actStartPause_, &QAction::triggered, this, &PomodoroTimer::toggle);
        connect(actReset_, &QAction::triggered, this, &PomodoroTimer::reset);
        connect(actSettings_, &QAction::triggered, this, &PomodoroTimer::openSettings);
    }

    // ----- Behavior -----
    void start()
    {
        running_ = true;
        timer_.start(1s);
        if (targetMs_ <= 0) setTargetFromNow(remainingForPhase());
        startPause_->setText(tr("Pause"));
    }

    void pause()
    {
        running_ = false;
        timer_.stop();
        auto rem = remainingMs();
        targetMs_ = QDateTime::currentMSecsSinceEpoch() + rem;
        startPause_->setText(tr("Start"));
        updateUi();
    }

    void toggle() { running_ ? pause() : start(); }

    void reset()
    {
        timer_.stop();
        running_ = false;
        targetMs_ = 0;
        startPause_->setText(tr("Start"));
        updateUi(true);
    }

    void openSettings()
    {
        SettingsDialog dlg(workSecs_ / 60, breakSecs_ / 60, this);
        if (dlg.exec() == QDialog::Accepted)
        {
            workSecs_ = dlg.workMinutes() * 60;
            breakSecs_ = dlg.breakMinutes() * 60;
            reset();
        }
    }

    void onTick()
    {
        if (remainingMs() <= 0)
        {
            timer_.stop();
            running_ = false;
            switchPhase();
            updateUi(true);
            startPause_->setText(tr("Start"));
        }
        else
        {
            updateUi();
        }
    }

    void switchPhase()
    {
        phase_ = (phase_ == Phase::Work) ? Phase::Break : Phase::Work;
        targetMs_ = 0;
    }

    int remainingForPhase() const
    {
        return (phase_ == Phase::Work) ? workSecs_ : breakSecs_;
    }

    qint64 remainingMs() const
    {
        if (targetMs_ <= 0) return remainingForPhase() * 1000LL;
        const qint64 now = QDateTime::currentMSecsSinceEpoch();
        return std::max<qint64>(0, targetMs_ - now);
    }

    void setTargetFromNow(int seconds)
    {
        targetMs_ = QDateTime::currentMSecsSinceEpoch() + qint64(seconds) * 1000;
        monotonic_.restart();
    }

    static QString formatSecs(int secs)
    {
        if (secs < 0) secs = 0;
        return QTime(0, 0).addSecs(secs).toString("mm:ss");
    }

    void updateUi(bool resetPhaseToFull = false)
    {
        if (resetPhaseToFull) setTargetFromNow(remainingForPhase());
        const int secs = int((remainingMs() + 500) / 1000);
        timerLabel_->setText(formatSecs(secs));

        const bool isWork = (phase_ == Phase::Work);
        statusLabel_->setText(isWork ? tr("Work Session") : tr("Break Time"));
        statusLabel_->setStyleSheet(
            isWork ? "color:#e74c3c;font-weight:bold;font-size:18px"
                   : "color:#27ae60;font-weight:bold;font-size:18px");
    }

    // ----- Settings persistence -----
    void restoreSettings()
    {
        QSettings s("YourOrg", "PomodoroQt");
        workSecs_ = s.value("workSecs", 25 * 60).toInt();
        breakSecs_ = s.value("breakSecs", 5 * 60).toInt();
        restoreGeometry(s.value("geom").toByteArray());
    }

    void saveSettings()
    {
        QSettings s("YourOrg", "PomodoroQt");
        s.setValue("workSecs", workSecs_);
        s.setValue("breakSecs", breakSecs_);
        s.setValue("geom", saveGeometry());
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PomodoroTimer window;
    window.show();

    return app.exec();
}

#include "main.moc"
