#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include<ui/navigatewidget.hpp>
#include<ui/contentview.hpp>
#include<core/category.hpp>
#include<QSplitter>

class MainView : public QSplitter
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);

private:
    NavigateWidget * navigate;
    QWidget * currentContentView;
    Category * currentCategory = nullptr;

public slots:
    void switchToNewsWidget();
    void switchToSettingsWidget();
    void switchToCategoryWidget(Category * category);  
    void switchToFavoritesNews();
    void newArticleAvailable();
    
signals:
    void filterByCategory(Category * category);
    void filterByFavorites();
    void showAllNews();
    void callToNewContainerWithNewArticles(Category * category);
    void newSourceAvailable();
};

#endif
