#ifndef SETTINGSWIDGETFORM_HPP
#define SETTINGSWIDGETFORM_HPP

#include<core/language.hpp>
#include<ui/settingswidgetitems.hpp>
#include<QWidget>
#include<QFormLayout>
#include<QCheckBox>
#include<QSlider>
#include<QComboBox>

class SettingsWidgetForm : public QWidget{
    Q_OBJECT
public:
    explicit SettingsWidgetForm(Language *lang, QWidget * parent = nullptr);
    ~SettingsWidgetForm();
    void updateLanguage(Language * lang);
public slots:
    //slider slots
    void newsLimitSliderSlot(int value);
    void newsAutoRemoveDaysSliderSlot(int value);
    void newsAutoRefreshPeriodSliderSlot(int value);
    void newsAudioNotificationVolumeSliderSlot(int value);

signals:
    void languageChanged();

private:
    void ui_configure();
    void ui_mainlayout_init();
    bool languageExternalCheckBoxPressed;
    QStringList getFilesFromDir(const QString & path);
    void addElementsToComboBox(QComboBox * combobox, const QStringList & elements);

    // Languge
    Language * language = nullptr;


    // ** UI elements **
    
    QFormLayout * mainLayout = nullptr;
    
    // checkbox

    QCheckBox * generalMinimizeAfterExitCheckBox = nullptr;
    QCheckBox * languageExternalCheckBox = nullptr; // for looking for external translation
    QCheckBox * newsAutoRefreshCheckBox = nullptr;
    QCheckBox * newsAutoRemoveCheckBox = nullptr;
    QCheckBox * newsAudioNotificationCheckBox = nullptr;
    
    // slider
    
    SettingsWidgetSliderWithLabel * newsAudioNotificationVolumeSlider = nullptr;
    SettingsWidgetSliderWithLabel* newsLimitSlider = nullptr;
    SettingsWidgetSliderWithLabel * newsAutoRemoveDaysSlider = nullptr;
    SettingsWidgetSliderWithLabel * newsAutoRefreshPeriodSlider = nullptr;
    
    // combobox
    
    QComboBox * languageComboBox = nullptr;
    QComboBox * newsAudioNotificationFileComboBox = nullptr;

};

#endif