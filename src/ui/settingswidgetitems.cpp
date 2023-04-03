#include<core/utils.hpp>
#include<core/config.hpp>
#include<ui/settingswidgetitems.hpp>
#include<QFileDialog>
#include<QContextMenuEvent>
#include<QMenu>
#include<QAction>
#include<QInputDialog>

SettingsWidgetCategorySpacer::SettingsWidgetCategorySpacer(const QString & imgPath, const QString & txt, QWidget *parent): QWidget(parent){
    this->layout = new QHBoxLayout();
    this->image = new QLabel();
    this->text = new QLabel();
    this->init(imgPath, txt);
    this->layout->addWidget(this->image);
    this->layout->addSpacing(20);
    this->layout->addWidget(this->text);
    setLayout(this->layout);
}

void SettingsWidgetCategorySpacer::init(const QString & imgPath, const QString & txt){
    this->image->setMinimumSize(64,64);
    this->image->setMaximumSize(64,64);
    this->image->setScaledContents(true);
    this->image->setPixmap(QPixmap(imgPath));
    this->text->setScaledContents(true);
    this->text->setTextFormat(Qt::TextFormat::RichText);
    this->text->setWordWrap(true);
    this->text->setStyleSheet("font-size: 32px; color: #a1a1a1; font-family: Times;");
    this->text->setText(txt);
}

void SettingsWidgetCategorySpacer::setText(const QString & txt){
    this->text->setText(txt);
}

SettingsWidgetCategorySpacer::~SettingsWidgetCategorySpacer(){
    delete image;
    delete text;
    delete layout;
}

SettingsWidgetSliderWithLabel::SettingsWidgetSliderWithLabel(const QString & label, int initValue, QWidget *parent): QWidget(parent){
    // init
    this->layout = new QHBoxLayout();
    this->slider = new QSlider(Qt::Orientation::Horizontal);
    this->text = new QLabel();
    this->label = new QLabel();
    // label
    this->label->setTextFormat(Qt::TextFormat::RichText);
    this->label->setScaledContents(true);
    this->label->setAlignment(Qt::AlignCenter);
    this->label->setStyleSheet("color: #c0c0c0;");
    this->label->setText(label);
    // layout
    this->layout->addWidget(this->label);
    this->layout->addSpacing(10);
    this->layout->addWidget(this->slider);
    this->layout->addSpacing(10);
    this->layout->addWidget(this->text);
    // slider config
    this->slider->setStyleSheet("margin:5px;");
    // label config
    this->text->setScaledContents(true);
    this->text->setAlignment(Qt::AlignCenter);
    this->text->setTextFormat(Qt::TextFormat::RichText);
    this->text->setFixedSize(64,48);
    this->text->setStyleSheet("color: #c0c0c0; padding: 5px; border-radius:10px; border: 1px solid #ababab;");
    // set values
    this->setValues(initValue);

    connect(this->slider, &QSlider::valueChanged, [&](int value){
        this->text->setText(QString::number(value));
        emit valueHasBeenChanged(value);
    });

    setLayout(this->layout);
}

void SettingsWidgetSliderWithLabel::setValues(int value){
    if(this->slider){
        this->slider->setValue(value);
    }
    if(this->text){
        this->text->setText(QString::number(value));
    }
}

SettingsWidgetSliderWithLabel::~SettingsWidgetSliderWithLabel(){
    delete this->text;
    delete this->slider;
    delete this->layout;
}

/*
    CATEGORY
*/

SettingsWidgetCategoryImageChanger::SettingsWidgetCategoryImageChanger(QLabel *parent) : QLabel(parent){
    setFixedSize(64,64);
    setScaledContents(true);
    this->firstImagePath =":/icons/default-category.png";
    this->currentImagePath = this->firstImagePath;
    setPixmap(QPixmap(this->firstImagePath));
}

SettingsWidgetCategoryImageChanger::SettingsWidgetCategoryImageChanger(const QString & path, QLabel *parent) : QLabel(parent){
    setFixedSize(48,48);
    setScaledContents(true);
    this->firstImagePath=path;
    this->currentImagePath=path;
    setPixmap(QPixmap(path));
}

