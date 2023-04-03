#ifndef NEWSITEMWIDGET_H
#define NEWSITEMWIDGET_H

#include <core/news.hpp>
#include <ui/newsitemlabels.hpp>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class NewsItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewsItemWidget(QWidget *parent = nullptr);
    void setNews(const News & news);

    ~NewsItemWidget();

signals:
    void favoriteClicked();
    void hideClicked();
    void titleClicked();
    void thumbnailClicked();
    

private:
    // methods
    void update();
    void clean();
    // ui elements
    QWidget row_1;  // all first row without description
    QHBoxLayout row_1_layout; // two elements image/col2 
    QWidget row_1_col_2;
    QVBoxLayout row_1_col_2_layout;
    QWidget row_1_col_2_date;
    QHBoxLayout row_1_col_2_date_layout;
    QWidget row_2;
    QVBoxLayout row_2_layout;
    QVBoxLayout mainLayout;

    // detaild
    NewsItemTitle title;
    QLabel date;
    NewsItemDescription description;
    NewsItemImage image;
    NewsItemFavoriteButton favoriteButton;
    NewsItemHideButton hideButton;

    // state
    News article;
    bool isDescriptionVisable;

private slots:
    void onClickFavoriteButton();
    void onClickHideButton();
    void onClickTitle();
    void onClickImage();

};

#endif // NEWSITEMWIDGET_H
