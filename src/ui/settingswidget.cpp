#include<core/config.hpp>
#include<ui/contentview.hpp>

SettingsWidget::SettingsWidget(QWidget *parent): QWidget(parent)
{   
    Settings cfg = ConfigSingleton::getInstance().GetConfig().getSettings();
    this->language = new Language(cfg.getLanguage());
    layout = new QVBoxLayout(this);

    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setStyleSheet("QScrollArea { border: none; } QScrollBar:vertical{border: none; background-color: #2f2f2f; width: 8px; margin: 0px; border-radius: 4px;} QScrollBar::handle:vertical{background-color: #5f5f5f; min-height: 20px; border-radius: 4px;} QScrollBar::add-line:vertical{background-color: #2f2f2f; height: 0px; subcontrol-position: bottom; subcontrol-origin: margin;} QScrollBar::sub-line:vertical{background-color: #2f2f2f; height: 0px; subcontrol-position: top; subcontrol-origin: margin;}");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    
    // init
    scrollAreaContainerLayout = new QVBoxLayout(scrollArea);
    scrollAreaContainerLayout->setAlignment(Qt::AlignTop);
    generalLabel = new SettingsWidgetCategorySpacer(":/icons/settings-general-icon.png", this->language->getGeneralLabel());
    categoriesLabel = new SettingsWidgetCategorySpacer(":/icons/default-category.png", this->language->getCategoriesLabel());
    sourcesLabel = new SettingsWidgetCategorySpacer(":/icons/settings-news-icon.png", this->language->getSourcesLabel());
    
    // general
    scrollAreaContainerLayout->addWidget(generalLabel);
    formWidget = new SettingsWidgetForm(this->language);
    scrollAreaContainerLayout->addWidget(formWidget);
    // categories
    scrollAreaContainerLayout->addWidget(categoriesLabel);
    categoryWidget = new SettingsWidgetCategories(this->language);
    scrollAreaContainerLayout->addWidget(categoryWidget);
    // sources
    scrollAreaContainerLayout->addWidget(sourcesLabel);
    sourceWidget = new SettingsWidgetSources(this->language);
    scrollAreaContainerLayout->addWidget(sourceWidget);

    scrollAreaContainer = new QWidget();
    scrollAreaContainer->setLayout(scrollAreaContainerLayout);
    scrollArea->setWidget(scrollAreaContainer);
    
    // connects
    connect(this->categoryWidget, &SettingsWidgetCategories::categoriesHaveBeenChanged, this, &SettingsWidget::categoriesHaveBeenChanged);
    connect(this, &SettingsWidget::refreshSourceTree ,this->sourceWidget, &SettingsWidgetSources::categoryHasBeenChanged);
    connect(this->formWidget, &SettingsWidgetForm::languageChanged, this, &SettingsWidget::languageHasBeenChanged);
    layout->addWidget(scrollArea);
}

/*
    This slot works for two events: 
        1. category changed 
        2. source changed
*/
void SettingsWidget::categoriesHaveBeenChanged(){
    // categories have been changed so we need:
        // emit to navigate bar for changing 
    emit updateNavigateBar();
        // emit to refresh source tree
    emit refreshSourceTree();
}

void SettingsWidget::languageHasBeenChanged(){
    delete language;
    this->language = new Language(ConfigSingleton::getInstance().GetConfig().getSettings().getLanguage());
    generalLabel->setText(this->language->getGeneralLabel());
    formWidget->updateLanguage(this->language);
    categoriesLabel->setText(this->language->getCategoriesLabel());
    categoryWidget->updateLanguage(this->language);
    sourcesLabel->setText(this->language->getSourcesLabel());
    sourceWidget->updateLanguage(this->language);
}

SettingsWidget::~SettingsWidget(){
    delete sourceWidget;
    delete sourcesLabel;
    delete categoryWidget;
    delete categoriesLabel;
    delete formWidget;
    delete generalLabel;
    delete scrollAreaContainerLayout;
    delete scrollAreaContainer;
    delete scrollArea;
    delete layout;
    delete language;
}