void SettingsWidgetCategoryImageChanger::enterEvent(QEnterEvent *event){
    QLabel::enterEvent(event);  
    setCursor(Qt::PointingHandCursor);  
}

void SettingsWidgetCategoryImageChanger::leaveEvent(QEvent *event){
    QLabel::leaveEvent(event);  
    setCursor(Qt::ArrowCursor);
}

void SettingsWidgetCategoryImageChanger::resetImage(){
    this->currentImagePath=this->firstImagePath;
    setPixmap(QPixmap(this->firstImagePath));
}

QString SettingsWidgetCategoryImageChanger::getCurrentImagePath() const {return this->currentImagePath;}


void SettingsWidgetCategoryImageChanger::mousePressEvent(QMouseEvent *event){
    /*
        Load image from system
    */
    this->currentImagePath = QFileDialog::getOpenFileName(this,"Open Image", ".", tr("Image Files (*.png *.jpg *.bmp)"));
    if(currentImagePath.isEmpty()){return;}
    /*
        Copy image to assets, skip copy if filename is exist in assets dir and select 
        image in same filename.
    */
    QFile img(this->currentImagePath);
    QDir dir("./assets");
    QString assetPath = dir.absolutePath()+"/"+img.fileName().section("/",-1,-1);
    if(assetPath!=this->currentImagePath){
        QFile::copy(this->currentImagePath,assetPath);
    }
    this->currentImagePath="./assets/"+img.fileName().section("/",-1,-1);        
    /*
        Set image and emit
    */
    setPixmap(QPixmap(this->currentImagePath));
    emit changeImage(this->currentImagePath);
}


SettingsWidgetCategoryAdderForm::SettingsWidgetCategoryAdderForm(Language * lang, QWidget * parent) : QWidget(parent){
    this->language = lang;
    layout = new QVBoxLayout();
    image = new SettingsWidgetCategoryImageChanger();
    inputText = new QLineEdit("");
    saveButton = new QPushButton(this->language->getCategoriesAdderButtonSave());
    cancelButton = new QPushButton(this->language->getCategoriesAdderButtonClear());

    inputText->setStyleSheet("color: #c0c0c0; border: 1px solid gray; padding:5px");
    inputText->setPlaceholderText(this->language->getCategoriesAdderInputNamePlaceholder());
    saveButton->setStyleSheet("color: #c0c0c0;");
    cancelButton->setStyleSheet("color: #c0c0c0;");
    
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(image,0,Qt::AlignCenter);
    layout->addWidget(inputText);
    layout->addWidget(saveButton);
    layout->addWidget(cancelButton);

    connect(this->cancelButton, &QPushButton::pressed, this, &SettingsWidgetCategoryAdderForm::clearForm);
    connect(this->saveButton, &QPushButton::pressed, this, &SettingsWidgetCategoryAdderForm::addCategory);
    connect(this->image, &SettingsWidgetCategoryImageChanger::changeImage, this, &SettingsWidgetCategoryAdderForm::changedImage);
    setLayout(this->layout);
}

void SettingsWidgetCategoryAdderForm::updateLanguage(Language * lang){
    this->language = lang;
    saveButton->setText(this->language->getCategoriesAdderButtonSave());
    cancelButton->setText(this->language->getCategoriesAdderButtonClear());
    inputText->setPlaceholderText(this->language->getCategoriesAdderInputNamePlaceholder());
}

