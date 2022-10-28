#ifndef GAME_H
#define GAME_H

#include <string>
#include <QObject>
#include <QPoint>
#include <typerwordgenerator.h>
#include <textreader.h>

/**
 * @brief The Game class This represents the game. The player will start with 2 lives (this means that the player can be
 * hit 3 times before game over)
 */
class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    int lives;
    int currentScore;
    int highScore;
    int ammo;
    std::string words[4];
    TyperWordGenerator wordGenerator;

    void setupGame();
    void removeAmmo();
    void addLife();
    void resetDifficulty();
    void increaseDifficulty();
    void updateCurrentScore(int points);
    void updateHighScore(int score);
    void addWord(std::size_t index);
    void verifyWord(std::string word);
    void swapLanguage(std::string languageDictPath);

private:
    int getPointsForWord(std::string word);

signals:
    void changeHighScore(int score);
    void changeScore(int score);
    void changeAmmo(int ammo);
    void changeLives(int lives);
    void gameOver(int endScore);
    void updateWordList(std::string wordList[]);
    void canFire(bool, QPoint);
    void wrongWord();
    void changeCorrectWord(int index);
    void setDifficulty(std::string difficulty);

public slots:
    void startNewGame();
    void checkCanFire(QPoint);
    void shotBullet();
    void removeLife();
    void addAmmo();


};

#endif // GAME_H
