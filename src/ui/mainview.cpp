#include<ui/mainview.hpp>
#include<ui/mainwindow.hpp>
#include<malloc.h>

#ifdef _WIN32
    #include <malloc.h>
    #define MEMORY_TRIM _heapmin()
#else
    #include <malloc.h>
    #define MEMORY_TRIM malloc_trim(0)
#endif


MainView::MainView(QWidget *parent): QSplitter(parent)
{
    setHandleWidth(5);
    setStyleSheet("QSplitter::handle { background-color: gray; }");
    
    navigate = new NavigateWidget();
    currentContentView = new NewsContainerWidget();
    static_cast<NewsContainerWidget*>(currentContentView)->reload();
    addWidget(navigate);
    addWidget(currentContentView);

    QList<int> sizes;
    sizes << 20 << 80;
    setSizes(sizes);

    connect(navigate, &NavigateWidget::setNewsWidget, this, &MainView::switchToNewsWidget);
    connect(navigate, &NavigateWidget::setSettingsWidget, this, &MainView::switchToSettingsWidget);
    connect(navigate, &NavigateWidget::setCategoryWidget, this, &MainView::switchToCategoryWidget);
    connect(navigate, &NavigateWidget::setFavoritesWidget, this, &MainView::switchToFavoritesNews);
}

/*
    This slot sends information to the news container that new articles are available.
*/
void MainView::newArticleAvailable(){
    NewsContainerWidget* tmp = dynamic_cast<NewsContainerWidget*>(currentContentView);
    if(tmp){
        connect(this, &MainView::callToNewContainerWithNewArticles, tmp, &NewsContainerWidget::newArticlesAvailable);
        emit callToNewContainerWithNewArticles(this->currentCategory);
    }
}

/*
    Open a container with messages filtered by favorites
*/
void MainView::switchToFavoritesNews(){
    if(currentContentView){
        this->currentCategory = nullptr;
        delete currentContentView;
    }
    //malloc_trim(0); // free memory after other views
    #ifdef _WIN32
        _heapmin();
    #else
        malloc_trim(0);
    #endif
	
    currentContentView = new NewsContainerWidget();
    connect(this, &MainView::filterByFavorites, static_cast<NewsContainerWidget*>(currentContentView), &NewsContainerWidget::filterByFavorites);
    emit filterByFavorites();
    addWidget(currentContentView);
}

/*
    Open a container with messages
*/
void MainView::switchToNewsWidget(){
    if(currentContentView){
        this->currentCategory = nullptr;
        delete currentContentView;
    }
    //malloc_trim(0); // free memory after other views
    MEMORY_TRIM;
	
    currentContentView = new NewsContainerWidget();
    connect(this, &MainView::showAllNews, static_cast<NewsContainerWidget*>(currentContentView), &NewsContainerWidget::showAll);
    emit showAllNews();
    addWidget(currentContentView);
    qDebug() << "switch to news";
}

/*
    Open settings view
*/
void MainView::switchToSettingsWidget(){
    if(currentContentView){
        this->currentCategory = nullptr;
        delete currentContentView;
    }
    //malloc_trim(0); // free memory after other views

    MEMORY_TRIM;
	
    currentContentView = new SettingsWidget();
    connect(static_cast<SettingsWidget*>(currentContentView), &SettingsWidget::updateNavigateBar, this->navigate, &NavigateWidget::updateCategories);
    addWidget(currentContentView);
    qDebug() << "switch to setting";
}

/*
    Open a container with messages filtered by category
*/
void MainView::switchToCategoryWidget(Category * category){
    if(currentContentView){
        this->currentCategory = nullptr;
        delete currentContentView;
    }
    //malloc_trim(0); // free memory after other views
    MEMORY_TRIM;
    this->currentCategory = category;
    qDebug() << "switch to category on position: "<< this->currentCategory ->getPosition();

    currentContentView = new NewsContainerWidget();
    connect(this, &MainView::filterByCategory, static_cast<NewsContainerWidget*>(currentContentView), &NewsContainerWidget::filterByCategory);
    emit filterByCategory(this->currentCategory);
    addWidget(currentContentView);
}  