void SettingsWidgetCategoryAdderForm::addCategory(){
    auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
    // check text if unique and not empty
    if(this->inputText->text().isEmpty() || categories.contains(this->inputText->text())){return;}
    Category newCategory;
    newCategory.setName(this->inputText->text());
    newCategory.setIconPath(this->image->getCurrentImagePath());

    // get categories and sort
    Category * categoryList = new Category[categories.size()];
    int i = 0;
    for(i; i < categories.size(); i++){
        categoryList[i] = categories.values()[i];
    }
    std::sort(categoryList, categoryList+categories.size(), [](Category &c1, Category &c2){
        return c1.getPosition() < c2.getPosition();
    });
    
    // correct numeration (change current nummeration for 1,2,3...n)
    // this correction is needed when user delete category
      
    for(i = 0; i < categories.size(); i++){
        categories[categoryList[i].getName()].setPosition(i+1);
    }
    // set position
    newCategory.setPosition(categories.size()+1);
    // add
    categories.insert(newCategory.getName(), newCategory);
    //save
    ConfigSingleton::getInstance().Save();
    // emit
    emit categoryAdded();
    delete [] categoryList;

    // back to default form values
    this->inputText->setText("");
    this->image->resetImage();
}

void SettingsWidgetCategoryAdderForm::clearForm(){
    inputText->setText("");
    image->resetImage();
}

void SettingsWidgetCategoryAdderForm::changedImage(const QString & imagePath){
    //qDebug() << imagePath;
}

SettingsWidgetCategoryAdderForm::~SettingsWidgetCategoryAdderForm(){
    delete cancelButton;
    delete saveButton;
    delete inputText;
    delete image;
    delete layout;
}

SettingsWidgetCategoryList::SettingsWidgetCategoryList(Language * lang, QListWidget * parent) : QListWidget(parent){
    this->language = lang;
    setFixedHeight(300);
    setDragDropMode(QAbstractItemView::InternalMove);
    setStyleSheet("color: white; QListWidget::item { font-size: 24px; };");
    this->itemList = new QList<QListWidgetItem*>();
    this->loadItemList();
}

void SettingsWidgetCategoryList::updateLanguage(Language * lang){
    this->language = lang;
}

void SettingsWidgetCategoryList::dropEvent(QDropEvent * event){
    event->setDropAction(Qt::MoveAction);
    // get list size
    int itemCount = count();
    int row = itemCount;

    // Check that the last element has been dropped to out of scope list size.
    // if yes then break function, else move element
    QPoint pos = event->position().toPoint();
    if (pos.y() >= visualItemRect(item(itemCount - 1)).bottom()){return;}
    QListWidget::dropEvent(event);
    // save changes
    this->orderChanged(); 
}

void SettingsWidgetCategoryList::loadItemList(){
    auto categories = ConfigSingleton::getInstance().GetConfig().getCategories();
    Category * categoryList = new Category[categories.size()];
    int i = 0;
    for(i; i < categories.size(); i++){
        categoryList[i] = categories.values()[i];
    }
    std::sort(categoryList, categoryList+categories.size(), [](Category &c1, Category &c2){
        return c1.getPosition() < c2.getPosition();
    });
    for(i = 0; i < categories.size(); i++){
        this->itemList->append(new QListWidgetItem(QIcon(categoryList[i].getIconPath()),categoryList[i].getName()));
        addItem(this->itemList->at(i));
    }

    delete [] categoryList;
}

void SettingsWidgetCategoryList::clearItemList(){
    while(!itemList->isEmpty()){
        QListWidgetItem* itemPtr = itemList->takeFirst();
        delete itemPtr;
    }
    this->itemList->squeeze();
    this->itemList->clear();
    clear();
}

void SettingsWidgetCategoryList::updateList(){
    this->clearItemList();
    this->loadItemList();
}

void SettingsWidgetCategoryList::orderChanged(){
    auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();

    // get all items
    QList<QListWidgetItem*> items = this->findItems("", Qt::MatchContains); 
    int itemCount = items.count();
    // change item position
    for(int i = 0; i < itemCount; i++){ 
        QString key = items.at(i)->text();
        categories[key].setPosition(i+1);
    }
    // save
    ConfigSingleton::getInstance().Save();
    // emit
    emit categoryUpdated();
}

