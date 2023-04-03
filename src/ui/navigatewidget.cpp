#include<ui/navigatewidget.hpp>
#include<core/config.hpp>
#include<QFile>

NavigateWidget::NavigateWidget(QWidget *parent): QWidget(parent)
{
    setMinimumHeight(200);
    setMinimumWidth(110);
    setMaximumWidth(110);
    layout = new QVBoxLayout(this);
    allNewsButton = new AllNewsButton();
    settingsButton = new SettingsButton();
    favoritesButton = new FavoritesButton();
    categoryScrollArea = new QScrollArea();
    categoryScrollArea->setBackgroundRole(QPalette::Dark);
    categoryScrollArea->setStyleSheet("QScrollArea { border: none; } QScrollBar:vertical{border: none; background-color: #2f2f2f; width: 8px; margin: 0px; border-radius: 4px;} QScrollBar::handle:vertical{background-color: #5f5f5f; min-height: 20px; border-radius: 4px;} QScrollBar::add-line:vertical{background-color: #2f2f2f; height: 0px; subcontrol-position: bottom; subcontrol-origin: margin;} QScrollBar::sub-line:vertical{background-color: #2f2f2f; height: 0px; subcontrol-position: top; subcontrol-origin: margin;}");
    categoryScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    categoryScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    categoryScrollArea->setWidgetResizable(true);
    categoryArea = new QVBoxLayout();
    categoryAreaWidget = new QWidget();
    categoryArea->setAlignment(Qt::AlignTop);
    categoryScrollArea->setWidget(categoryAreaWidget);
    categoryScrollArea->setMinimumHeight(100);
    this->loadCategoris();
    layout->addWidget(allNewsButton, 0 , Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(categoryScrollArea, 2);
    layout->addSpacing(10);
    layout->addWidget(favoritesButton,0,Qt::AlignCenter);
    layout->addSpacing(5);
    layout->addWidget(settingsButton,0, Qt::AlignCenter);
    // set current widget  
    currentNav = NavWidget::NEWS;
    this->changeSelectedMargin(NavWidget::NEWS, nullptr);
    connect(this->allNewsButton, &AllNewsButton::setOpenNavWidget, this, &NavigateWidget::setCurrentNav);
    connect(this->settingsButton, &SettingsButton::setOpenNavWidget, this, &NavigateWidget::setCurrentNav);
    connect(this->favoritesButton, &FavoritesButton::setOpenNavWidget, this, &NavigateWidget::setCurrentNav);
}

/*
    ON/OFF visual effect - mark selected nav button
*/
void NavigateWidget::changeSelectedMargin(NavWidget value, Category * category){
    QString styleON =  "border-bottom: 2px solid; border-bottom-color: #ff6b23; padding-bottom: 5px;";
    QString styleOFF = "border-bottom: 0";
    
    // off all styles
    allNewsButton->setStyleSheet(styleOFF);
    settingsButton->setStyleSheet(styleOFF);
    favoritesButton->setStyleSheet(styleOFF);
    for(int i = 0; i < this->categoryArea->count(); i++){
        QWidget * w = this->categoryArea->itemAt(i)->widget();
        if(w){
            CategoryButton * tmp = dynamic_cast<CategoryButton*>(w);
            tmp->setStyleSheet(styleOFF);
        }
    }
    // set style to widget
    switch(value){
        case NavWidget::NEWS:
            allNewsButton->setStyleSheet(styleON);
            break;
        
        case NavWidget::SETTINGS:
            settingsButton->setStyleSheet(styleON);
            break;

        case NavWidget::FAVORITE:
            favoritesButton->setStyleSheet(styleON);
            break;

        case NavWidget::CATEGORY:
            CategoryButton * tmpBtn = nullptr;
            for(int i = 0; i < this->categoryArea->count(); i++){
                QWidget * w = this->categoryArea->itemAt(i)->widget();
                if(w){
                    CategoryButton * tmp = dynamic_cast<CategoryButton*>(w);
                    if(tmp && tmp->getCategoryDetails().getPosition() == category->getPosition()){
                        tmpBtn = tmp;
                        tmpBtn->setStyleSheet(styleON);
                        break;
                    }
                }
            }
            break;
    }
}

/*
    Change the Nav view. Category is the same view as News, but filtered.
    This method sends signal to MainView widget.
*/
void NavigateWidget::setCurrentNav(NavWidget value, Category * category){
        currentNav = value;
        changeSelectedMargin(currentNav, category);
        switch(currentNav){
            case NavWidget::NEWS:
                emit setNewsWidget();
                break;
            case NavWidget::SETTINGS:
                emit setSettingsWidget();
                break;
            case NavWidget::CATEGORY:
                emit setCategoryWidget(category);
                break;
            case NavWidget::FAVORITE:
                emit setFavoritesWidget();
                break;
        }
}

/*
    Refresh categoris widget
*/
void NavigateWidget::updateCategories(){
    this->loadCategoris();
}

/*
    Load categories from Config, sort and display them.
*/
void NavigateWidget::loadCategoris(){
    QMap<QString, Category> cMap = ConfigSingleton::getInstance().GetConfig().getCategories();
    QList<Category> categories;
    for(QString key : cMap.keys()){
        categories.append(cMap[key]);
    }
    std::sort(categories.begin(), categories.end(), [](Category c1, Category c2){
        return c1.getPosition() < c2.getPosition();
    });
    // clear layout
    while(!categoryArea->isEmpty()){
        delete categoryArea->takeAt(0);
    }
    
    delete categoryArea;
    delete categoryAreaWidget;

    categoryArea = new QVBoxLayout();
    categoryAreaWidget = new QWidget();
    categoryArea->setAlignment(Qt::AlignTop);
    for(Category item : categories){
        CategoryButton * cb = new CategoryButton();
        cb->setCategoryDetails(item);
        connect(cb, &CategoryButton::setOpenNavWidget, this, &NavigateWidget::setCurrentNav);
        this->categoryArea->addWidget(cb,0, Qt::AlignCenter);
        this->categoryArea->addSpacing(10);
    }
    this->categoryAreaWidget->setLayout(categoryArea);
    this->categoryScrollArea->setWidget(categoryAreaWidget);
}

/*============================================================================================*/


AllNewsButton::AllNewsButton(QWidget * parent) : QLabel(parent){
    setScaledContents(true);
    setPixmap(QPixmap(":/icons/news-icon.png"));
    setMaximumSize(64,64);
    setMinimumSize(64,64);
}

void AllNewsButton::enterEvent(QEnterEvent *event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);
}

