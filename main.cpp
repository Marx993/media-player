#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSlider>
#include <QPushButton>
#include <QFileDialog>
#include <QStatusBar>
#include <QStyleFactory>
#include <QPalette>
#include <QKeyEvent>
#include <QWindow>

#include <vlc/vlc.h>

class RetroVLC : public QMainWindow {
    Q_OBJECT

public:
    RetroVLC() {
        setWindowTitle("Media Player");
        resize(800, 500);

        isFullscreen = false;

        // ===== TOP BAR =====
        QToolBar *topBar = addToolBar("Top");
        topBar->addAction("Open", this, &RetroVLC::openFile);
        topBar->addAction("Play", this, &RetroVLC::play);
        topBar->addAction("Pause", this, &RetroVLC::pause);
        topBar->addAction("Stop", this, &RetroVLC::stop);

        // ===== VIDEO AREA =====
        videoFrame = new QWidget();
        videoFrame->setStyleSheet("background:black;");
        videoFrame->setMinimumHeight(300);

        QWidget *central = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(central);
        layout->addWidget(videoFrame);

        // ===== VLC INIT =====
        const char *vlc_args[] = { "--no-xlib" };

        vlcInstance = libvlc_new(1, vlc_args);
        vlcPlayer = libvlc_media_player_new(vlcInstance);

        attachVLC();

        // ===== BOTTOM BAR =====
        QWidget *bottom = new QWidget();
        QHBoxLayout *b = new QHBoxLayout(bottom);

        playBtn = new QPushButton("Play");
        pauseBtn = new QPushButton("Pause");
        stopBtn = new QPushButton("Stop");

        connect(playBtn, &QPushButton::clicked, this, &RetroVLC::play);
        connect(pauseBtn, &QPushButton::clicked, this, &RetroVLC::pause);
        connect(stopBtn, &QPushButton::clicked, this, &RetroVLC::stop);

        seek = new QSlider(Qt::Horizontal);
        seek->setRange(0, 1000);

        volume = new QSlider(Qt::Horizontal);
        volume->setRange(0, 100);
        volume->setValue(80);

        connect(volume, &QSlider::valueChanged, this, &RetroVLC::setVolume);
        connect(seek, &QSlider::sliderMoved, this, &RetroVLC::setPosition);

        b->addWidget(playBtn);
        b->addWidget(pauseBtn);
        b->addWidget(stopBtn);
        b->addWidget(seek);
        b->addWidget(volume);

        layout->addWidget(bottom);
        setCentralWidget(central);

        statusBar()->showMessage("Ready");
    }

    ~RetroVLC() {
        libvlc_media_player_release(vlcPlayer);
        libvlc_release(vlcInstance);
    }

private:
    QWidget *videoFrame;
    QSlider *seek;
    QSlider *volume;
    QPushButton *playBtn;
    QPushButton *pauseBtn;
    QPushButton *stopBtn;

    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;

    bool isFullscreen;
    QWidget *centralBackup = nullptr;

    // IMPORTANT: reattach VLC video
    void attachVLC() {
#if defined(Q_OS_WIN)
        libvlc_media_player_set_hwnd(vlcPlayer, (void*)videoFrame->winId());
#else
        libvlc_media_player_set_xwindow(vlcPlayer, (uint32_t)videoFrame->winId());
#endif
    }

public slots:
    void openFile() {
        QString file = QFileDialog::getOpenFileName(this, "Open Video");
        if (file.isEmpty()) return;

        libvlc_media_t *media =
            libvlc_media_new_path(vlcInstance, file.toStdString().c_str());

        libvlc_media_player_set_media(vlcPlayer, media);
        libvlc_media_release(media);

        libvlc_media_player_play(vlcPlayer);
    }

    void play() { libvlc_media_player_play(vlcPlayer); }
    void pause() { libvlc_media_player_pause(vlcPlayer); }
    void stop() { libvlc_media_player_stop(vlcPlayer); }

    void setVolume(int v) {
        libvlc_audio_set_volume(vlcPlayer, v);
    }

    void setPosition(int p) {
        libvlc_media_player_set_position(vlcPlayer, p / 1000.0);
    }

    // ===== REAL VIDEO FULLSCREEN MODE =====
    void enterVideoFullscreen() {
        centralBackup = centralWidget();

        videoFrame->setParent(nullptr);
        videoFrame->showFullScreen();

        attachVLC();

        isFullscreen = true;
    }

    void exitVideoFullscreen() {
        videoFrame->setParent(centralBackup);

        QVBoxLayout *layout = (QVBoxLayout*)centralBackup->layout();
        layout->insertWidget(0, videoFrame);

        showNormal();
        attachVLC();

        isFullscreen = false;
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_F) {
            if (!isFullscreen) enterVideoFullscreen();
            else exitVideoFullscreen();
        }

        if (event->key() == Qt::Key_Escape && isFullscreen) {
            exitVideoFullscreen();
        }

        QMainWindow::keyPressEvent(event);
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QApplication::setStyle("Windows");

    QPalette p;
    p.setColor(QPalette::Window, QColor("#C0C0C0"));
    p.setColor(QPalette::Base, QColor("#FFFFFF"));
    p.setColor(QPalette::Button, QColor("#C0C0C0"));
    p.setColor(QPalette::Light, Qt::white);
    p.setColor(QPalette::Dark, QColor("#404040"));
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::ButtonText, Qt::black);

    QApplication::setPalette(p);

    RetroVLC w;
    w.show();

    return app.exec();
}