void SettingsWidgetCategoryList::contextMenuEvent(QContextMenuEvent * event){
    QListWidgetItem *item = itemAt(event->pos());
    if (item) {
        QMenu menu;
        menu.setStyleSheet(
            "QMenu{"
            "background-color: rgb(4, 0, 24);"
            "color: rgb(210, 210, 210);"
            "border: 1px solid black;"
            "}"
            "QMenu::item{"
            "background-color: transparent;"
            "color: rgb(210, 210, 210);"
            "}"
            "QMenu::item:selected{"
            "background-color: rgb(48, 40, 88);"
            "color: rgb(210, 210, 210);"
            "}"
        );

        QAction *editIconAction = menu.addAction(this->language->getCategoriesListActionEditIcon());
        QAction *editTextAction = menu.addAction(this->language->getCategoriesListActionEditName());
        QAction *deleteAction = menu.addAction(this->language->getCategoriesListActionDelete());
        QAction *selectedAction = menu.exec(event->globalPos());
        
        if (selectedAction == editIconAction) {
            auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
            if(!categories.contains(item->text())){return;}// exit if not unique
            /*
                Load image from system
            */
            QString currentImagePath = QFileDialog::getOpenFileName(this,this->language->getCategoriesListDialogEditImageTitle(), ".", tr("Image Files (*.png *.jpg *.bmp)"));
            if(currentImagePath.isEmpty()){return;}
            /*
                Copy image to assets, skip copy if filename is exist in assets dir and select 
                image in same filename.
            */
            QFile img(currentImagePath);
            QDir dir("./assets");
            QString assetPath = dir.absolutePath()+"/"+img.fileName().section("/",-1,-1);
            if(assetPath!=currentImagePath){
                QFile::copy(currentImagePath,assetPath);
            }
            currentImagePath="./assets/"+img.fileName().section("/",-1,-1);  
            /*
                Set icon
            */
            item->setIcon(QIcon(currentImagePath));
            categories[item->text()].setIconPath(currentImagePath);
            ConfigSingleton::getInstance().Save();
            emit categoryUpdated();

        } else if (selectedAction == editTextAction) {
            bool ok;
            QString text = QInputDialog::getText(this, this->language->getCategoriesListDialogEditNameTitle(),
                                                this->language->getCategoriesListDialogEditNameField(), QLineEdit::Normal,
                                                item->text(), &ok);
            if (ok && !text.isEmpty()){ // text is not null and empty
                auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
                // check if new name is unique
                if(categories.contains(text)){return;}// exit if not unique

                // replace category - remove old name and add with new name
                Category tmp = categories[item->text()];
                tmp.setName(text);
                categories.remove(item->text());
                categories.insert(text, tmp);

                // replace category name in all sources
                auto & sources = ConfigSingleton::getInstance().GetConfig().getSources();
                for(QString key: sources.keys()){
                    int index = sources[key].getCategories().indexOf(item->text());
                    if(index > -1){
                        QStringList tmp_categories = sources[key].getCategories();
                        tmp_categories.removeAt(index);
                        tmp_categories.append(text);
                        sources[key].setCategories(tmp_categories);
                    }
                }

                // replace category name in all news
                auto & news = ConfigSingleton::getInstance().GetConfig().getNews();
                for(QString key: news.keys()){
                    int index = news[key].getCategories().indexOf(item->text());
                    if(index > -1){
                        QStringList tmp_categories = news[key].getCategories();
                        tmp_categories.removeAt(index);
                        tmp_categories.append(text);
                        news[key].setCategories(tmp_categories);
                    }
                }
                   
                ConfigSingleton::getInstance().Save();
                item->setText(tmp.getName());
                emit categoryUpdated();
            }
        }else if (selectedAction == deleteAction) {
            auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
            if(!categories.contains(item->text())){return;}
            // delete from category
            categories.remove(item->text());
            // delete from sources
            auto & sources = ConfigSingleton::getInstance().GetConfig().getSources();
            for(QString key: sources.keys()){
                int index = sources[key].getCategories().indexOf(item->text());
                if(index > -1){
                    QStringList tmp_categories = sources[key].getCategories();
                    tmp_categories.removeAt(index);
                    sources[key].setCategories(tmp_categories);
                }
            }
            // delete from news
            auto & news = ConfigSingleton::getInstance().GetConfig().getNews();
            for(QString key: news.keys()){
                int index = news[key].getCategories().indexOf(item->text());
                if(index > -1){
                    QStringList tmp_categories = news[key].getCategories();
                    tmp_categories.removeAt(index);
                    news[key].setCategories(tmp_categories);
                }
            }
            // save
            ConfigSingleton::getInstance().Save();
            emit categoryUpdated();
        }
    }
}

