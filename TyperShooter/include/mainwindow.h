#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSize>
#include <QLabel>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Framework/DebugDraw.h>
#include <missile.h>
#include <bullet.h>
#include <vector>
#include <QSize>
#include <QLabel>
#include <QMouseEvent>
#include <vector>
#include <math.h>
#include <cmath>
#include <set>
#include <game.h>
#include "shootablelabel.h"
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, const Game& gameModel);
    ~MainWindow();

private slots:
    // Creates a new game from the menu
    void newGameAction();
    // Closes the menu and resumes the game
    void cancelMenuAction();
    void on_pauseButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_englishButton_clicked();
    void on_spanishButton_clicked();
    void on_chineseButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    QTimer* missileTimer;
    int variableSpawn;
    sf::RenderTexture texture;
    std::set<Missile*> missiles;
    std::vector<Bullet*> bullets;
    b2World* world;
    b2BodyDef groundDef;
    b2Body* ground;
    b2PolygonShape groundBox;
    QTimer* backgroundTimer;
    std::vector<QString> backgrounds;
    QMediaPlayer* player;
    int backgroundIndex;
    void (MainWindow::*okAction)();  // Action to take on menu's okay
    void (MainWindow::*cancelAction)();  // Action to take on menu's cancel
    void keyPressEvent(QKeyEvent *keyEvent);
    int wordIndex;
    bool playerCreated;

    // Starts a timer that switches the background image
    void startDaytimeTimer();

    // Loads the indexed background in the backgrounds vector in a circular fashion
    // Increments the index after done
    void loadAndIncrementBackground();

    // Renders the world into the window
    void renderTexture();



    // Starts a Game
    void startGame();

    // Stops a Game
    void stopGame();

    // Resume a Game
    void resumeGame();

    // Shows a Menu Screen
    void showMenu(QString okButtonRole, void (MainWindow::*okAction)(), QString cancelButtonRole, void (MainWindow::*cancelAction)());

    // Closes the Menu Screen if applicable
    void closeMenu();

    void setupDebug();

    QPixmap toPixmap();

    //sets the font of the word list, textEnter, highscore and score to be consistent with the overall app
    void setupGameInfoAndTypeArea();
    //setup the life area show the user has a visual display of the remaining lives
    void setupLivesArea();
    //setup the ammo area show the user has a visual display of the remaining ammo
    void setupAmmoArea();
    //hides the life area, ammo area, and the text enter before a game has started
    void hideGameInfoBeforeStart();
    //show the life area, ammo area, and text enter area
    void showGameInfo();

    void revertWordAreaToDefault();

    void revertWordLabelToDefault();

    void stopAnimatingMissile();

    void resumeAnimatingMissile();

    void missileExplodedOnGround();

    void deleteMissileThatAreExploded(Missile* missile);

    void mediaPlayer();

    void hideDifficulty();

    void showDifficulty();


public slots:
    //displays the starting words for the game
    void displayWords(std::string wordList[]);
    void changeHighScore(int score);
    void changeScore(int score);
    void changeLives(int lives);
    void changeAmmo(int ammo);
    void setDifficulty(std::string difficulty);

    // Adds a missile to the world
    void addMissile();
    void gameOver(int endScore);
    void wrongWordIndicator();
    void changeCorrectWord(int index);
    void shootBullet(bool canShoot, QPoint target);

signals:
    void startGameModel();
    void checkWord(std::string word);
    void shotBullet();
    void stopMissile();
    void resumeMissile();
    void loseLife();
    void swaplanguage(std::string languagedictloc);
};

#endif // MAINWINDOW_H
