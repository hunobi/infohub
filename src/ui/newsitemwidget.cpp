#include<ui/newsitemwidget.hpp>
#include<core/utils.hpp>
#include<core/config.hpp>
#include<QPixmap>
#include<QDesktopServices>
#include<QUrl>

NewsItemWidget::NewsItemWidget(QWidget *parent) : QWidget(parent){
    this->isDescriptionVisable = false;
    date.setTextFormat(Qt::TextFormat::RichText);
    date.setStyleSheet("color: #cbcbcb;");
    row_2.setStyleSheet("background-color: #4b5469; border: 5px black solid; border-bottom-left-radius: 15px; border-bottom-right-radius: 15px; margin-top: 0px; margin-bottom: 10px;");
    mainLayout.setContentsMargins(0, 0, 0, 0);
    mainLayout.setSpacing(0);
    //connections:
    connect(&title, &NewsItemTitle::openUrl, this, &NewsItemWidget::onClickTitle);
    connect(&favoriteButton, &NewsItemFavoriteButton::favoriteButtonClicked, this, &NewsItemWidget::onClickFavoriteButton);
    connect(&hideButton, &NewsItemHideButton::hideButtonClicked, this, &NewsItemWidget::onClickHideButton);
    connect(&image, &NewsItemImage::openUrl, this, &NewsItemWidget::onClickImage);
}

void NewsItemWidget::setNews(const News & news){
    article = news;
    this->update();
}

void NewsItemWidget::update(){
    this->clean();
    date.setText(secondsToDateTimeString(this->article.getTimestamp()).append(" [").append(QUrl(this->article.getUrl()).host()).append("]"));
    date.setScaledContents(true);
    date.setFont(QFont("Courier", 12, QFont::Weight::Thin));

    title.setText(article.getTitle());
    image.setPixmap(QPixmap(this->article.getImagePath()).scaled(400,400));

    if(this->article.isSaved()){
        favoriteButton.setPixmap(QPixmap(":/icons/news-favorite-on.png"));
    }else{
        favoriteButton.setPixmap(QPixmap(":/icons/news-favorite-off.png"));
    }

    if(this->article.isHidden()){
        hideButton.setPixmap(QPixmap(":/icons/news-hide.png"));
    }else{
        hideButton.setPixmap(QPixmap(":/icons/news-show.png"));
    }
    
    row_1_layout.addWidget(&image,0, Qt::AlignTop);

    row_1_col_2_date_layout.addWidget(&date,2,Qt::AlignTop);
    row_1_col_2_date_layout.addWidget(&hideButton, 0);
    row_1_col_2_date_layout.addWidget(&favoriteButton,0);
    row_1_col_2_date.setLayout(&row_1_col_2_date_layout);
    row_1_col_2_layout.addWidget(&row_1_col_2_date,0);
    row_1_col_2_layout.addWidget(&title,1,Qt::AlignTop);
    row_1_col_2.setLayout(&row_1_col_2_layout);
    
    row_1_layout.addWidget(&row_1_col_2);
    row_1.setLayout(&row_1_layout);
    row_1.setMaximumHeight(row_1.height());
    mainLayout.addWidget(&row_1);

    /*
        Check if news describe is not empty. Choose styles and add describe to main layout

        In the future i will use this->isDescriptionVisable in this place.
    */
    if(!this->article.getDescribe().isEmpty()){
        row_1.setStyleSheet("background-color: #4b5469; border: 5px black solid;border-top-left-radius: 15px; border-top-right-radius: 15px; margin-bottom: 0px;");
        description.setText(removeHtmlTags(this->article.getDescribe().left(500).append(" [...]")));
        row_2_layout.addWidget(&description);
        row_2.setLayout(&row_2_layout);
        mainLayout.addWidget(&row_2);
    }else{
        row_1.setStyleSheet("background-color: #4b5469; border: 5px black solid;border-radius: 15px;  margin-bottom: 10px;");
    }
    
    setLayout(&mainLayout);
}

/*
    Remove items
*/
void NewsItemWidget::clean(){    
    image.pixmap().detach();
    image.clear();
    favoriteButton.pixmap().detach();
    favoriteButton.clear();
    hideButton.pixmap().detach();
    hideButton.clear();
    row_1_layout.removeWidget(&image);
    row_1_col_2_date_layout.removeWidget(&date);
    row_1_col_2_date_layout.removeWidget(&hideButton);
    row_1_col_2_date_layout.removeWidget(&favoriteButton);
    row_1_col_2_layout.removeWidget(&row_1_col_2_date);
    row_1_col_2_layout.removeWidget(&title);
    mainLayout.removeWidget(&row_1);
    row_1_layout.removeWidget(&row_1_col_2);
    row_2_layout.removeWidget(&description);
    mainLayout.removeWidget(&row_2);
}

