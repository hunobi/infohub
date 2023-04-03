#include<ui/loadingwidget.hpp>
#include<core/config.hpp>
#include<core/utils.hpp>
#include<QThread>
#include<QDir>
#include<QDirIterator>

LoadingWidget::LoadingWidget(QWidget *parent){
    // Set loading layout
    layout = new QVBoxLayout(this);

    // tworzymy progress bar
    progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setStyleSheet("color: white;");
    
    layout->addWidget(progressBar,1);
    QTimer::singleShot(500, this, &LoadingWidget::checkRequiredFiles);
}

void LoadingWidget::checkRequiredFiles(){
    // load config singleton
    Config & cfg = ConfigSingleton::getInstance().GetConfig();
    QList<QString> dirs = {"cache","assets","audio","lang"};
    auto categoryMap = cfg.getCategories();
    auto newsMap = cfg.getNews();
    auto srcMap = cfg.getSources();
    QStringList keys_cat = categoryMap.keys();
    QStringList keys_news = newsMap.keys();
    QStringList files_path;
    QDirIterator it("./cache", QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) files_path.append(it.next());

    uint maxTasks = dirs.size()+keys_cat.size()+keys_news.size()+files_path.count();
    uint iter = 0; 
    this->progress = 0;
    int i = 0;

    // create missing dirs
    for(i=0 ; i<dirs.size(); i++){
        QString dirname = dirs.at(i);
        QDir dir;
        if(!dir.exists(dirname)){
            dir.mkpath(dirname);
        }
        iter++;
        this->updateProgress(iter,maxTasks);
    }
    

    /*
        Check categories icon. Set default if icon does not exist.
    */
    for(i = 0; i<keys_cat.size(); i++){
        Category tmp = categoryMap.find(keys_cat.at(i)).value();
        QFile file(tmp.getIconPath());
        if(!file.exists()){
            tmp.setIconPath(":/icons/default-category.png");
            if(cfg.getCategories().contains(tmp.getName())){
                cfg.getCategories()[tmp.getName()].setIconPath(tmp.getIconPath());
            }
        }
        iter++;
        this->updateProgress(iter,maxTasks);
    }


    qint64 days = cfg.getSettings().getNewsAutoRemoveDays();

    qint64 time_limit = days*86400; // 1 day = 86400 sec


    /*
        Remove old news if autoremove options is enable
    */
    if(cfg.getSettings().getNewsAutoRemove()){
        qint64 current_time = getTimeSec();
        for(i=0; i<keys_news.size();i++){
            News tmp_news = newsMap.value(keys_news.at(i));
        
            if(tmp_news.isSaved()){  // if news is saved then skip this news and continue work...
                iter++;
                this->updateProgress(iter,maxTasks);
                continue;
            }
            
            qint64 news_life = tmp_news.getTimestamp();
            if(current_time > news_life+time_limit){
                if(cfg.getNews().contains(keys_news.at(i))){
                    cfg.getNews().remove(keys_news.at(i));
                }
            }
            iter++;
            this->updateProgress(iter,maxTasks);
        }
        
    }else{
        iter += keys_news.size();
        this->updateProgress(iter,maxTasks);
    }

    // Scan cache and remove old files
    // Remove image if only file is not
    for(QString filepath : files_path){
        QString filename = filepath.section("/",-1,-1);
        if(!newsMap.contains(filename)){
            if(QFile::exists(filepath)){
                QFile::remove(filepath);
            }
        }
        iter++;
        this->updateProgress(iter,maxTasks);
    }

    ConfigSingleton::getInstance().Save();
    
    emit procedureFinished();
}

void LoadingWidget::updateProgress(uint i, uint max)
{
    this->progress = (int)((100*i)/(max));
    if (progress > 100) {
        progressBar->setValue(100);
        return;
    }
    progressBar->setValue(progress);
}

LoadingWidget::~LoadingWidget(){
    delete layout;
    delete progressBar;
}