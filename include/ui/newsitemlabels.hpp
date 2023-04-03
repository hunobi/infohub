#ifndef NEWSITEMLABELS_HPP
#define NEWSITEMLABELS_HPP
#include <QLabel>

    class NewsItemTitle : public QLabel{
        Q_OBJECT
    public:
        explicit NewsItemTitle(QWidget *parent = nullptr);
    protected:
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    signals:
        void openUrl();
    };


    class NewsItemDescription : public QLabel{
        Q_OBJECT
    public:
        explicit NewsItemDescription(QWidget *parent = nullptr);
    protected:
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        //void mousePressEvent(QMouseEvent *event) override;
    };

    class NewsItemImage : public QLabel{
        Q_OBJECT
    public:
        explicit NewsItemImage(QWidget *parent = nullptr);
    protected:
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    signals:
        void openUrl();
    };

    class NewsItemFavoriteButton : public QLabel{
        Q_OBJECT
    public:
        explicit NewsItemFavoriteButton(QWidget *parent = nullptr);
    protected:
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    signals:
        void favoriteButtonClicked();
    };

    class NewsItemHideButton : public QLabel{
        Q_OBJECT
    public:
        explicit NewsItemHideButton(QWidget *parent = nullptr);
    protected:
        void enterEvent(QEnterEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
    signals:
        void hideButtonClicked();
    };
#endif