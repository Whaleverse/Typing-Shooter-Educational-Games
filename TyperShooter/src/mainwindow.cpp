#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFontDatabase>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>
//#include <QRandomGenerator>
#include <QTime>
#include <QMediaPlayer>

MainWindow::MainWindow(QWidget *parent, const Game& gameModel) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    backgroundTimer(new QTimer(this)),
    backgrounds({"resources/Images/slcBaseDayMid.png",
                 "resources/Images/slcBaseEveningMid.png",
                 "resources/Images/slcBaseNightMid.png",
                 "resources/Images/slcBaseEveningMid.png"}),
    backgroundIndex(0)
{
    ui->setupUi(this);
    ui->pixmapLabel->setScaledContents(true);
    ui->pixmapLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    variableSpawn = 5000;
    texture.create(541,731);

    // Load Fonts
    QFontDatabase::addApplicationFont("resources/Fonts/Zpix.ttf");
    QFontDatabase::addApplicationFont("resources/Fonts/Minecraftia.ttf");

    // Start Rendering and Missile Timer
    missileTimer = new QTimer(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::renderTexture);

    // Set background.
    loadAndIncrementBackground();
    startDaytimeTimer();

    // Game setup
    showMenu("Start!", &MainWindow::newGameAction, "", &MainWindow::closeMenu);
    setDifficulty("Level 1");
    hideDifficulty();
    setupGameInfoAndTypeArea();

    mediaPlayer();

    //Model and MainWindow signals and slots logic
    connect(this, &MainWindow::startGameModel, &gameModel, &Game::startNewGame);
    connect(&gameModel, &Game::updateWordList, this, &MainWindow::displayWords);
    connect(this, &MainWindow::checkWord, &gameModel, &Game::verifyWord);
    connect(&gameModel, &Game::changeScore, this, &MainWindow::changeScore);
    connect(&gameModel, &Game::changeHighScore, this, &MainWindow::changeHighScore);
    connect(&gameModel, &Game::gameOver, this, &MainWindow::gameOver);
    connect(&gameModel, &Game::changeAmmo, this, &MainWindow::changeAmmo);
    connect(&gameModel, &Game::setDifficulty, this, &MainWindow::setDifficulty);
    connect(&gameModel, &Game::changeLives, this, &MainWindow::changeLives);
    connect(this, &MainWindow::loseLife, &gameModel, &Game::removeLife);
    connect(ui->pixmapLabel, &shootableLabel::Mouse_Pressed, &gameModel, &Game::checkCanFire);
    connect(&gameModel, &Game::canFire, this, &MainWindow::shootBullet);
    connect(this, &MainWindow::shotBullet, &gameModel, &Game::shotBullet);
    connect(&gameModel, &Game::wrongWord, this, &MainWindow::wrongWordIndicator);
    connect(&gameModel, &Game::changeCorrectWord, this, &MainWindow::changeCorrectWord);
    connect(this, &MainWindow::swaplanguage, &gameModel, &Game::swapLanguage);
}

// Starts a timer that switches the background image
void MainWindow::startDaytimeTimer()
{
    int minutes = 1;
    connect(backgroundTimer, &QTimer::timeout, this, &MainWindow::loadAndIncrementBackground);
    backgroundTimer->start(int(minutes * 60000));  // to milliseconds
}

// Loads the indexed background from the backgrounds vector in a circular fashion.
// Increments the index after done.
void MainWindow::loadAndIncrementBackground()
{
    // Get the current circular index
    unsigned long circIndex = unsigned(abs(backgroundIndex)) % backgrounds.size();
    QPixmap background(backgrounds.at(circIndex));
    background = background.scaled(this->size(), Qt::KeepAspectRatio);

    // Set Background
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);
    this->update();
    backgroundIndex++;
}

// Shows a Menu Screen with the given roles. If a role is blank, the button disappears.
void MainWindow::showMenu(QString okButtonRole,
                          void (MainWindow::*okAction)(),
                          QString cancelButtonRole,
                          void (MainWindow::*cancelAction)())
{
    QFont latinPixelFont = QFont("Minecraftia", 16, 1);
    QFont latinHeaderPixelFont = QFont("Minecraftia", 30, 1);

    ui->menuLabel->setFont(latinHeaderPixelFont);
    ui->gridWidget->setVisible(true);
    ui->buttonBox->setFont(latinPixelFont);
    ui->spanishButton->setFont(latinPixelFont);
    ui->englishButton->setFont(latinPixelFont);
    ui->chineseButton->setFont(latinPixelFont);
    ui->pauseButton->setVisible(false);
    QPushButton* okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton* cancelButton = ui->buttonBox->button(QDialogButtonBox::Cancel);
    okButton->setText(okButtonRole);
    cancelButton->setText(cancelButtonRole);
    okButton->setVisible(okButtonRole != "");
    cancelButton->setVisible(cancelButtonRole != "");
    this->okAction = okAction;
    this->cancelAction = cancelAction;
}

