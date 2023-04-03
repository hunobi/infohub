#ifndef NAVIGATEWIDGET_HPP
#define NAVIGATEWIDGET_HPP
#include <core/category.hpp>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QToolTip>

class AllNewsButton;
class SettingsButton;
class CategoryButton;
class FavoritesButton;

class NavigateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigateWidget(QWidget *parent = nullptr);
    enum NavWidget{
        NEWS,
        CATEGORY,
        SETTINGS,
        FAVORITE
    };

public slots:
    void setCurrentNav(NavWidget value, Category * category);
    void updateCategories();

signals:
    void setNewsWidget();
    void setSettingsWidget();
    void setFavoritesWidget();
    void setCategoryWidget(Category * category);

private:
    // load categories
    void loadCategoris();
    // disable selected margin
    void changeSelectedMargin(NavWidget value, Category * category);
    // layout of navigate widget
    QVBoxLayout * layout = nullptr;
    // all news button
    AllNewsButton * allNewsButton = nullptr;
    // category scroll container
    QScrollArea * categoryScrollArea = nullptr;
    // category layout in scroll area
    QVBoxLayout * categoryArea = nullptr;
    QWidget * categoryAreaWidget = nullptr;
    // setting button
    SettingsButton * settingsButton = nullptr;
    // favorite button
    FavoritesButton * favoritesButton = nullptr;
    // current navwidget
    NavWidget currentNav;
};

class AllNewsButton : public QLabel{
    Q_OBJECT
public:
    explicit AllNewsButton(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void setOpenNavWidget(NavigateWidget::NavWidget type, Category * category);
};


class SettingsButton : public QLabel{
    Q_OBJECT
public:
    explicit SettingsButton(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void setOpenNavWidget(NavigateWidget::NavWidget type, Category * category);
};

class FavoritesButton : public QLabel{
    Q_OBJECT
public:
    explicit FavoritesButton(QWidget *parent = nullptr);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void setOpenNavWidget(NavigateWidget::NavWidget type, Category * category);
};

class CategoryButton : public QLabel{
    Q_OBJECT
public:
    explicit CategoryButton(QWidget *parent = nullptr);
    Category getCategoryDetails(){return this->categoryDetails;}
    void setCategoryDetails(const Category & value);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    void configuration();
    Category categoryDetails;

signals:
    void setOpenNavWidget(NavigateWidget::NavWidget type, Category * category);

public slots:

    void showToolTip()
    {
        QToolTip::showText(this->mapToGlobal(QPoint(100,0)), this->categoryDetails.getName());
    }

    void hideToolTip()
    {
        QToolTip::hideText();
    }
};


#endif