#include<core/config.hpp>
#include<ui/settingswidgetform.hpp>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QDirIterator>
#include <QFile>
#include<ui/mainview.hpp>

SettingsWidgetForm::SettingsWidgetForm(Language *lang, QWidget *parent): QWidget(parent){
    Settings cfg = ConfigSingleton::getInstance().GetConfig().getSettings();
    this->languageExternalCheckBoxPressed = false;
    language = lang;
    mainLayout = new QFormLayout();
    mainLayout->setAlignment(Qt::AlignCenter);
    // ui
    generalMinimizeAfterExitCheckBox = new QCheckBox(this->language->getGeneralMinimizeAfterExit());
    languageExternalCheckBox = new QCheckBox(this->language->getGeneralLanguageExternal());
    newsAutoRefreshCheckBox = new QCheckBox(this->language->getGeneralNewsAutoRefresh());
    newsAutoRemoveCheckBox = new QCheckBox(this->language->getGeneralNewsAutoRemove());
    newsAudioNotificationCheckBox = new QCheckBox(this->language->getGeneralNewsAudioNotification());
    newsAudioNotificationVolumeSlider = new SettingsWidgetSliderWithLabel(this->language->getGeneralNewsAudioNotificationVolume(),cfg.getNewsAudioNotificationVolume()*100.0f);
    newsLimitSlider = new SettingsWidgetSliderWithLabel(this->language->getGeneralNewsLimit(),cfg.getNewsLimit());
    newsAutoRemoveDaysSlider = new SettingsWidgetSliderWithLabel(this->language->getGeneralNewsAutoRemoveDays(),cfg.getNewsAutoRemoveDays());
    newsAutoRefreshPeriodSlider = new SettingsWidgetSliderWithLabel(this->language->getGeneralNewsAutoRefreshPeriod(),cfg.getNewsAutoRefreshPeriod());
    languageComboBox  = new QComboBox();
    newsAudioNotificationFileComboBox = new QComboBox();
    // configure
    this->ui_configure();
    this->ui_mainlayout_init();
    // show all
    setLayout(mainLayout);
}

void SettingsWidgetForm::updateLanguage(Language * lang){
    this->language = lang;
    generalMinimizeAfterExitCheckBox->setText(this->language->getGeneralMinimizeAfterExit());
    languageExternalCheckBox->setText(this->language->getGeneralLanguageExternal());
    newsAutoRefreshCheckBox->setText(this->language->getGeneralNewsAutoRefresh());
    newsAutoRemoveCheckBox->setText(this->language->getGeneralNewsAutoRemove());
    newsAudioNotificationCheckBox->setText(this->language->getGeneralNewsAudioNotification());
    newsAudioNotificationVolumeSlider->setLabelText(this->language->getGeneralNewsAudioNotificationVolume());
    newsLimitSlider->setLabelText(this->language->getGeneralNewsLimit());
    newsAutoRemoveDaysSlider->setLabelText(this->language->getGeneralNewsAutoRemoveDays());
    newsAutoRefreshPeriodSlider->setLabelText(this->language->getGeneralNewsAutoRefreshPeriod());

    qobject_cast<QLabel *>(this->mainLayout->labelForField(this->languageComboBox))->setText(this->language->getGeneralLanguage());
    qobject_cast<QLabel *>(this->mainLayout->labelForField(this->newsAudioNotificationFileComboBox))->setText(this->language->getGeneralNewsAudioNotificationFile());

}

// slider slots

void SettingsWidgetForm::newsLimitSliderSlot(int value){
    ConfigSingleton::getInstance().GetConfig().getSettings().setNewsLimit(value);
    ConfigSingleton::getInstance().Save();
}
void SettingsWidgetForm::newsAutoRemoveDaysSliderSlot(int value){
    ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAutoRemoveDays(value);
    ConfigSingleton::getInstance().Save();
}
void SettingsWidgetForm::newsAutoRefreshPeriodSliderSlot(int value){
    ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAutoRefreshPeriod(value);
    ConfigSingleton::getInstance().Save();
}
void SettingsWidgetForm::newsAudioNotificationVolumeSliderSlot(int value){
    ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAudioNotificationVolume(value/100.0f);
    ConfigSingleton::getInstance().Save();
}