// Closes the Menu Screen if applicable
void MainWindow::closeMenu()
{
    ui->gridWidget->setVisible(false);
    ui->pauseButton->setVisible(true);
}

// Stops a Game
void MainWindow::stopGame()
{
    timer->stop();
    ui->typeWordArea->setReadOnly(true);
    stopAnimatingMissile();
    player->pause();
}

// Resume a Game
void MainWindow::resumeGame()
{
    timer->start(1);  // every 1 ms
    ui->typeWordArea->setReadOnly(false);
    resumeAnimatingMissile();
    player->play();
}

void MainWindow::stopAnimatingMissile()
{
    for(Missile* missile : missiles)
    {
        missile->stopAnimating();
    }
}

void MainWindow::resumeAnimatingMissile()
{
    for(Missile* missile : missiles)
    {
        missile->resumeAnimating();
    }
}

// Starts a Game
void MainWindow::startGame()
{
    // Create Box2D world
    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);

    // Create the ground
    groundDef.position.Set(0.0f, -600.0f);
    ground = world->CreateBody(&groundDef);
    groundBox.SetAsBox(500.0f, 10.0f);
    ground->CreateFixture(&groundBox, 0.0f);

    // Add a missile
    missiles = std::set<Missile*>();
    resumeGame();
    bullets = std::vector<Bullet*>();
    //setupDebug();

    // Set a timer to update the screen every 1 ms
    timer->start(1);
    variableSpawn = 5000;
    missileTimer->singleShot(variableSpawn, this, &MainWindow::addMissile);
    showGameInfo();

    //Set focus on line edit
    //(See if this works with shooting missles)
    QWidget* widget = ui->typeWordArea;
    widget->setFocus();

    emit startGameModel();

    ui->menuLabel->setText(QString::fromStdString("WordBlaster"));
    QFont latinHeaderPixelFont = QFont("Minecraftia", 30, 1);
    ui->menuLabel->setFont(latinHeaderPixelFont);

    ui->chineseButton->setVisible(true);
    ui->englishButton->setVisible(true);
    ui->spanishButton->setVisible(true);

    player->setMuted(false);
}

// Adds a missile to the world
void MainWindow::addMissile()
{
    qsrand(QTime::currentTime().second());
    Missile* missile = new Missile(nullptr,(rand()%530) + 10, 0, *world, 96, 96);
    missiles.insert(missile);
    connect(missile, &Missile::groundHit, this, &MainWindow::missileExplodedOnGround);
    connect(missile, &Missile::deleteMissile, this, &MainWindow::deleteMissileThatAreExploded);
    variableSpawn *= .99;
    missileTimer->singleShot(variableSpawn, this, &MainWindow::addMissile);
}

void MainWindow::deleteMissileThatAreExploded(Missile* missile)
{
    missiles.erase(missile);
    delete missile;
}

void MainWindow::renderTexture()
{
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;

    texture.clear(sf::Color::Transparent);  // Clear prev texture

    world->Step(timeStep, velocityIterations, positionIterations);

    for(Missile* missile : missiles)
    {
        missile->updatePosition();


        b2ContactEdge* contacts = missile->getContactList();
        if(contacts)
        {
            missile->explode(world, ground);
            // reapply the previous force to prevent bounceback
            contacts->other->ApplyLinearImpulse(contacts->other->GetLinearVelocity(),
                                                b2Vec2(0.0f,0.0f), true);
        }

        texture.draw(missile->getSprite());
    }

    std::vector<unsigned int> removeBullets = std::vector<unsigned int>();

    for(unsigned int i = 0; i < bullets.size(); i++){
        bullets[i]->updatePosition();
        float xPos = bullets[i]->getPosition().x;
        float yPos = bullets[i]->getPosition().y;
        bool xOutOfBounds = (xPos < 0 || xPos > 541);
        bool yOutOfBounds = (yPos > 0 || yPos < -580);
        if(xOutOfBounds || yOutOfBounds)
        {
            world->DestroyBody(bullets[i]->getBody());
            removeBullets.push_back(i);  // mark for deletion
        }
        else {
            texture.draw(bullets[i]->getSprite());
        }
    }
    // delete those marked for deletion
    for(unsigned int i : removeBullets){
        bullets.erase(bullets.begin() + i);
    }
    texture.display();
    ui->pixmapLabel->setPixmap(toPixmap());
}

void MainWindow::missileExplodedOnGround()
{
    emit loseLife();
}


QPixmap MainWindow::toPixmap()
{
    sf::Texture currentTexture = texture.getTexture();
    sf::Image currentImg = currentTexture.copyToImage();
    const uint8_t *pixelData = currentImg.getPixelsPtr();

    QImage qi(pixelData, 541, 731, QImage::Format_ARGB32);
    qi = qi.rgbSwapped();
    return QPixmap::fromImage(qi);
}