void AllNewsButton::leaveEvent(QEvent *event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}

void AllNewsButton::mousePressEvent(QMouseEvent * event){
    emit setOpenNavWidget(NavigateWidget::NavWidget::NEWS, nullptr);
}

/*============================================================================================*/

FavoritesButton::FavoritesButton(QWidget * parent) : QLabel(parent){
    setScaledContents(true);
    setPixmap(QPixmap(":/icons/news-favorite-on.png"));
    setMaximumSize(64,64);
    setMinimumSize(64,64);
}

void FavoritesButton::enterEvent(QEnterEvent *event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);
}

void FavoritesButton::leaveEvent(QEvent *event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}

void FavoritesButton::mousePressEvent(QMouseEvent * event){
    emit setOpenNavWidget(NavigateWidget::NavWidget::FAVORITE, nullptr);
}

/*============================================================================================*/
SettingsButton::SettingsButton(QWidget * parent) : QLabel(parent){
    setScaledContents(true);
    setPixmap(QPixmap(":/icons/setting-icon.png"));
    setMaximumSize(64,64);
    setMinimumSize(64,64);
}

void SettingsButton::enterEvent(QEnterEvent *event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}

void SettingsButton::leaveEvent(QEvent *event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}

void SettingsButton::mousePressEvent(QMouseEvent * event){
    emit setOpenNavWidget(NavigateWidget::NavWidget::SETTINGS, nullptr);
}

/*============================================================================================*/

CategoryButton::CategoryButton(QWidget * parent) : QLabel(parent){
    setScaledContents(true);
    setMaximumSize(48,48);
    setMinimumSize(48,48);
};

void CategoryButton::setCategoryDetails(const Category & value){
    this->categoryDetails = value;
    this->configuration();
}

void CategoryButton::configuration(){
    QString path = this->categoryDetails.getIconPath();
    QFile icon(path);
    if(icon.exists()){
        setPixmap(QPixmap(path));
    }else{
        setPixmap(QPixmap(":/icons/default-category.png"));
    } 
}

void CategoryButton::enterEvent(QEnterEvent *event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
    showToolTip();
}

void CategoryButton::leaveEvent(QEvent *event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
    hideToolTip();
}

void CategoryButton::mousePressEvent(QMouseEvent * event){
    Category c = this->getCategoryDetails();
    emit setOpenNavWidget(NavigateWidget::NavWidget::CATEGORY, &c);
}