/*
    Configure UI elements, set start values, ranges, connects etc..
*/
void SettingsWidgetForm::ui_configure(){
    this->setStyleSheet("QComboBox{color: #ababab;}QCheckBox{color:#c0c0c0;}QCheckBox::indicator{width:14px;height:14px;background-color:#626262;border:2px solid grey}QCheckBox::indicator:checked{background-color:#34f754;color:#34f754}QCheckBox::indicator:disabled{background-color:grey;border:2px solid #d3d3d3}");
    newsLimitSlider->getSlider()->setRange(25,100);
    newsAutoRemoveDaysSlider->getSlider()->setRange(1,360);
    newsAutoRefreshPeriodSlider->getSlider()->setRange(15,60);
    newsAudioNotificationVolumeSlider->getSlider()->setRange(0,100); // it needs scale to <0,1>

    languageComboBox->setMaximumWidth(200);
    newsAudioNotificationFileComboBox->setMaximumWidth(200);
    // init values from config
    Settings cfg = ConfigSingleton::getInstance().GetConfig().getSettings();
                        /*CheckBox*/
    generalMinimizeAfterExitCheckBox->setChecked(cfg.getGeneralMinimizeAfterExit());
    languageExternalCheckBox->setChecked(cfg.getLanguageExternal());
    newsAutoRefreshCheckBox->setChecked(cfg.getNewsAudioNotification());
    newsAutoRemoveCheckBox->setChecked(cfg.getNewsAutoRemove());
    newsAudioNotificationCheckBox->setChecked(cfg.getNewsAudioNotification());
    
                        /*COMBOBOX*/
    // load lang from qrc
    QStringList nativeLang = this->getFilesFromDir(":/lang");
    this->addElementsToComboBox(languageComboBox, nativeLang);
    
    // load lang from user dir - if user wants it.
    if(cfg.getLanguageExternal()){
        QStringList userLang = this->getFilesFromDir("./lang");
        this->addElementsToComboBox(languageComboBox, userLang);
    }

    // load autio from qrc
    QStringList nativeAudio = this->getFilesFromDir(":/audio");
    this->addElementsToComboBox(newsAudioNotificationFileComboBox, nativeAudio);
    // load audio from user dir
    QStringList userAudio = this->getFilesFromDir("./audio");
    this->addElementsToComboBox(newsAudioNotificationFileComboBox, userAudio);

    // set current 
    if(!cfg.getLanguage().isEmpty()){
        languageComboBox->setCurrentText(cfg.getLanguage().split("/").at(cfg.getLanguage().split("/").size()-1));
    }
    if(!cfg.getNewsAudioNotificationPath().isEmpty()){
        newsAudioNotificationFileComboBox->setCurrentText(cfg.getNewsAudioNotificationPath().split("/").at(cfg.getNewsAudioNotificationPath().split("/").size()-1));
    }
    // connect for language combobox
    /*
        Note:   
            p1,p2 path: Main difference is that first is from local dir, 
            but second is from qrc. User files have priority.
    */
    connect(languageComboBox, &QComboBox::currentTextChanged, [&](const QString & text){
        /*
            When the user changes the state of the languageExternalCheckBox, 
            this function is executed automatically.

            Automatic execution of this function generates an error. 
            My solution is to block that event, when the event of that checkbox is working.
        */
        if(this->languageExternalCheckBoxPressed){return;}
        /*
            Its ok, lets go...
        */

        QString p1("./lang/"+text);
        QString p2(":/lang/"+text);


        if(QFile::exists(p1)){
            ConfigSingleton::getInstance().GetConfig().getSettings().setLanguage(p1); 
            ConfigSingleton::getInstance().Save();
            emit languageChanged();
        }
        else if(QFile::exists(p2)){
            ConfigSingleton::getInstance().GetConfig().getSettings().setLanguage(p2); 
            ConfigSingleton::getInstance().Save();
            emit languageChanged();
        }
    });
    // connect for audio combobox
    connect(newsAudioNotificationFileComboBox, &QComboBox::currentTextChanged, [&](const QString & text){
        QString p1("./audio/"+text);
        QString p2(":/audio/"+text);
        
        if(QFile::exists(p1)){
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAudioNotificationPath(p1); 
            ConfigSingleton::getInstance().Save();
        }
        else if(QFile::exists(p2)){
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAudioNotificationPath(p2); 
            ConfigSingleton::getInstance().Save();
        }
    });
                        /*CHECKBOX*/
    connect(generalMinimizeAfterExitCheckBox, &QCheckBox::stateChanged, [&](int state){
        if(Qt::CheckState::Unchecked == state){
            ConfigSingleton::getInstance().GetConfig().getSettings().setGeneralMinimizeAfterExit(false);
        }else{
            ConfigSingleton::getInstance().GetConfig().getSettings().setGeneralMinimizeAfterExit(true);
        }
        ConfigSingleton::getInstance().Save();
    });
    connect(languageExternalCheckBox, &QCheckBox::stateChanged, [&](int state){
        languageExternalCheckBoxPressed = true;
        QString current = this->languageComboBox->currentText();
        this->languageComboBox->clear();

         // load lang from qrc
        QStringList nativeLang = this->getFilesFromDir(":/lang");

        if(Qt::CheckState::Unchecked == state){
            ConfigSingleton::getInstance().GetConfig().getSettings().setLanguageExternal(false);
        }else{
            // load lang from user dir - if user wants it.
            QStringList userLang = this->getFilesFromDir("./lang");
            for(QString item : userLang){
                if(!nativeLang.contains(item)){
                    nativeLang.append(item);
                }
            }
            ConfigSingleton::getInstance().GetConfig().getSettings().setLanguageExternal(true);
        }
        languageComboBox->addItems(nativeLang);
        languageComboBox->setCurrentText(current);
        ConfigSingleton::getInstance().Save();
        languageExternalCheckBoxPressed = false;
    });
    connect(newsAutoRefreshCheckBox, &QCheckBox::stateChanged, [&](int state){
        if(Qt::CheckState::Unchecked == state){
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAutoRefresh(false);
        }else{
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAutoRefresh(true);
        }
        ConfigSingleton::getInstance().Save();
    });
    connect(newsAutoRemoveCheckBox, &QCheckBox::stateChanged, [&](int state){
        if(Qt::CheckState::Unchecked == state){
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAutoRemove(false);
        }else{
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAutoRemove(true);
        }
        ConfigSingleton::getInstance().Save();
    });
    connect(newsAudioNotificationCheckBox, &QCheckBox::stateChanged, [&](int state){
        if(Qt::CheckState::Unchecked == state){
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAudioNotification(false);
        }else{
            ConfigSingleton::getInstance().GetConfig().getSettings().setNewsAudioNotification(true);
        }
        ConfigSingleton::getInstance().Save();
    });
    // connect slider
    connect(newsLimitSlider, &SettingsWidgetSliderWithLabel::valueHasBeenChanged ,this,&SettingsWidgetForm::newsLimitSliderSlot);
    connect(newsAutoRemoveDaysSlider, &SettingsWidgetSliderWithLabel::valueHasBeenChanged ,this,&SettingsWidgetForm::newsAutoRemoveDaysSliderSlot);
    connect(newsAutoRefreshPeriodSlider, &SettingsWidgetSliderWithLabel::valueHasBeenChanged ,this,&SettingsWidgetForm::newsAutoRefreshPeriodSliderSlot);
    connect(newsAudioNotificationVolumeSlider, &SettingsWidgetSliderWithLabel::valueHasBeenChanged ,this,&SettingsWidgetForm::newsAudioNotificationVolumeSliderSlot);
}

