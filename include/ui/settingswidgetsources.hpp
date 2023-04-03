#ifndef SETTINGSWIDGETSOURCES_HPP
#define SETTINGSWIDGETSOURCES_HPP

#include<core/language.hpp>
#include<ui/settingswidgetitems.hpp>
#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>

class SettingsWidgetSources : public QWidget{
    Q_OBJECT
public:
    explicit SettingsWidgetSources(Language *lang, QWidget * parent = nullptr);
    ~SettingsWidgetSources();
    void updateLanguage(Language * lang);
public slots:
    void treeHasBeenEdited();
    void categoryHasBeenChanged();

signals:
    void updateSourceTreeWidget();

private:
    Language * language = nullptr;
    QVBoxLayout * mainLayout = nullptr;

    // adding source
    QWidget * adderWidget = nullptr;
    QHBoxLayout * adderLayout = nullptr;
    SettingsWidgetSourceAdderForm * addSourceForm = nullptr;
    // source tree:
    SettingsWidgetSourceTree * sourceTreeWidget;
};


#endif