#include<ui/contentview.hpp>
#include<core/config.hpp>
#include<QPixmapCache>
#include<QScrollBar>

NewsContainerWidget::NewsContainerWidget(QWidget *parent): QWidget(parent)
{
    setMinimumWidth(800);
    this->page = 1;
    scrollArea.setBackgroundRole(QPalette::Dark);
    scrollArea.setStyleSheet("QScrollArea { border: none; } QScrollBar:vertical{border: none; background-color: #2f2f2f; width: 8px; margin: 0px; border-radius: 4px;} QScrollBar::handle:vertical{background-color: #5f5f5f; min-height: 20px; border-radius: 4px;} QScrollBar::add-line:vertical{background-color: #2f2f2f; height: 0px; subcontrol-position: bottom; subcontrol-origin: margin;} QScrollBar::sub-line:vertical{background-color: #2f2f2f; height: 0px; subcontrol-position: top; subcontrol-origin: margin;}");
    scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea.setWidgetResizable(true);
    scrollAreaWidget.setLayout(&scrollAreaLayout);
    scrollArea.setWidget(&scrollAreaWidget);
    layout.addWidget(&scrollArea);
    // nav bar
    spacing_left = new QWidget();
    spacing_right = new QWidget();
    spacing_left->setMinimumSize(128,48);
    spacing_right->setMinimumSize(128,48);
    spacing_left->hide();
    spacing_right->hide();
    pageLabel = new QLabel(QString::number(this->page));
    pageLabel->setTextFormat(Qt::TextFormat::RichText);
    pageLabel->setStyleSheet("color: #c2c2c2; font-size: 32px; font-family: Monospace; font-weight: bold; border: 2px solid gray; border-radius: 15px; padding: 2px;");
    nextButton = new NewsContainerNextPageButton();
    prevButton = new NewsContainerPrevPageButton();
    navPageLayout = new QHBoxLayout();
    navPageLayout->setAlignment(Qt::AlignCenter);
    navPageLayout->setSpacing(0);
    navPageLayout->setContentsMargins(0,0,0,0);
    navPageLayout->addWidget(spacing_left,0);
    navPageLayout->addWidget(prevButton,0);
    navPageLayout->addWidget(pageLabel,1,Qt::AlignCenter);
    navPageLayout->addWidget(nextButton,0);
    navPageLayout->addWidget(spacing_right,0);
    navPageWidget = new QWidget();
    navPageWidget->setLayout(navPageLayout);
    layout.addWidget(navPageWidget);
    setLayout(&layout);
    
    connect(this->nextButton, &NewsContainerNextPageButton::loadNextPage, this, &NewsContainerWidget::loadNextPage);
    connect(this->prevButton, &NewsContainerPrevPageButton::loadPrevPage, this, &NewsContainerWidget::loadPrevPage);

    this->loadNews(); // load all news
    //this->reload(); // print all

}

/*
    Load and sort all news into cache, remember to skip hidden news
*/
void NewsContainerWidget::loadNews(){
    this->news_cache.clear();
    QMap<QString, News> nMap = ConfigSingleton::getInstance().GetConfig().getNews();
    for(QString key : nMap.keys()){
        if(nMap[key].isHidden()){continue;}
        this->news_cache.append(nMap[key]);
    }
    std::sort(this->news_cache.begin(), this->news_cache.end(), [](News &n1, News &n2){
        return n1.getTimestamp() > n2.getTimestamp();
    });
}

/*
    When new articles are available, reload news and if user look at first page, 
    reload widget. If category is selected, this method will filter articles.
*/
void NewsContainerWidget::newArticlesAvailable(Category * category = nullptr){
    this->loadNews();
    if(category){
        this->filterByCategory(category);    
    }
    if(this->page == 1){
        this->reload();
    }
}

/*
    Load prev page
*/
void NewsContainerWidget::loadNextPage(){
    if(this->page < this->maxPage){
        this->page +=1; 
        pageLabel->setText(QString::number(this->page));
        this->reload(); 
    }
}

/*
    Load next page
*/
void NewsContainerWidget::loadPrevPage(){
    if(this->page > 1){
        this->page -=1;
        pageLabel->setText(QString::number(this->page));
        this->reload(); 
    }
}