SettingsWidgetCategoryList::~SettingsWidgetCategoryList(){
    this->clearItemList();
    delete itemList;
    clear();
}


/*
    SOURCE
*/

SettingsWidgetSourceAdderForm::SettingsWidgetSourceAdderForm(Language * lang, QWidget * parent) : QWidget(parent){
    this->language = lang;
    layout = new QVBoxLayout();
    inputName = new QLineEdit("");
    inputAddress = new QLineEdit("");
    saveButton = new QPushButton(this->language->getSourcesAdderButtonSave());
    cancelButton = new QPushButton(this->language->getSourcesAdderButtonClear());

    inputName->setStyleSheet("color: #c0c0c0; border: 1px solid gray; padding:5px");
    inputName->setPlaceholderText(this->language->getSourcesAdderInputNamePlaceholder());

    inputAddress->setStyleSheet("color: #c0c0c0; border: 1px solid gray; padding:5px");
    inputAddress->setPlaceholderText(this->language->getSourcesAdderInputAddressPlaceholder());

    saveButton->setStyleSheet("color: #c0c0c0;");
    cancelButton->setStyleSheet("color: #c0c0c0;");
    
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(inputName);
    layout->addWidget(inputAddress);
    layout->addWidget(saveButton);
    layout->addWidget(cancelButton);

    connect(this->cancelButton, &QPushButton::pressed, this, &SettingsWidgetSourceAdderForm::clearForm);
    connect(this->saveButton, &QPushButton::pressed, this, &SettingsWidgetSourceAdderForm::addSource);
    setLayout(this->layout);
}

void SettingsWidgetSourceAdderForm::updateLanguage(Language * lang){
    this->language = lang;
    saveButton->setText(this->language->getSourcesAdderButtonSave());
    cancelButton->setText(this->language->getSourcesAdderButtonClear());
    inputName->setPlaceholderText(this->language->getSourcesAdderInputNamePlaceholder());
    inputAddress->setPlaceholderText(this->language->getSourcesAdderInputAddressPlaceholder());
}

void SettingsWidgetSourceAdderForm::addSource(){
    // check
    if(inputName->text().isEmpty() || inputAddress->text().isEmpty()){return;}
    auto & sources = ConfigSingleton::getInstance().GetConfig().getSources();
    if(sources.contains(inputName->text())){return;}
    // Now, we know that source name is unique and not empty. Lets add it to database.
    Source newSource(inputName->text(),inputAddress->text(), QStringList(), true);
    sources.insert(newSource.getName(), newSource);
    //save
    ConfigSingleton::getInstance().Save();
    // emit
    emit sourceAdded();
    // clean
    this->clearForm();
}

void SettingsWidgetSourceAdderForm::clearForm(){
    inputName->setText("");
    inputAddress->setText("");
}


SettingsWidgetSourceAdderForm::~SettingsWidgetSourceAdderForm(){
    delete cancelButton;
    delete saveButton;
    delete inputAddress;
    delete inputName;
    delete layout;
}


//--

