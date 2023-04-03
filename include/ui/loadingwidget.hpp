#ifndef LOADINGWIDGET_HPP
#define LOADINGWIDGET_HPP

#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>

class LoadingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingWidget(QWidget *parent = nullptr);
    void checkRequiredFiles();
    ~LoadingWidget();

private:
    QVBoxLayout *layout = nullptr;
    QProgressBar *progressBar = nullptr;
    int progress;

private slots:
    void updateProgress(uint i, uint max);
    
signals:
    void procedureFinished();
};


#endif