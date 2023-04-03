#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <ui/loadingwidget.hpp>
#include <ui/mainview.hpp>
#include <core/workerthread.hpp>
#include <core/news.hpp>
#include <QSoundEffect>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QSystemTrayIcon *trayIcon;
    LoadingWidget * loadingWidget;
    MainView * mainView;
    QSoundEffect audioNotifier;
    WorkerThread backgroundWorker;


public slots:
    void onProcedureFinished();
    void addNewsToConfig(const QMap<QString,News> & new_news);

private slots:
    void playAudioNotification();
    void openApplication();
    void quitApplication();

signals:
    void checkRequireDirs();
    void newArticleAvailable();
};

#endif