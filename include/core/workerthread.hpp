#ifndef WORKERTHREAD_HPP
#define WORKERTHREAD_HPP

#include<QObject>
#include<QRunnable>
#include<QThread>

#include<core/source.hpp>
#include<core/news.hpp>

class WorkerThread : public QThread {
    Q_OBJECT
public:
    WorkerThread();
    void run() override;
    void updateNewsList(const QList<News> & LNews);
    void setThreadRunning(const bool & value){this->isThreadRunning = value;}
private:
    void load_news();
    qint64 timestamp_last_load_news;
    QMap<QString,News> news_from_tasks;
    bool isThreadRunning;
    bool firstTime;

signals:
    void updatedNewsWithContent(QMap<QString,News> new_news);
    void newsHasBeenUpdated();
};


class LoadNewsTask : public QRunnable{
public:
    LoadNewsTask(const Source &src, WorkerThread * parent) : source(src), parentThread(parent){}
    void run() override;

private:
    WorkerThread * parentThread;
    Source source;
};


#endif