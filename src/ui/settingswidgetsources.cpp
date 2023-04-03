#include<ui/settingswidgetsources.hpp>

SettingsWidgetSources::SettingsWidgetSources(Language * lang, QWidget * parent) : QWidget(parent)
{
    this->language = lang;
    this->mainLayout = new QVBoxLayout();
    this->adderWidget = new QWidget();
    this->adderLayout = new QHBoxLayout();
    this->addSourceForm = new SettingsWidgetSourceAdderForm(this->language);
    this->sourceTreeWidget = new SettingsWidgetSourceTree(this->language);

    this->adderLayout->setAlignment(Qt::AlignLeft);
    this->adderLayout->addWidget(this->addSourceForm);
    this->adderWidget->setLayout(this->adderLayout);

    this->mainLayout->addWidget(this->adderWidget);
    this->mainLayout->addWidget(this->sourceTreeWidget);

    // connects

    // for submit form and add source
    connect(this->addSourceForm, &SettingsWidgetSourceAdderForm::sourceAdded, this, &SettingsWidgetSources::treeHasBeenEdited);
    
    // for refresh source tree
    connect(this,&SettingsWidgetSources::updateSourceTreeWidget, this->sourceTreeWidget, &SettingsWidgetSourceTree::sourceHasBeenChanged);
    
    // layout
    setLayout(this->mainLayout);
}

void SettingsWidgetSources::updateLanguage(Language * lang){
    this->language = lang;
    this->addSourceForm->updateLanguage(this->language);
    this->sourceTreeWidget->updateLanguage(this->language);
}

void SettingsWidgetSources::treeHasBeenEdited(){
    emit updateSourceTreeWidget(); // request for refresh source tree (source changed)
}

void SettingsWidgetSources::categoryHasBeenChanged(){
    emit updateSourceTreeWidget(); // request for refresh source tree (category changed)
}

SettingsWidgetSources::~SettingsWidgetSources(){
    delete sourceTreeWidget;
    delete addSourceForm;
    delete adderLayout;
    delete adderWidget;
    delete mainLayout;
}