SourceTreeParentItemDialog::SourceTreeParentItemDialog(Source * src, Language * lang, QWidget * parent) : QDialog(parent){
    this->language = lang;
    
    setMinimumWidth(500);

    name = new QLineEdit(src->getName());
    address = new QLineEdit(src->getAddress());
    
    okButton = new QPushButton(this->language->getSettingsSourcesParentDialogSubmit());
    connect(okButton, &QPushButton::clicked, [=](){
        this->acceptStatus= true;
        this->accept();
    });
    
    cancelButton = new QPushButton(this->language->getSettingsSourcesParentDialogCancel());
    connect(cancelButton, &QPushButton::clicked, [=](){
        this->acceptStatus= false;
        this->reject();
    });

    name->setPlaceholderText(this->language->getSettingsSourcesParentDialogNamePlaceholder());
    address->setPlaceholderText(this->language->getSettingsSourcesParentDialogAddressPlaceholder());

    layout = new QVBoxLayout(this);
    layout->addWidget(name);
    layout->addWidget(address);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);
    setLayout(this->layout);
}

const bool & SourceTreeParentItemDialog::isAccept(){
    return this->acceptStatus;
}

QString SourceTreeParentItemDialog::getName(){
    return this->name->text();
}

QString SourceTreeParentItemDialog::getAddress(){
    return this->address->text();
}

SourceTreeParentItemDialog::~SourceTreeParentItemDialog(){
    delete name;
    delete address;
    delete okButton;
    delete cancelButton;
    delete layout;
}

SourceTreeParentItem::SourceTreeParentItem (const QString & text, Language * lang, QTreeWidgetItem * parent) : QTreeWidgetItem(parent){
    this->language = lang;
    this->setText(0,text);
    Source src = ConfigSingleton::getInstance().GetConfig().getSources()[text];
    if(src.isEnabled()){
        this->setIcon(0,QIcon(":/icons/circle-green.png"));
    }else{
        this->setIcon(0,QIcon(":/icons/circle-red.png"));
    }
}

SourceTreeParentItem::SourceTreeParentItem (const QString & text, Language * lang, QTreeWidget * parent) : QTreeWidgetItem(parent){
    this->language = lang;
    this->setText(0,text);
    Source src = ConfigSingleton::getInstance().GetConfig().getSources()[text];
    if(src.isEnabled()){
        this->setIcon(0,QIcon(":/icons/circle-green.png"));
    }else{
        this->setIcon(0,QIcon(":/icons/circle-red.png"));
    }
}