/*
    Reload widget
*/
void NewsContainerWidget::reload(){
    this->maxPage = std::ceil(news_cache.size()/(float)ConfigSingleton::getInstance().GetConfig().getSettings().getNewsLimit());
    this->clean();
     if(this->page == 1){
        this->prevButton->hide();
        this->spacing_left->show();
    }else{
        this->prevButton->show();
        this->spacing_left->hide();
    }

    if(this->maxPage == this->page){
        this->nextButton->hide();
        this->spacing_right->show();
    }else{this->nextButton->show();this->spacing_right->hide();}

    scrollAreaLayout.setAlignment(Qt::AlignTop);
    // load limited
    int offset = ConfigSingleton::getInstance().GetConfig().getSettings().getNewsLimit();
    int startPosition = (this->page-1)*offset;
    QList<News> tmpList = news_cache.mid(startPosition, offset);

    for(News item : tmpList){
        news_in_layout.append(new NewsItemWidget());
        news_in_layout.at(news_in_layout.count()-1)->setNews(item);
    }

    // add to layout
    for(auto item : this->news_in_layout){
        scrollAreaLayout.addWidget(item);
    }
    scrollArea.verticalScrollBar()->setValue(0);
}

/*
    Filter News by Favorites
*/
void NewsContainerWidget::filterByFavorites(){
    QList<News> tmp;
    for(News item : this->news_cache){
        if(item.isSaved()){
            tmp.append(item);
        }
    }
    this->news_cache = tmp;
    // reload view
    this->reload();
}

/*
    Filter News by Category
*/
void NewsContainerWidget::filterByCategory(Category * category){
    // if category is nullptr... skip
    if(!category){return;}
    // Filter by Category and reduce news_cache
    QList<News> tmp;
    for(News item : this->news_cache){
        if(item.getCategories().contains(category->getName())){
            tmp.append(item);
        }
    }
    this->news_cache = tmp;
    // reload view
    this->reload();
}

/*
    Show all news, but..
    This is main page so, we want to skip all news with disabled sources
    Disabled sources will be visible in categories and it is correct situation.
*/
void NewsContainerWidget::showAll(){
    auto sList = ConfigSingleton::getInstance().GetConfig().getSources();
    QList<News> tmp;
    for(QString key : sList.keys()){
        if(!sList[key].isEnabled()){
            for(News item : news_cache){
                if(item.getSourceName()==key){
                    tmp.append(item);
                }
            }
        }
    }
    for(News item : tmp){
        int index = news_cache.indexOf(item);
        if(index > -1){
            news_cache.removeAt(index);
        }
    }
    this->reload();
}

void NewsContainerWidget::clean(){
    for(NewsItemWidget* item : this->news_in_layout){
        if(item){
            scrollAreaLayout.removeWidget(item);
            delete item;
        }
    }
    news_in_layout.squeeze();
    news_in_layout.clear();
}

NewsContainerWidget::~NewsContainerWidget(){
    this->clean();
    this->news_cache.squeeze();
    this->news_cache.clear();
    this->scrollAreaLayout.deleteLater();
    this->layout.deleteLater();
    delete nextButton;
    delete prevButton;
    delete pageLabel;
    delete spacing_left;
    delete spacing_right;
    delete navPageLayout;
    delete navPageWidget;
    QPixmapCache::clear();
}


//==================

NewsContainerNextPageButton::NewsContainerNextPageButton(QWidget *parent) : QLabel(parent){
    setPixmap(QPixmap(":/icons/news-page-next.png"));
    setMaximumSize(128,48);
    setMinimumSize(128,48);
    setScaledContents(true);
}

void NewsContainerNextPageButton::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}
void NewsContainerNextPageButton::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
void NewsContainerNextPageButton::mousePressEvent(QMouseEvent *event){
    emit loadNextPage();
}

// =======================

NewsContainerPrevPageButton::NewsContainerPrevPageButton(QWidget *parent) : QLabel(parent){
    setPixmap(QPixmap(":/icons/news-page-prev.png"));
    setMaximumSize(128,48);
    setMinimumSize(128,48);
    setScaledContents(true);
}

void NewsContainerPrevPageButton::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}
void NewsContainerPrevPageButton::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
void NewsContainerPrevPageButton::mousePressEvent(QMouseEvent *event){
    emit loadPrevPage();
}