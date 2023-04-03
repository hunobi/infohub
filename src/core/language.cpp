#include<core/language.hpp>
#include<QJsonDocument>
#include<QFile>
bool Language::isValid(const QString & path){
    // init all required keys
    const QStringList required_keys = {"SETTINGS_GENERAL_LABEL", "SETTINGS_GENERAL_MINIMIZE_AFTER_EXIT", "SETTINGS_GENERAL_LANGUAGE_EXTERNAL", "SETTINGS_GENERAL_LANGUAGE", "SETTINGS_GENERAL_NEWS_LIMIT", "SETTINGS_GENERAL_NEWS_AUTO_REMOVE", "SETTINGS_GENERAL_NEWS_AUTO_REMOVE_DAYS", "SETTINGS_GENERAL_NEWS_AUTO_REFRESH", "SETTINGS_GENERAL_NEWS_AUTO_REFRESH_PERIOD", "SETTINGS_GENERAL_NEWS_AUDIO_NOTIFICATION", "SETTINGS_GENERAL_NEWS_AUDIO_NOTIFICATION_VOLUME", "SETTINGS_GENERAL_NEWS_AUDIO_NOTIFICATION_FILE", "SETTINGS_CATEGORIES_LABEL", "SETTINGS_CATEGORIES_ADDER_INPUT_NAME_PLACEHOLDER", "SETTINGS_CATEGORIES_ADDER_BUTTON_SAVE", "SETTINGS_CATEGORIES_ADDER_BUTTON_CLEAR", "SETTINGS_CATEGORIES_LIST_ACTION_EDIT_ICON", "SETTINGS_CATEGORIES_LIST_ACTION_EDIT_NAME", "SETTINGS_CATEGORIES_LIST_ACTION_DELETE", "SETTINGS_CATEGORIES_LIST_DIALOG_EDIT_IMAGE_TITLE", "SETTINGS_CATEGORIES_LIST_DIALOG_EDIT_NAME_TITLE", "SETTINGS_CATEGORIES_LIST_DIALOG_EDIT_NAME_FIELD", "SETTINGS_SOURCES_LABEL", "SETTINGS_SOURCES_ADDER_INPUT_NAME_PLACEHOLDER", "SETTINGS_SOURCES_ADDER_INPUT_ADDRESS_PLACEHOLDER", "SETTINGS_SOURCES_ADDER_BUTTON_SAVE", "SETTINGS_SOURCES_ADDER_BUTTON_CLEAR", "SETTINGS_SOURCES_TREE_PARENT_DIALOG_NAME_PLACEHOLDER", "SETTINGS_SOURCES_TREE_PARENT_DIALOG_ADDRESS_PLACEHOLDER", "SETTINGS_SOURCES_TREE_PARENT_DIALOG_SUBMIT", "SETTINGS_SOURCES_TREE_PARENT_DIALOG_CANCEL", "SETTINGS_SOURCES_TREE_PARENT_ACTION_EDIT", "SETTINGS_SOURCES_TREE_PARENT_ACTION_SWITCH_ON", "SETTINGS_SOURCES_TREE_PARENT_ACTION_SWITCH_OFF", "SETTINGS_SOURCES_TREE_PARENT_ACTION_ADD_CATEGORY", "SETTINGS_SOURCES_TREE_PARENT_DIALOG_ADD_CATEGORY_TITLE", "SETTINGS_SOURCES_TREE_PARENT_DIALOG_ADD_CATEGORY_FIELD", "SETTINGS_SOURCES_TREE_PARENT_ACTION_DELETE", "SETTINGS_SOURCES_TREE_CHILD_ACTION_DELETE"};
    QFile file(path);
    if(!file.exists()){return false;}
    if(file.open(QFile::ReadOnly)){
        QByteArray data = file.readAll();
        file.close();
        QJsonDocument json = QJsonDocument::fromJson(data);
        if(!json.isNull() && json.isObject()){
            QJsonObject core = json.object();
            /*
                Check json if contains all required keys
            */
            for(QString key : required_keys){
                if(!core.contains(key)){
                    return false;
                }
            }
            return true;
        }// end if: json checker
    }// end if: file open
    return false;

}

Language::Language(const QString & path){
    QFile file(path);
    if(!Language::isValid(path)){
        throw "Translation file is invalid.";
    }
    if(file.open(QFile::ReadOnly)){
        this->translation = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }else{
        qDebug() << "The translation file cannot be opened.";
    }
}