/*
    Add or Remove news from saved
*/
void NewsItemWidget::onClickFavoriteButton(){
    this->article.setSavedStatus(!this->article.isSaved());
    ConfigSingleton::getInstance().GetConfig().getNews()[this->article.getHash()].setSavedStatus(this->article.isSaved());
    ConfigSingleton::getInstance().Save();
    this->update();
}

/*
    Click to hide news or show again (toggle)
*/
void NewsItemWidget::onClickHideButton(){
    this->article.setHiddenStatus(!this->article.isHidden());
    ConfigSingleton::getInstance().GetConfig().getNews()[this->article.getHash()].setHiddenStatus(this->article.isHidden());
    ConfigSingleton::getInstance().Save();
    this->update();
}

/*
    When you click on Title, you will open URL in your favorite browser and you will mark news as read.
*/
void NewsItemWidget::onClickTitle(){
    QDesktopServices::openUrl(QUrl(this->article.getUrl()));
    if(!this->article.isRead()){
        ConfigSingleton::getInstance().GetConfig().getNews()[this->article.getHash()].setReadStatus(true);
        ConfigSingleton::getInstance().Save();
    }
}

/*
   Currently open URL like title click, in the future this method will show/hide description
*/
void NewsItemWidget::onClickImage(){
    this->onClickTitle();
}

NewsItemWidget::~NewsItemWidget(){
    this->clean();
}

// =======================================================

NewsItemTitle::NewsItemTitle(QWidget *parent) : QLabel(parent){
    setTextFormat(Qt::TextFormat::RichText);
    setStyleSheet("color: #ffad4e ");
    setWordWrap(true);
    setScaledContents(true);
    setFont(QFont("Serif", 18, QFont::Weight::Bold));
}

void NewsItemTitle::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}
void NewsItemTitle::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
void NewsItemTitle::mousePressEvent(QMouseEvent *event){
    emit openUrl();
}

// =======================================================

NewsItemDescription::NewsItemDescription(QWidget *parent) : QLabel(parent){
    setTextFormat(Qt::TextFormat::RichText);
    setStyleSheet("color: #d1d6da; padding: 8px; qproperty-alignment: 'AlignJustify';");
    setWordWrap(true);
    setScaledContents(true);
    setTextInteractionFlags(Qt::TextSelectableByMouse);
    setFont(QFont("Times",14, QFont::Weight::Medium, false));
}

void NewsItemDescription::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::CursorShape::IBeamCursor);  
}
void NewsItemDescription::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
/*
void NewsItemDescription::mousePressEvent(QMouseEvent *event){
    //emit hideDescription();
}*/

// =======================================================

NewsItemImage::NewsItemImage(QWidget *parent) : QLabel(parent){
    setFixedSize(200,200);
    setMaximumSize(200,200);
    setMinimumSize(200,200);
    setScaledContents(true);
}

void NewsItemImage::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}
void NewsItemImage::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
void NewsItemImage::mousePressEvent(QMouseEvent *event){
    emit openUrl();
}

// =======================================================

NewsItemFavoriteButton::NewsItemFavoriteButton(QWidget * parent) : QLabel(parent){
    setFixedSize(32,32);
    setMaximumSize(32,32);
    setMinimumSize(32,32);
    setScaledContents(true);
}

void NewsItemFavoriteButton::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}
void NewsItemFavoriteButton::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
void NewsItemFavoriteButton::mousePressEvent(QMouseEvent *event){
    emit favoriteButtonClicked();
}

// =======================================================

NewsItemHideButton::NewsItemHideButton(QWidget * parent) : QLabel(parent){
    setFixedSize(32,32);
    setMaximumSize(32,32);
    setMinimumSize(32,32);
    setScaledContents(true);
}

void NewsItemHideButton::enterEvent(QEnterEvent * event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}

void NewsItemHideButton::leaveEvent(QEvent * event){
    QLabel::leaveEvent(event); 
    setCursor(Qt::ArrowCursor);
}
void NewsItemHideButton::mousePressEvent(QMouseEvent *event){
    emit hideButtonClicked();
}