void SourceTreeParentItem::showContextMenu(const QPoint & point){
    // load data
    auto & sources = ConfigSingleton::getInstance().GetConfig().getSources();
    Source & current = sources[this->text(0)];
    // context menu
    QMenu menu;
    menu.setStyleSheet(
        "QMenu{"
        "background-color: rgb(4, 0, 24);"
        "color: rgb(210, 210, 210);"
        "border: 1px solid black;"
        "}"
        "QMenu::item{"
        "background-color: transparent;"
        "color: rgb(210, 210, 210);"
        "}"
        "QMenu::item:selected{"
        "background-color: rgb(48, 40, 88);"
        "color: rgb(210, 210, 210);"
        "}"
    );
    // action list
    QAction *editNameAction = menu.addAction(this->language->getSettingsSourcesTreeParentActionEdit());
    QAction *addCategoryAction = menu.addAction(this->language->getSettingsSourcesTreeParentActionAddCategory());
    
    QAction *switchAction = nullptr;
    if(current.isEnabled()){switchAction = menu.addAction(this->language->getSettingsSourcesTreeParentActionSwitchOff());}
    else{switchAction = menu.addAction(this->language->getSettingsSourcesTreeParentActionSwitchOn());}

    QAction *deleteAction = menu.addAction(this->language->getSettingsSourcesTreeParentActionDelete());
    // selected:
    QAction *selectedAction = menu.exec(point);
    // lets go
    if(selectedAction == editNameAction){
        bool ok;
        SourceTreeParentItemDialog * dialog = new SourceTreeParentItemDialog(&current,this->language, this->treeWidget());
        dialog->exec();

        if (dialog->isAccept() && !dialog->getName().isEmpty() && !dialog->getAddress().isEmpty()){ // text is not null and empty
            auto & sources = ConfigSingleton::getInstance().GetConfig().getSources();
            // edit sources
            if(current.getName() == dialog->getName()){ // if old name and new name is equal so change only address.
                current.setAddress(dialog->getAddress());
            }else{ // if names are different so then...
            
                if(sources.contains(dialog->getName())){return;}
                // edit news
                auto & news = ConfigSingleton::getInstance().GetConfig().getNews();
                for(QString key : news.keys()){
                    if(news[key].getSourceName() == current.getName()){
                        news[key].setSourceName(dialog->getName());
                    }
                }
                // edit categories
                auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
                for(QString key : categories.keys()){
                    int index = categories[key].getSources().indexOf(current.getName());
                    if(index>-1){
                        QStringList cList = categories[key].getSources();
                        cList.removeAt(index);
                        if(!cList.contains(dialog->getName())){
                            cList.append(dialog->getName());
                        }
                        categories[key].setSources(cList);
                    }
                }
                sources.insert(dialog->getName(), Source(dialog->getName(),dialog->getAddress(),current.getCategories(), current.isEnabled()));
                sources.remove(current.getName());
            }
                // save
            ConfigSingleton::getInstance().Save();
            // update ui
            this->setText(0,dialog->getName());
        }
    }
    else if (selectedAction == addCategoryAction) {
        QString sourceName = this->text(0);
        auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
        // get not selected categories
        QStringList keys;
        for(QString key:categories.keys()){
            if(!sources[sourceName].getCategories().contains(key)){
                keys.append(key);
            }
        }

        bool ok;
        QString text = QInputDialog::getItem(this->treeWidget(), this->language->getSettingsSourcesTreeParentDialogAddCategoryTitle(),
                                            this->language->getSettingsSourcesTreeParentDialogAddCategoryField(), keys,
                                            0, false, &ok);
        if(ok){
            // add to category
            QStringList tmpList = categories[text].getSources();
            if(!tmpList.contains(sourceName)){
                tmpList.append(sourceName);
            }
            categories[text].setSources(tmpList);
            // add to source
            tmpList = sources[sourceName].getCategories();
            if(!tmpList.contains(text)){
                tmpList.append(text);
            }
            sources[sourceName].setCategories(tmpList);
            // add to news
            auto & news = ConfigSingleton::getInstance().GetConfig().getNews();
            for(QString key : news.keys()){
                if(news[key].getSourceName() == sourceName){
                    tmpList = news[key].getCategories();
                    if(!tmpList.contains(text)){
                        tmpList.append(text);
                        news[key].setCategories(tmpList);
                    }
                }
            }
            // save
            ConfigSingleton::getInstance().Save();
            // add to UI
            SourceTreeChildItem* categoryItem = new SourceTreeChildItem(text, this->language, this);
        }
    }
    else if (selectedAction == switchAction) {
        current.setEnableStatus(!current.isEnabled());
        ConfigSingleton::getInstance().Save();
        if(current.isEnabled()){
            this->setIcon(0,QIcon(":/icons/circle-green.png"));
        }else{
            this->setIcon(0,QIcon(":/icons/circle-red.png"));
        }
    }
    else if (selectedAction == deleteAction) {
        /*
            Remove source from categories
        */
        QStringList cList = current.getCategories();
        auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
        for(QString key : cList){
            QStringList sList = categories[key].getSources();
            int index = sList.indexOf(current.getName());
            if(index > -1){
                sList.remove(index);
                categories[key].setSources(sList);
            }
        }

        /*
            Remove source from config
        */
        sources.remove(this->text(0));
        ConfigSingleton::getInstance().Save();
        // remove from UI
        if(this->treeWidget()){
            this->~SourceTreeParentItem();
        }
    }
}


SourceTreeChildItem::SourceTreeChildItem(const QString & text, Language * lang, QTreeWidgetItem * parent) : QTreeWidgetItem(parent){
    this->language = lang;
    this->setText(0,text);
}

