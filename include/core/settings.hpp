#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <core/jsonableinterface.hpp>


class Settings : JsonableInterface<Settings>{
public:
    Settings();

    QJsonObject ObjectToJsonObject() override;
    Settings FromJsonObjectToObject(const QJsonObject & json) override;

    /*
        Getters and Setters
    */
    bool getGeneralMinimizeAfterExit() const {
        return general_minimize_after_exit;
    }

    void setGeneralMinimizeAfterExit(bool minimize) {
        general_minimize_after_exit = minimize;
    }

    QString getLanguage() const {
        return language;
    }

    void setLanguage(const QString &lang) {
        language = lang;
    }

    bool getLanguageExternal() const {
        return language_external;
    }

    void setLanguageExternal(bool external) {
        language_external = external;
    }

    int getNewsLimit() const {
        return news_limit;
    }

    void setNewsLimit(int limit) {
        news_limit = limit;
    }

    bool getNewsAutoRefresh() const {
        return news_auto_refresh;
    }

    void setNewsAutoRefresh(bool refresh) {
        news_auto_refresh = refresh;
    }

    bool getNewsAutoRemove() const {
        return news_auto_remove;
    }

    void setNewsAutoRemove(bool remove) {
        news_auto_remove = remove;
    }

    int getNewsAutoRemoveDays() const {
        return news_auto_remove_days;
    }

    void setNewsAutoRemoveDays(int days) {
        news_auto_remove_days = days;
    }

    bool getNewsAudioNotification() const {return news_audio_notification;}
    void setNewsAudioNotification(bool notification) {news_audio_notification = notification;}

    QString getNewsAudioNotificationPath() const {return news_audio_notification_path;}
    void setNewsAudioNotificationPath(const QString &path){news_audio_notification_path = path;}

    int getNewsAutoRefreshPeriod()const {return news_auto_refresh_period;}
    void setNewsAutoRefreshPeriod(const int & value){news_auto_refresh_period = value;}

    void setNewsAudioNotificationVolume(const float & value){news_audio_notification_volume = value;}
    float getNewsAudioNotificationVolume(){return news_audio_notification_volume;}

private:
    bool    general_minimize_after_exit;
    QString language;
    bool    language_external;
    // <25, 100> Limit news per page
    int     news_limit;
    bool    news_auto_refresh;
    // <15, 60> Value is in minutes
    int     news_auto_refresh_period; // minutes
    bool    news_auto_remove;
    // <1,360>  Value is in days
    int     news_auto_remove_days;  // days
    bool    news_audio_notification;
    // <0,1>  Float type between 0 and 1
    float   news_audio_notification_volume;
    QString news_audio_notification_path;
};


#endif
