#include<core/settings.hpp>

/*
    Basic constructor
*/
Settings::Settings(){}


/*
    From QJsonObject to Settings object

    If setting key is missing, set default value.
*/
Settings Settings::FromJsonObjectToObject(const QJsonObject & json){
    Settings settings;
    
    if (json.contains("general_minimize_after_exit") && json["general_minimize_after_exit"].isBool()) {
        settings.general_minimize_after_exit = json["general_minimize_after_exit"].toBool();
    } else {
        settings.general_minimize_after_exit = true;
    }
    
    if (json.contains("language") && json["language"].isString()) {
    // correction if value is invalid
        QString value = json["language"].toString();
        if(value.isNull()||value.isEmpty()){settings.language = ":/lang/pl-pl.json";}
        else{settings.language = value;}
    } else {
        settings.language = ":/lang/pl-pl.json";
    }
    
    if (json.contains("language_external") && json["language_external"].isBool()) {
        settings.language_external = json["language_external"].toBool();
    } else {
        settings.language_external = false;
    }
    
    if (json.contains("news_limit") && json["news_limit"].isDouble()) {
        // correction if value is invalid
        int value = json["news_limit"].toInt();
        if(value<25){settings.news_limit = 25;}
        else if(value > 100){settings.news_limit = 100;}
        else{settings.news_limit = value;}
    } else {
        settings.news_limit = 30;
    }
    
    if (json.contains("news_auto_refresh") && json["news_auto_refresh"].isBool()) {
        settings.news_auto_refresh = json["news_auto_refresh"].toBool();
    } else {
        settings.news_auto_refresh = true;
    }
    
    if (json.contains("news_auto_refresh_period") && json["news_auto_refresh_period"].isDouble()) {
        // correction if value is invalid
        int value = json["news_auto_refresh_period"].toInt();
        if(value < 15){settings.news_auto_refresh_period = 15;}
        else if(value>60){settings.news_auto_refresh_period = 60;}
        else{settings.news_auto_refresh_period = value;}
    } else {
        settings.news_auto_refresh_period = 15;
    }
    
    if (json.contains("news_auto_remove") && json["news_auto_remove"].isBool()) {
        settings.news_auto_remove = json["news_auto_remove"].toBool();
    } else {
        settings.news_auto_remove = true;
    }
    
    if (json.contains("news_auto_remove_days") && json["news_auto_remove_days"].isDouble()) {
        // correction if value is invalid
        int value = json["news_auto_remove_days"].toInt();
        if(value < 1){settings.news_auto_remove_days=1;}
        else if(value>360){settings.news_auto_remove_days=360;}
        else{settings.news_auto_remove_days = value;}
    } else {
        settings.news_auto_remove_days = 30;
    }
    
    if (json.contains("news_audio_notification") && json["news_audio_notification"].isBool()) {
        settings.news_audio_notification = json["news_audio_notification"].toBool();
    } else {
        settings.news_audio_notification = true;
    }
    
    if (json.contains("news_audio_notification_volume") && json["news_audio_notification_volume"].isDouble()){
        // correction if value is invalid
        float value = json["news_audio_notification_volume"].toDouble();
        if(value > 1){settings.news_audio_notification_volume = 1.0f;}
        else if(value < 0){settings.news_audio_notification_volume = 0.0f;}
        else{settings.news_audio_notification_volume = value;}
    }else{
        settings.news_audio_notification_volume = 0.75f;
    }

    if (json.contains("news_audio_notification_path") && json["news_audio_notification_path"].isString()) {
        // correction if value is invalid
        QString value = json["news_audio_notification_path"].toString();
        if(value.isNull() || value.isEmpty()){
            settings.news_audio_notification_path = ":/audio/notify-1.wav";
        }else{settings.news_audio_notification_path = value;}
    } else {
        settings.news_audio_notification_path = ":/audio/notify-1.wav";
    }
    
    return settings;
    
    /*
    Settings settings;
    if (!json.contains("general_minimize_after_exit") || !json.contains("language") || 
            !json.contains("language_external") || !json.contains("news_limit") || 
            !json.contains("news_auto_refresh") || !json.contains("news_auto_remove") ||
            !json.contains("news_auto_remove_days") || !json.contains("news_audio_notification") || 
            !json.contains("news_audio_notification_path")) {
        throw std::runtime_error("[config.json][settings]\tMissing field in JSON object");
    }
    settings.general_minimize_after_exit = json["general_minimize_after_exit"].toBool();
    settings.language = json["language"].toString();
    settings.language_external = json["language_external"].toBool();
    settings.news_limit = json["news_limit"].toInt();
    settings.news_auto_refresh = json["news_auto_refresh"].toBool();
    settings.news_auto_remove = json["news_auto_remove"].toBool();
    settings.news_auto_remove_days = json["news_auto_remove_days"].toInt();
    settings.news_audio_notification = json["news_audio_notification"].toBool();
    settings.news_audio_notification_path = json["news_audio_notification_path"].toString();
    return settings;*/
}


/*
    Object to QJsonObject
*/
QJsonObject Settings::ObjectToJsonObject(){
    QJsonObject main;
    main.insert("general_minimize_after_exit",      getGeneralMinimizeAfterExit());
    main.insert("language",                         getLanguage());
    main.insert("language_external",                getLanguageExternal());
    main.insert("news_limit",                       getNewsLimit());
    main.insert("news_auto_refresh",                getNewsAutoRefresh());
    main.insert("news_auto_refresh_period",         getNewsAutoRefreshPeriod());
    main.insert("news_auto_remove",                 getNewsAutoRemove());
    main.insert("news_auto_remove_days",            getNewsAutoRemoveDays());
    main.insert("news_audio_notification",          getNewsAudioNotification());
    main.insert("news_audio_notification_volume",   getNewsAudioNotificationVolume());
    main.insert("news_audio_notification_path",     getNewsAudioNotificationPath());
    return main;
}