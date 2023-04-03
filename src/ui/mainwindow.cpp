#include <ui/mainwindow.hpp>
#include <core/config.hpp>
#include <QApplication>
#include <QScreen>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    // Set application icon
    QIcon appIcon(":/icons/app-icon.png");
    QApplication::setWindowIcon(appIcon);
    // Set window title
    setWindowTitle(tr("InfoHub"));
    setStyleSheet("QWidget {background-color: #202729}");
    // Create system tray icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(appIcon);
    trayIcon->setToolTip(tr("InfoHub"));
    trayIcon->setVisible(true);

    // Add menu to tray icon
    QMenu *menu = new QMenu(this);
    menu->addAction(tr("Open"), this, SLOT(openApplication()));
    menu->addAction(tr("Quit"), this, SLOT(quitApplication()));
    trayIcon->setContextMenu(menu);

    // Center window on screen
    setGeometry(0,0,1200,768);
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    setGeometry(x,y,width(),height());
    
    // Load and check all resources, dirs, files
    loadingWidget= new LoadingWidget();
    
    setCentralWidget(loadingWidget);

    connect(loadingWidget, &LoadingWidget::procedureFinished, this, &MainWindow::onProcedureFinished);
    connect(&this->backgroundWorker, &WorkerThread::updatedNewsWithContent, this, &MainWindow::addNewsToConfig);
    connect(&this->backgroundWorker, &WorkerThread::updatedNewsWithContent, this, &MainWindow::playAudioNotification);
}


/*
    When loadingWidget has finished its work, MainWindow will change its view.
*/
void MainWindow::onProcedureFinished(){
    // run background worker
    this->backgroundWorker.start();
    mainView = new MainView();
    connect(this, &MainWindow::newArticleAvailable, this->mainView, &MainView::newArticleAvailable);
    setCentralWidget(mainView);
    delete loadingWidget;
}

/*
    Event Handler - Play audio notification when a new article is available
*/
void MainWindow::playAudioNotification(){
    if(ConfigSingleton::getInstance().GetConfig().getSettings().getNewsAudioNotification()){
        this->audioNotifier.setVolume(ConfigSingleton::getInstance().GetConfig().getSettings().getNewsAudioNotificationVolume());
        QString path = ConfigSingleton::getInstance().GetConfig().getSettings().getNewsAudioNotificationPath();
        this->audioNotifier.setSource(QUrl::fromLocalFile(path));
        audioNotifier.play();
    }
}


/*
    Add new and unique News objects to Config.json
*/
void MainWindow::addNewsToConfig(const QMap<QString,News> & new_news){
    qDebug() << "Worker finished work.";
    Config & cfg = ConfigSingleton::getInstance().GetConfig();
    QStringList tmpKeys;
    /*
        Scan unique keys
    */
    for(QString key : new_news.keys()){
        if(!cfg.getNews().contains(key)){
            tmpKeys.append(key);
        }
    }
    
    qDebug() << "New articles:\t" <<  tmpKeys.size();

    /*
        add new News objects to memory
    */
    for(QString key : tmpKeys){
        cfg.getNews().insert(key, new_news.value(key));
    }

    // save to disk
    ConfigSingleton::getInstance().Save();
    emit newArticleAvailable();
}  

/*

    Check config.json and configure close window event.
    If "general_minimize_after_exit" : true, appliaction will minimize to tray icon.
    Else: apllication will quit.

*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    bool value = ConfigSingleton::getInstance().GetConfig().getSettings().getGeneralMinimizeAfterExit();
    if(value){
        if (trayIcon->isVisible()) {
            hide();
            event->ignore();
        } else {
            event->accept();
        }
    }else{
        this->backgroundWorker.setThreadRunning(false);
        this->backgroundWorker.wait();
        quitApplication();
    }
}

void MainWindow::openApplication()
{
    show();
    activateWindow();
}

/*
    Quit from app but first wait to end worker
*/
void MainWindow::quitApplication()
{
    this->backgroundWorker.setThreadRunning(false);
    this->backgroundWorker.wait();
    qApp->quit();
}
