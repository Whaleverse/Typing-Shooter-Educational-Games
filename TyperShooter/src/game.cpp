#include "game.h"
#include "typerwordgenerator.h"

Game::Game() : wordGenerator("resources/Languages/english.txt")
{
    highScore = 0;
}

/**
 * @brief Game::setupGame - setups a default state for the game. A standard/default game starts with 2 lives, 0 score, 0 highscore, and no ammo.
 * A list of 4 random words is generated.
 */
void Game::setupGame()
{
    lives = 2;
    currentScore = 0;
    ammo = 0;
    for(std::size_t index = 0; index < sizeof(words)/sizeof(words[0]); index++)
    {
        addWord(index);
    }
}

/**
 * @brief Game::addWord - adds a new word to the word list at the specified index
 * @param index - the index to add a new word too.
 */
void Game::addWord(std::size_t index)
{
    words[index] = wordGenerator.randomWord();
}

/**
 * @brief Game::verifyWord - this method will verify if the word is a word on the current word list or not. If the word
 * is in the list. It will call to update the score and get a new word added to the list.
 * @param word - word to check if it is in the word list
 */
void Game::verifyWord(std::string word)
{
    for(std::size_t index = 0; index < sizeof(words)/sizeof(words[0]); index++)
    {
        if(word.compare(words[index]) == 0)
        {
            updateCurrentScore(getPointsForWord(word));
            addWord(index);
            addAmmo();
            emit updateWordList(words);
            emit changeCorrectWord(static_cast<int>(index));
            return;
        }
    }
    emit wrongWord();
}

/**
 * @brief Game::addAmmo - adds 1 ammo.
 */
void Game::addAmmo()
{
    ammo += 1;
    emit changeAmmo(ammo);
}

/**
 * @brief Game::removeAmmo - removes 1 ammo
 */
void Game::removeAmmo()
{
    ammo -= 1;
    emit changeAmmo(ammo);
}

/**
 * @brief Game::checkCanFire - determines whether there is ammo to fire a bullet or not
 * @return - true if there is ammo to fire, else false.
 */
void Game::checkCanFire(QPoint target)
{
    emit canFire(ammo > 0, target);
}

void Game::shotBullet()
{
    removeAmmo();
}

/**
 * @brief Game::addLife adds 1 to the lives count
 */
void Game::addLife()
{
    lives += 1;
    emit changeLives(lives);
}

/**
 * @brief Game::removeLife removes 1 to the lives count and signals a Game is over if the user has used all their lives and has
 * taken damage.
 */
void Game::removeLife()
{
    lives -= 1;
    if(lives < 0)
    {
        emit gameOver(currentScore);
        resetDifficulty();
    }
    else
    {
        emit changeLives(lives);
    }
}

/**
 * @brief Game::resetDifficulty - reset the difficulty to zero
 */
void Game::resetDifficulty()
{
    wordGenerator.resetDifficulty();
    emit setDifficulty("Level " + std::to_string(wordGenerator.getCurrentLevel()));
}

/**
 * @brief Game::increaseDifficulty - increase the difficulty until the max level
 */
void Game::increaseDifficulty()
{
    wordGenerator.increaseDifficulty();
    emit setDifficulty("Level " + std::to_string(wordGenerator.getCurrentLevel()));
}

/**
 * @brief Game::updateCurrentScore - add specified points to the current score and possibly updates high score if cur.
 *  Emit signal to update the score on the window.
 * @param points - points to add to current score
 */
void Game::updateCurrentScore(int points)
{
    currentScore += points;
    if (currentScore > 1000)
        increaseDifficulty();
    else if (currentScore > 3000)
        increaseDifficulty();

    if(currentScore > highScore)
    {
        updateHighScore(currentScore);
    }
    emit changeScore(currentScore);
}

/**
 * @brief Game::updateHighScore
 * @param score
 */
void Game::updateHighScore(int score)
{
    highScore = score;
    emit changeHighScore(highScore);
}


/**
 * @brief Game::getPointsForWord - scoring system for words
 * @param word - to score
 * @return the points for the word
 */
int Game::getPointsForWord(std::string word)
{
    if(word.size() < 7)
    {
        return static_cast<int>(word.size()) * 10;
    }
    else
    {
        return static_cast<int>(word.size()) * 20;
    }
}

/**
 * @brief Game::startNewGame - setups a new game with the default settings
 */
void Game::startNewGame()
{
    setupGame();
    resetDifficulty();
    emit updateWordList(words);
    emit updateCurrentScore(0);
    emit updateHighScore(highScore);
    emit changeAmmo(ammo);
    emit changeLives(lives);
}

/**
 * @brief Game::swaplanguage - swaps the main language
 * @param languagedictloc - the location to the textfile for reading in the dictionary
 */
void Game::swapLanguage(std::string languageDictPath)
{
    wordGenerator = TyperWordGenerator(languageDictPath);
}

