#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <QString>
#include <QJsonObject>

class Language
{
public: 
    
    /*
        This static method checks if the file containing the translation is valid and complete.
    */
    static bool isValid(const QString & path);
    
    Language(const QString & path);
    
    // Getters:
    QString getGeneralLabel() const {
        return translation.value("SETTINGS_GENERAL_LABEL").toString();
    }

    QString getGeneralMinimizeAfterExit() const {
        return translation.value("SETTINGS_GENERAL_MINIMIZE_AFTER_EXIT").toString();
    }

    QString getGeneralLanguageExternal() const {
        return translation.value("SETTINGS_GENERAL_LANGUAGE_EXTERNAL").toString();
    }

    QString getGeneralLanguage() const {
        return translation.value("SETTINGS_GENERAL_LANGUAGE").toString();
    }

    QString getGeneralNewsLimit() const {
        return translation.value("SETTINGS_GENERAL_NEWS_LIMIT").toString();
    }

    QString getGeneralNewsAutoRemove() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUTO_REMOVE").toString();
    }

    QString getGeneralNewsAutoRemoveDays() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUTO_REMOVE_DAYS").toString();
    }

    QString getGeneralNewsAutoRefresh() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUTO_REFRESH").toString();
    }

    QString getGeneralNewsAutoRefreshPeriod() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUTO_REFRESH_PERIOD").toString();
    }

    QString getGeneralNewsAudioNotification() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUDIO_NOTIFICATION").toString();
    }

    QString getGeneralNewsAudioNotificationVolume() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUDIO_NOTIFICATION_VOLUME").toString();
    }

    QString getGeneralNewsAudioNotificationFile() const {
        return translation.value("SETTINGS_GENERAL_NEWS_AUDIO_NOTIFICATION_FILE").toString();
    }

    QString getCategoriesLabel() const {
        return translation.value("SETTINGS_CATEGORIES_LABEL").toString();
    }

    QString getCategoriesAdderInputNamePlaceholder() const {
        return translation.value("SETTINGS_CATEGORIES_ADDER_INPUT_NAME_PLACEHOLDER").toString();
    }

    QString getCategoriesAdderButtonSave() const {
        return translation.value("SETTINGS_CATEGORIES_ADDER_BUTTON_SAVE").toString();
    }

    QString getCategoriesAdderButtonClear() const {
        return translation.value("SETTINGS_CATEGORIES_ADDER_BUTTON_CLEAR").toString();
    }

    QString getCategoriesListActionEditIcon() const {
        return translation.value("SETTINGS_CATEGORIES_LIST_ACTION_EDIT_ICON").toString();
    }

    QString getCategoriesListActionEditName() const {
        return translation.value("SETTINGS_CATEGORIES_LIST_ACTION_EDIT_NAME").toString();
    }

    QString getCategoriesListActionDelete() const {
        return translation.value("SETTINGS_CATEGORIES_LIST_ACTION_DELETE").toString();
    }

    QString getCategoriesListDialogEditImageTitle() const {
        return translation.value("SETTINGS_CATEGORIES_LIST_DIALOG_EDIT_IMAGE_TITLE").toString();
    }

    QString getCategoriesListDialogEditNameTitle() const {
        return translation.value("SETTINGS_CATEGORIES_LIST_DIALOG_EDIT_NAME_TITLE").toString();
    }

    QString getCategoriesListDialogEditNameField() const {
        return translation.value("SETTINGS_CATEGORIES_LIST_DIALOG_EDIT_NAME_FIELD").toString();
    }

    QString getSourcesLabel() const {
        return translation.value("SETTINGS_SOURCES_LABEL").toString();
    }

    QString getSourcesAdderInputNamePlaceholder() const {
        return translation.value("SETTINGS_SOURCES_ADDER_INPUT_NAME_PLACEHOLDER").toString();
    }

    QString getSourcesAdderInputAddressPlaceholder() const {
        return translation.value("SETTINGS_SOURCES_ADDER_INPUT_ADDRESS_PLACEHOLDER").toString();
    }

    QString getSourcesAdderButtonSave() const {
        return translation.value("SETTINGS_SOURCES_ADDER_BUTTON_SAVE").toString();
    }

    QString getSourcesAdderButtonClear() const {
        return translation.value("SETTINGS_SOURCES_ADDER_BUTTON_CLEAR").toString();
    }

    QString getSettingsSourcesParentDialogNamePlaceholder() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_DIALOG_NAME_PLACEHOLDER").toString();
    }

    QString getSettingsSourcesParentDialogAddressPlaceholder() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_DIALOG_ADDRESS_PLACEHOLDER").toString();
    }

    QString getSettingsSourcesParentDialogSubmit() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_DIALOG_SUBMIT").toString();
    }

    QString getSettingsSourcesParentDialogCancel() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_DIALOG_CANCEL").toString();
    }

    QString getSettingsSourcesTreeParentActionEdit() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_ACTION_EDIT").toString();
    }

    QString getSettingsSourcesTreeParentActionSwitchOn() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_ACTION_SWITCH_ON").toString();
    }

    QString getSettingsSourcesTreeParentActionSwitchOff() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_ACTION_SWITCH_OFF").toString();
    }

    QString getSettingsSourcesTreeParentActionAddCategory() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_ACTION_ADD_CATEGORY").toString();
    }

    QString getSettingsSourcesTreeParentDialogAddCategoryTitle() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_DIALOG_ADD_CATEGORY_TITLE").toString();
    }

    QString getSettingsSourcesTreeParentDialogAddCategoryField() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_DIALOG_ADD_CATEGORY_FIELD").toString();
    }

    QString getSettingsSourcesTreeParentActionDelete() {
        return translation.value("SETTINGS_SOURCES_TREE_PARENT_ACTION_DELETE").toString();
    }

    QString getSettingsSourcesTreeChildActionDelete() {
        return translation.value("SETTINGS_SOURCES_TREE_CHILD_ACTION_DELETE").toString();
    }

private:
    QJsonObject translation;
};

#endif