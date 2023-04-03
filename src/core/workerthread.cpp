#include<core/workerthread.hpp>
#include<core/config.hpp>
#include<core/utils.hpp>

#include<QThreadPool>
#include<QJsonObject>
#include<QJsonArray>
#include<QFile>
#include<QDir>
#include <QEventLoop>

WorkerThread::WorkerThread(){
    this->isThreadRunning = true;
    this->firstTime = false;
    this->timestamp_last_load_news = 0;
}

/*
    Main loop of Worker thread.
*/
void WorkerThread::run(){
    while(this->isThreadRunning){
        // get values from settings
        bool news_auto_refresh = ConfigSingleton::getInstance().GetConfig().getSettings().getNewsAutoRefresh();
        int news_auto_refresh_period = ConfigSingleton::getInstance().GetConfig().getSettings().getNewsAutoRefreshPeriod();
        
        /* 
            Check for new news from time to time.
            news_auto_refresh_period returns minutes, so we have to convert it to seconds

            firstTime is mechanism for situation when news_auto_refresh is false and user starts program
            When news_auto_refresh is false without firstTime app will never update news.
        */
        if(!this->firstTime || (news_auto_refresh && getTimeSec()-(this->timestamp_last_load_news + (news_auto_refresh_period*60))>0)){
            this->load_news();
            this->timestamp_last_load_news = getTimeSec();
            if(!this->firstTime){this->firstTime = true;}
        }
        // You are very tired, go to sleep
        this->sleep(1); //seconds
    }
}

/*

    This method creates tasks for every Source object and execute them.
    Every task adds to local QMap new News object.
    Finally this method emit two event to tell them that new News are available.

*/
void WorkerThread::load_news(){
    QThreadPool *threadPool = QThreadPool::globalInstance();
    QMap<QString, Source> srcMap = ConfigSingleton::getInstance().GetConfig().getSources();
    QList<QString> keys = srcMap.keys();

    // list of pointers, becouse i want to clear memory later.
    for (const QString &key : keys) {
        if(srcMap.value(key).isEnabled()){ // load news from source only if source is enabled
            LoadNewsTask *task = new LoadNewsTask(srcMap.value(key), this);
            threadPool->start(task);
        }
    }

    // wait for finish threadPool
    threadPool->waitForDone();

    if(!this->news_from_tasks.empty()){
        // first emit to main proces to update Config
        emit updatedNewsWithContent(this->news_from_tasks);
        // now i can sent info to other objects that Config has new News
        emit newsHasBeenUpdated();
    }
    // clear local memory of News object
    this->news_from_tasks.clear();
}

/*
    Get news list from sources and check unique
*/
void WorkerThread::updateNewsList(const QList<News> & LNews){
    auto nMap = ConfigSingleton::getInstance().GetConfig().getNews();
    for(auto item : LNews){
        if(!this->news_from_tasks.contains(item.getHash()) && !nMap.contains(item.getHash())){
            this->news_from_tasks.insert(item.getHash(), item);
        }
    }
}



/*
    This is a main procedure of download Cache and News info
*/
void LoadNewsTask::run(){
    QJsonArray arr = getNewsArrayFromSource(this->source.getAddress());
    Config cfg = ConfigSingleton::getInstance().GetConfig();
    QList<News> new_news_objects;
    for(auto item : arr){
        /*
            If user want to remove old news, procedure will skip old news.
            Creating News object causes computing SHA256.
            My idea is to compute lifetime from raw timestamp of JsonObject. This way is faster.
            86400 - this is 1day in seconds
        */
        if(cfg.getSettings().getNewsAutoRemove() && (getTimeSec()-item.toObject().value("timestamp").toInteger() > cfg.getSettings().getNewsAutoRemoveDays()*86400)){
            continue;
        }
        // add news object procedure...
        News tmp(this->source.getName(), item.toObject());
        QString tmp_hash = tmp.getHash();
        if(!cfg.getNews().contains(tmp_hash)){
            // download cache
            QString img_url = item.toObject().value("image_url").toString();
            if(!img_url.isNull() || !img_url.isEmpty()){
                int numOfMonth = QDateTime::fromSecsSinceEpoch(tmp.getTimestamp()).date().month();
                QString virtual_path = "cache/"+QString::number(numOfMonth)+"/"+tmp_hash;
                QFile f(virtual_path);
                if(!f.exists()){
                    QString img_path = downloadImageToCacheFromUrl(img_url, tmp_hash);
                    if(!img_path.isNull()){
                        tmp.setImagePath(img_path);
                    }
                }else{
                    tmp.setImagePath(virtual_path);
                }
            }
            new_news_objects.append(tmp);
        }
    }
    // send signal to WorkerThread
    this->parentThread->updateNewsList(new_news_objects);
}
