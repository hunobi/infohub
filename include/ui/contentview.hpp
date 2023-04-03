#ifndef CONTENTVIEW_HPP
#define CONTENTVIEW_HPP

#include <core/category.hpp>
#include <core/news.hpp>
#include <core/language.hpp>

#include <ui/newsitemwidget.hpp>
#include <ui/settingswidgetform.hpp>
#include <ui/settingswidgetcategories.hpp>
#include <ui/settingswidgetsources.hpp>
#include <ui/settingswidgetitems.hpp>

#include <QJsonObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

class NewsContainerNextPageButton : public QLabel{
    Q_OBJECT
public:
    explicit NewsContainerNextPageButton(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void loadNextPage();
};

class NewsContainerPrevPageButton : public QLabel{
    Q_OBJECT
public:
    explicit NewsContainerPrevPageButton(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void loadPrevPage();
};

class NewsContainerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewsContainerWidget(QWidget *parent = nullptr);
    void reload();
    ~NewsContainerWidget();
private:
    void loadNews();
    void clean();
    int page;
    int maxPage;
    QList<News> news_cache;
    QList<NewsItemWidget*> news_in_layout;
    QVBoxLayout layout;
    QScrollArea scrollArea;
    QWidget scrollAreaWidget;
    QVBoxLayout scrollAreaLayout;
    QHBoxLayout * navPageLayout = nullptr;
    QWidget * navPageWidget = nullptr;
    NewsContainerNextPageButton *nextButton = nullptr;
    NewsContainerPrevPageButton *prevButton = nullptr;
    QLabel * pageLabel = nullptr;
    QWidget * spacing_left = nullptr;
    QWidget * spacing_right = nullptr;

 public slots:
    void newArticlesAvailable(Category * category);
    void loadNextPage();
    void loadPrevPage();
    void filterByCategory(Category * category);
    void filterByFavorites();
    void showAll();
};



class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

private:
    Language * language = nullptr;
    QVBoxLayout * layout = nullptr;
    QScrollArea * scrollArea = nullptr;
    QWidget * scrollAreaContainer = nullptr;
    QVBoxLayout * scrollAreaContainerLayout = nullptr;
    // form
    SettingsWidgetCategorySpacer * generalLabel = nullptr;
    SettingsWidgetForm * formWidget = nullptr;
    // categories
    SettingsWidgetCategorySpacer * categoriesLabel = nullptr;
    SettingsWidgetCategories * categoryWidget = nullptr;
    // sources
    SettingsWidgetCategorySpacer * sourcesLabel = nullptr;
    SettingsWidgetSources * sourceWidget = nullptr;

public slots:
    void categoriesHaveBeenChanged();
    void languageHasBeenChanged();
signals:
    void updateNavigateBar();
    void refreshSourceTree();
};














#endif