void SourceTreeChildItem::showContextMenu(const QPoint & point){
    // context menu and style
    QMenu menu;
    menu.setStyleSheet(
        "QMenu{"
        "background-color: rgb(4, 0, 24);"
        "color: rgb(210, 210, 210);"
        "border: 1px solid black;"
        "}"
        "QMenu::item{"
        "background-color: transparent;"
        "color: rgb(210, 210, 210);"
        "}"
        "QMenu::item:selected{"
        "background-color: rgb(48, 40, 88);"
        "color: rgb(210, 210, 210);"
        "}"
    );

    QAction *deleteAction = menu.addAction(this->language->getSettingsSourcesTreeChildActionDelete());
    // selected:
    QAction *selectedAction = menu.exec(point);
    // lets go
    if (selectedAction == deleteAction) {
        QString categoryName = this->text(0);
        SourceTreeParentItem *parentItem = static_cast<SourceTreeParentItem*>(this->parent());
        QString sourceName = parentItem->text(0);
       /*
            Remove source from category
       */
        auto & categories = ConfigSingleton::getInstance().GetConfig().getCategories();
        QStringList sList = categories[categoryName].getSources();
        int index = sList.indexOf(sourceName);
        if(index>-1){
            sList.removeAt(index);
            categories[categoryName].setSources(sList);
        }

       /*
            Remove category from sources
       */
        auto & sources = ConfigSingleton::getInstance().GetConfig().getSources();
        QStringList cList = sources[sourceName].getCategories();
        index = cList.indexOf(categoryName);
        if(index>-1){
            cList.removeAt(index);
            sources[sourceName].setCategories(cList);
        }

        /*
            Remove from News
        */
        auto & news = ConfigSingleton::getInstance().GetConfig().getNews();
        for(QString key : news.keys()){
            if(news[key].getSourceName() == sourceName){
                QStringList cmList = news[key].getCategories();
                index = cmList.indexOf(categoryName);
                if(index>-1){
                    cmList.removeAt(index);
                    news[key].setCategories(cmList);
                }
            }
        }
        // save
        ConfigSingleton::getInstance().Save();
        // remove from UI
        parentItem->removeChild(this);
    }
}


SettingsWidgetSourceTree::SettingsWidgetSourceTree(Language * lang, QTreeWidget * parent): QTreeWidget(parent){
    this->language = lang;
    this->setStyleSheet(
        "color: white"
    );
    this->setHeaderHidden(true);
    this->setMinimumHeight(300);
    this->loadTree();
}

void SettingsWidgetSourceTree::updateLanguage(Language * lang){
    this->language = lang;
    this->sourceHasBeenChanged(); // reload tree
}

void SettingsWidgetSourceTree::contextMenuEvent(QContextMenuEvent * event){
    QTreeWidgetItem *item = itemAt(event->pos());
    if (item) {
        SourceTreeParentItem * parentItem = nullptr;
        SourceTreeChildItem * childItem = nullptr;

        parentItem = dynamic_cast<SourceTreeParentItem*>(item);
        if(parentItem){
            parentItem->showContextMenu(event->globalPos());
            return;
        }

        childItem = dynamic_cast<SourceTreeChildItem*>(item);
        if(childItem){
            childItem->showContextMenu(event->globalPos());
            return;
        }
    }
}

void SettingsWidgetSourceTree::clearTree(){
    QList<QTreeWidgetItem *> elements = this->findItems("", Qt::MatchFlag::MatchContains,0);
    for (int i = 0; i < elements.count(); ++i) {
        auto childItem = elements.at(i);
        delete childItem;
    }
    this->clear();
}


void SettingsWidgetSourceTree::loadTree(){
    auto sources = ConfigSingleton::getInstance().GetConfig().getSources();
    for(QString key : sources.keys()){
        SourceTreeParentItem* sourceItem = new SourceTreeParentItem(key, this->language, this);

        QList<QString> categories = sources[key].getCategories();

        for (const QString& category : categories) {
            SourceTreeChildItem* categoryItem = new SourceTreeChildItem(category, this->language, sourceItem);
        }
    }
}

void SettingsWidgetSourceTree::sourceHasBeenChanged(){
    this->clearTree();
    this->loadTree();
}

SettingsWidgetSourceTree::~SettingsWidgetSourceTree(){
    this->clearTree();
}