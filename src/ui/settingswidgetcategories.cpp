#include<ui/settingswidgetcategories.hpp>

SettingsWidgetCategories::SettingsWidgetCategories(Language *lang, QWidget * parent) : QWidget(parent){
    this->language = lang;
    this->addCategoryForm = new SettingsWidgetCategoryAdderForm(this->language);
    this->mainLayout = new QVBoxLayout();
    this->adderWidget = new QWidget();
    this->adderLayout = new QHBoxLayout();

    this->adderLayout->setAlignment(Qt::AlignLeft);
    
    this->adderLayout->addWidget(this->addCategoryForm);
    this->adderWidget->setLayout(this->adderLayout);
    
    
    this->categoryListWidget = new SettingsWidgetCategoryList(this->language);
    


    this->mainLayout->addWidget(this->adderWidget);
    this->mainLayout->addWidget(this->categoryListWidget);
    // connects
    connect(this->addCategoryForm, &SettingsWidgetCategoryAdderForm::categoryAdded, this, &SettingsWidgetCategories::categoryListHasBeenEdited);
    connect(this,&SettingsWidgetCategories::updateCategoryListWidget, this->categoryListWidget, &SettingsWidgetCategoryList::updateList);
    connect(this->categoryListWidget, &SettingsWidgetCategoryList::categoryUpdated, this, &SettingsWidgetCategories::categoryListHasBeenEdited);
    setLayout(this->mainLayout);
}

void SettingsWidgetCategories::updateLanguage(Language * lang){
    this->language = lang;
    this->addCategoryForm->updateLanguage(this->language);
    this->categoryListWidget->updateLanguage(this->language);
}

void SettingsWidgetCategories::categoryListHasBeenEdited(){
    emit updateCategoryListWidget();
    emit categoriesHaveBeenChanged();
}

SettingsWidgetCategories::~SettingsWidgetCategories(){
    delete addCategoryForm;
    delete categoryListWidget;
    delete adderLayout;
    delete adderWidget;
    delete mainLayout;
}