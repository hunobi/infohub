#ifndef SETTINGSWIDGETITEMS_HPP
#define SETTINGSWIDGETITEMS_HPP
#include<core/language.hpp>
#include<core/source.hpp>
#include<QWidget>
#include<QLabel>
#include<QPixmap>
#include<QHBoxLayout>
#include<QSlider>
#include<QLineEdit>
#include<QPushButton>
#include<QListWidget>
#include<QTreeWidget>
#include<QDialog>

class SettingsWidgetCategorySpacer : public QWidget{
    Q_OBJECT

public:
    //explicit SettingsWidgetCategorySpacer(QWidget *parent = nullptr);
    explicit SettingsWidgetCategorySpacer(const QString & imgPath, const QString & txt, QWidget *parent = nullptr);
    ~SettingsWidgetCategorySpacer();
    void init(const QString & imgPath, const QString & txt);
    void setText(const QString & txt);
private:
    QHBoxLayout * layout = nullptr;
    QLabel * image = nullptr;
    QLabel * text = nullptr;
};


class SettingsWidgetSliderWithLabel : public QWidget{
    Q_OBJECT

public:
    //explicit SettingsWidgetCategorySpacer(QWidget *parent = nullptr);
    explicit SettingsWidgetSliderWithLabel(const QString & label, int initValue, QWidget *parent = nullptr);
    ~SettingsWidgetSliderWithLabel();
    QSlider * getSlider(){return this->slider;}
    int getSliderValue(){return this->slider->value();}
    void setValues(int value);
    void setLabelText(const QString & txt){this->label->setText(txt);}
private:
    QHBoxLayout * layout = nullptr;
    QLabel * label = nullptr;
    QSlider * slider = nullptr;
    QLabel * text = nullptr;
signals:
    void valueHasBeenChanged(int value);
};

/*
        CATEGORY
*/

class SettingsWidgetCategoryImageChanger : public QLabel{
    Q_OBJECT
public:
    explicit SettingsWidgetCategoryImageChanger(QLabel *parent = nullptr);
    explicit SettingsWidgetCategoryImageChanger(const QString & path, QLabel *parent = nullptr);
    void resetImage();
    QString getCurrentImagePath() const;
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void changeImage(const QString & imagePath);
private:
    QString currentImagePath;
    QString firstImagePath;
};

class SettingsWidgetCategoryAdderForm : public QWidget{
    Q_OBJECT

public:
    explicit SettingsWidgetCategoryAdderForm(Language * lang, QWidget *parent = nullptr);
    ~SettingsWidgetCategoryAdderForm();
    void updateLanguage(Language * lang);
signals:
    void categoryAdded();

private slots:
    void addCategory();
    void clearForm();
    void changedImage(const QString & imagePath);

private:
    Language * language = nullptr;
    QVBoxLayout * layout = nullptr;
    SettingsWidgetCategoryImageChanger * image = nullptr;
    QLineEdit * inputText = nullptr;
    QPushButton * saveButton = nullptr;
    QPushButton * cancelButton = nullptr;
};

class SettingsWidgetCategoryList : public QListWidget{
    Q_OBJECT
public:
    explicit SettingsWidgetCategoryList(Language * lang, QListWidget * parent = nullptr);
    ~SettingsWidgetCategoryList();
    void updateLanguage(Language * lang);

protected:
    void dropEvent(QDropEvent * event) override;

public slots:
    void updateList();

private slots:
    void orderChanged();

signals:
    void categoryUpdated();

protected:
    void contextMenuEvent(QContextMenuEvent * event) override;

private:
    Language * language = nullptr;
    void clearItemList();
    void loadItemList();
    QList<QListWidgetItem*> * itemList = nullptr;
};

/*
        SOURCE
*/

class SettingsWidgetSourceAdderForm : public QWidget{
    Q_OBJECT

public:
    explicit SettingsWidgetSourceAdderForm(Language * lang, QWidget *parent = nullptr);
    ~SettingsWidgetSourceAdderForm();
    void updateLanguage(Language * lang);

signals:
    void sourceAdded();

private slots:
    void addSource();
    void clearForm();

private:
    Language * language = nullptr;
    QVBoxLayout * layout = nullptr;
    QLineEdit * inputName = nullptr;
    QLineEdit * inputAddress = nullptr;
    QPushButton * saveButton = nullptr;
    QPushButton * cancelButton = nullptr;
};

class SourceTreeParentItemDialog : public QDialog{
public:
    explicit SourceTreeParentItemDialog(Source * src, Language * lang, QWidget * parent = nullptr);
    ~SourceTreeParentItemDialog();
    QString getName();
    QString getAddress();
    const bool & isAccept();
private:
    bool acceptStatus = false;
    Language * language = nullptr;
    QLineEdit * name = nullptr;
    QLineEdit * address = nullptr;
    QPushButton *okButton = nullptr;
    QPushButton *cancelButton = nullptr;
    QVBoxLayout *layout = nullptr;
};


class SourceTreeParentItem : public QTreeWidgetItem{
public:
    explicit SourceTreeParentItem(const QString & text, Language * lang, QTreeWidget * parent = nullptr);
    explicit SourceTreeParentItem(const QString & text, Language * lang, QTreeWidgetItem * parent = nullptr);
    void showContextMenu(const QPoint & point);
private:
    Language * language;
};


class SourceTreeChildItem : public QTreeWidgetItem{
public:
    explicit SourceTreeChildItem(const QString & text, Language * lang, QTreeWidgetItem * parent = nullptr);
    void showContextMenu(const QPoint & point);
private:
    Language * language;  
};


class SettingsWidgetSourceTree : public QTreeWidget{
    Q_OBJECT
public:
    explicit SettingsWidgetSourceTree(Language * lang, QTreeWidget * parent = nullptr);
    ~SettingsWidgetSourceTree();
    void updateLanguage(Language * lang);


public slots:
    /*
        Changing the category requires updating the source tree, because the tree contains the category name.
    */
    void sourceHasBeenChanged();

protected:
    void contextMenuEvent(QContextMenuEvent * event) override;

private:
    void clearTree();
    void loadTree();

    Language * language = nullptr;
};

#endif