void MainWindow::shootBullet(bool canShoot, QPoint target)
{
    if(target.ry() > 280)
    {
        return;
    }
    if(canShoot){

        int y = target.ry();
        if(target.ry() >=215)
        {
            y = std::abs(y - 436);
        }
        else
        {
            y = std::abs(y -466);
        }
        Bullet* bullet = new Bullet(275,
                                    -542,
                                    *world,
                                    target.rx() - 282,
                                    y);
        bullets.push_back(bullet);
        emit shotBullet();
    }
}

// Creates a new game from the menu
void MainWindow::newGameAction()
{
    closeMenu();
    showDifficulty();
    startGame();
}

// Closes the menu and resumes the game
void MainWindow::cancelMenuAction()
{
    closeMenu();
    showDifficulty();
    if (world) resumeGame();
}

void MainWindow::showDifficulty()
{
    ui->levelLabel->setVisible(true);
    ui->levelLabel->setDisabled(false);
}

void MainWindow::hideDifficulty()
{
    ui->levelLabel->setVisible(false);
    ui->levelLabel->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pauseButton_clicked()
{
    if (world)
        stopGame();

    showMenu("Resume", &MainWindow::cancelMenuAction, "Restart!", &MainWindow::newGameAction);
    hideDifficulty();
}

void MainWindow::on_buttonBox_accepted()
{
    (this->*(okAction))();
}


void MainWindow::on_buttonBox_rejected()
{
    (this->*(cancelAction))();
}


////////////////////////////////////////////////////////
/////////Code Connecting MainWindow with Model//////////
////////////////////////////////////////////////////////

void MainWindow::setDifficulty(std::string difficulty)
{
    ui->levelLabel->setText(QString::fromStdString(difficulty));
    QFont latinHeaderPixelFont = QFont("Minecraftia", 18, 1);
    ui->levelLabel->setFont(latinHeaderPixelFont);
}

void MainWindow::changeScore(int score)
{
    std::string scoreString;
    scoreString = std::to_string(score);
    scoreString.insert(0, 8-scoreString.size(), '0');
    scoreString = "Current Score: " + scoreString;
    ui->currentScore->setText(QString::fromStdString(scoreString));
}

void MainWindow::changeHighScore(int score)
{
    std::string scoreString;
    scoreString = std::to_string(score);
    scoreString.insert(0, 8-scoreString.size(), '0');
    scoreString = "High Score: " + scoreString;
    ui->highScore->setText(QString::fromStdString(scoreString));
}


void MainWindow::displayWords(std::string wordList[])
{
    ui->typeWordArea->setText(QString());
    ui->word0->setText(QString::fromStdString(wordList[0]));
    ui->word1->setText(QString::fromStdString(wordList[1]));
    ui->word2->setText(QString::fromStdString(wordList[2]));
    ui->word3->setText(QString::fromStdString(wordList[3]));
}

void MainWindow::keyPressEvent( QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
    {
        emit checkWord(ui->typeWordArea->text().toUtf8().constData());
    }
}

void MainWindow::gameOver(int endScore)
{
    stopGame();

    showMenu("Play Again!", &MainWindow::newGameAction, "", &MainWindow::closeMenu);
    hideDifficulty();
    std::string scoreString = std::to_string(endScore);
    scoreString.insert(0, 8-scoreString.size(), '0');
    scoreString = "Congratulations \n" + scoreString;
    ui->menuLabel->setText(QString::fromStdString(scoreString));

    ui->chineseButton->setVisible(false);
    ui->englishButton->setVisible(false);
    ui->spanishButton->setVisible(false);

    QFont latinHeaderPixelFont = QFont("Minecraftia", 18, 1);
    ui->menuLabel->setFont(latinHeaderPixelFont);
}


void MainWindow::setupGameInfoAndTypeArea()
{
    hideGameInfoBeforeStart();
    ui->typeWordArea->setVisible(false);
    QFont latinPixelFont = QFont("Minecraftia", 12, 1);
    ui->word0->setFont(latinPixelFont);
    ui->word1->setFont(latinPixelFont);
    ui->word2->setFont(latinPixelFont);
    ui->word3->setFont(latinPixelFont);
    ui->currentScore->setFont(latinPixelFont);
    ui->highScore->setFont(latinPixelFont);
    ui->typeWordArea->setFont(latinPixelFont);
    latinPixelFont = QFont("Minecraftia", 18, 1);
    ui->numLives->setFont(latinPixelFont);
    ui->numAmmo->setFont(latinPixelFont);

    setupLivesArea();
    setupAmmoArea();
}


void MainWindow::setupLivesArea()
{
    QPixmap* lifeImg = new QPixmap("resources/Images/slcBaseDayTall.png");
    QPixmap scaledImg = lifeImg->scaled(40,40);
    ui->livesImg->setPixmap(scaledImg);
    QPixmap* xLifeImg = new QPixmap("resources/Images/ui.png");
    QRect croppedArea(111,23,9,9);
    QPixmap croppedImg = xLifeImg->copy(croppedArea);
    croppedImg = croppedImg.scaled(20,20);
    ui->xLivesImg->setPixmap(croppedImg);
}


void MainWindow::setupAmmoArea()
{
    QPixmap* ammoImg = new QPixmap("resources/Images/bullet.png");
    QPixmap scaledImg = ammoImg->scaled(40,40);
    ui->ammoImg->setPixmap(scaledImg);
    QPixmap* xAmmoImg = new QPixmap("resources/Images/ui.png");
    QRect croppedArea(111,23,9,9);
    QPixmap croppedImg = xAmmoImg->copy(croppedArea);
    croppedImg = croppedImg.scaled(20,20);
    ui->xAmmoImg->setPixmap(croppedImg);
}

void MainWindow::changeAmmo(int ammo)
{
    ui->numAmmo->setText(QString::fromStdString(std::to_string(ammo)));
}

void MainWindow::changeLives(int lives)
{
    ui->numLives->setText(QString::fromStdString(std::to_string(lives)));
}

void MainWindow::hideGameInfoBeforeStart()
{
    ui->typeWordArea->setVisible(false);
    ui->livesImg->setVisible(false);
    ui->xLivesImg->setVisible(false);
    ui->numLives->setVisible(false);
    ui->ammoImg->setVisible(false);
    ui->xAmmoImg->setVisible(false);
    ui->numAmmo->setVisible(false);
}

void MainWindow::showGameInfo()
{
    ui->typeWordArea->setVisible(true);
    ui->livesImg->setVisible(true);
    ui->xLivesImg->setVisible(true);
    ui->numLives->setVisible(true);
    ui->ammoImg->setVisible(true);
    ui->xAmmoImg->setVisible(true);
    ui->numAmmo->setVisible(true);
}

void MainWindow::wrongWordIndicator()
{
    QTimer* wrongWordTimer = new QTimer(this);
    wrongWordTimer->singleShot(250, this, &MainWindow::revertWordAreaToDefault);
    ui->typeWordArea->setStyleSheet("QWidget {background-color: red;}");
}

void MainWindow::revertWordAreaToDefault()
{
    ui->typeWordArea->setStyleSheet("QWidget {background-color: white;}");
}

void MainWindow::changeCorrectWord(int index)
{

    QTimer* correctWordTimer = new QTimer(this);
    wordIndex = index;
    correctWordTimer->singleShot(250, this, &MainWindow::revertWordLabelToDefault);

    if(index == 0)
    {
         ui->word0->setStyleSheet("QWidget {background-color: #17c44b;}");
    }
    else if(index == 1)
    {
        ui->word1->setStyleSheet("QWidget {background-color: #17c44b;}");
    }
    else if(index == 2)
    {
        ui->word2->setStyleSheet("QWidget {background-color: #17c44b;}");
    }
    else
    {
        ui->word3->setStyleSheet("QWidget {background-color: #17c44b;}");
    }

}

void MainWindow::revertWordLabelToDefault()
{
    if(wordIndex == 0)
    {
         ui->word0->setStyleSheet("QWidget {background-color: white;}");
    }
    else if(wordIndex == 1)
    {
        ui->word1->setStyleSheet("QWidget {background-color: white;}");
    }
    else if(wordIndex == 2)
    {
        ui->word2->setStyleSheet("QWidget {background-color: white;}");
    }
    else
    {
        ui->word3->setStyleSheet("QWidget {background-color: white;}");
    }
}

void MainWindow::on_englishButton_clicked()
{
    emit swaplanguage("resources/Languages/english.txt");
}

void MainWindow::on_spanishButton_clicked()
{
    emit swaplanguage("resources/Languages/spanish.txt");
}

void MainWindow::on_chineseButton_clicked()
{
   emit swaplanguage("resources/Languages/chinese.txt");
   QFont chineseHeaderPixelFont = QFont("Zpix", 30, 1);
   QFont chinesePixelFont = QFont("Zpix", 30, 1);

   ui->menuLabel->setFont(chineseHeaderPixelFont);
   ui->gridWidget->setVisible(true);
   ui->buttonBox->setFont(chinesePixelFont);
   ui->spanishButton->setFont(chinesePixelFont);
   ui->englishButton->setFont(chinesePixelFont);
   ui->chineseButton->setFont(chinesePixelFont);
}

void MainWindow::mediaPlayer()
{
    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/music/background.mp3"));
    player->setVolume(120);
    player->play();
    player->setMuted(true);
}
