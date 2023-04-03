#ifndef SETTINGSWIDGETCATEGORIES_HPP
#define SETTINGSWIDGETCATEGORIES_HPP
#include<core/language.hpp>
#include<ui/settingswidgetitems.hpp>
#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>

class SettingsWidgetCategories : public QWidget{
    Q_OBJECT
public:
    explicit SettingsWidgetCategories(Language *lang, QWidget * parent = nullptr);
    ~SettingsWidgetCategories();
    void updateLanguage(Language * lang);
public slots:
    void categoryListHasBeenEdited();
signals:
    void categoriesHaveBeenChanged(); // for parent slots
    void updateCategoryListWidget(); // for local slots

private:
    Language * language = nullptr;
    QVBoxLayout * mainLayout = nullptr;

    // adding category
    QWidget * adderWidget = nullptr;
    QHBoxLayout * adderLayout = nullptr;
    SettingsWidgetCategoryAdderForm * addCategoryForm = nullptr;
    // categories list:
    SettingsWidgetCategoryList * categoryListWidget;
};
#endif