/*
    Add UI elements to layout
*/
void SettingsWidgetForm::ui_mainlayout_init(){
    // general
    this->mainLayout->addRow(generalMinimizeAfterExitCheckBox);
    this->mainLayout->addRow(languageExternalCheckBox);
    this->mainLayout->addRow(this->language->getGeneralLanguage(),languageComboBox);
    // news
    this->mainLayout->addRow(newsLimitSlider);
    this->mainLayout->addRow(newsAutoRemoveCheckBox);
    this->mainLayout->addRow(newsAutoRemoveDaysSlider);
    this->mainLayout->addRow(newsAutoRefreshCheckBox);
    this->mainLayout->addRow(newsAutoRefreshPeriodSlider);
    this->mainLayout->addRow(newsAudioNotificationCheckBox);
    this->mainLayout->addRow(newsAudioNotificationVolumeSlider);
    this->mainLayout->addRow(this->language->getGeneralNewsAudioNotificationFile(),newsAudioNotificationFileComboBox);
    qobject_cast<QLabel *>(this->mainLayout->labelForField(this->languageComboBox))->setTextFormat(Qt::TextFormat::RichText);
    qobject_cast<QLabel *>(this->mainLayout->labelForField(this->languageComboBox))->setStyleSheet("color:#c0c0c0;");
    qobject_cast<QLabel *>(this->mainLayout->labelForField(this->newsAudioNotificationFileComboBox))->setTextFormat(Qt::TextFormat::RichText);
    qobject_cast<QLabel *>(this->mainLayout->labelForField(this->newsAudioNotificationFileComboBox))->setStyleSheet("color:#c0c0c0;");
}

/*
    Helpful method - get files from dirs
*/
QStringList SettingsWidgetForm::getFilesFromDir(const QString & path){
    QStringList list;
    QDirIterator file(path, QDirIterator::Subdirectories);
    while (file.hasNext()) {
        QString tmp = file.next();
        QStringList tmp_a = tmp.split("/");
        QString lastSubName = tmp_a.at(tmp_a.size()-1);
        if(lastSubName=="." || lastSubName == ".."){continue;}
        list.append(lastSubName);
    }
    return list;
}

/*
    Add String element from list to combobox
*/
void SettingsWidgetForm::addElementsToComboBox(QComboBox * combobox, const QStringList & elements){
    for(QString elem: elements){
        combobox->addItem(elem);
    }
}

/*
    Bye Bye object
*/
SettingsWidgetForm::~SettingsWidgetForm(){
    delete generalMinimizeAfterExitCheckBox;
    delete languageExternalCheckBox;
    delete newsAutoRefreshCheckBox;
    delete newsAutoRemoveCheckBox;
    delete newsAudioNotificationCheckBox;
    delete newsAudioNotificationVolumeSlider ;
    delete newsLimitSlider;
    delete newsAutoRemoveDaysSlider;
    delete newsAutoRefreshPeriodSlider;
    delete languageComboBox ;
    delete newsAudioNotificationFileComboBox;
    delete mainLayout ;
}