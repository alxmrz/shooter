#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include "CObject.h"
#include "objects/units/Unit.h"
#include "objects/backgrounds/Ground.h"
#include "objects/interactive/Ammunition.h"
#include "ui/Button.h"
#include "ui/Text.h"
#include "ui/Dialogue.h"
#include "Fabric.h"
#include "Window.h"
#include "GameState.h"

GameObjects::GameObjects(GameState* gs)
{
    this->gs = gs;
    fabric = new Fabric(this);
    backgroundSprite = fabric->createSprite("background", 0.f, 0.f);
    
    heartSprite = fabric->createSprite("heart", 0.f, 0.f);
    heartSprite->setScale(0.5f, 0.5f);
    heartSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));

    groundSprite = fabric->createSprite("ground", 0.f, 0.f);
    groundSprite->setTextureRect(sf::IntRect(0, 100, 50, 50));
    
    crystalSprite = fabric->createSprite("crystal", 0.f, 0.f);
    crystalSprite->setScale(0.5f, 0.5f);
    crystalSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    cristalCountText = fabric->createText("", 0.f, 0.f);
    ammoCountText = fabric->createText("", 0.f, 0.f);
    
    ammoSprite = fabric->createSprite("ammo", 0.f, 0.f);
    ammoSprite->setScale(0.5f, 0.5f);
    ammoSprite->setTextureRect(sf::IntRect(0, 0, 50, 50));
    
    flagSprite = fabric->createSprite("flag", 0.f, 0.f);
    flagSprite->setScale(0.07f, 0.07f);
    flagCountText = fabric->createText("", 0.f, 0.f);
    
    // Инициализация состояния смерти (UI смерти — это диалог, создаётся по смерти)
    gameOver = false;
    aim = NULL;
    dialogue = NULL;
    flagDialogue = false;
    deathDialogue = false;
    stageRestartPending = false;
    
    grounds = new sf::RenderTexture;
}

GameObjects::~GameObjects()
{
}

void GameObjects::clear()
{
    buttons.clear();
    all.clear();
    player = NULL;
    aim = NULL;
    delete dialogue;
    dialogue = NULL;
    flagDialogue = false;
    deathDialogue = false;
    stageRestartPending = false;
}

void GameObjects::update(Event event, float dt)
{
    if (isDialogueOpen()) {
        dialogue->update(dt);
        return;
    }

    updateWorld(event);
}

bool GameObjects::isDialogueOpen()
{
    return dialogue != nullptr && dialogue->isOpen();
}

void GameObjects::updateWorld(Event event)
{
    for (unsigned i = 0; i < all.size(); i++) {
        all[i]->update(event);
    }
}

void GameObjects::advanceDialogue()
{
    if (!isDialogueOpen()) {
        return;
    }

    advanceOpenDialogue();
}

void GameObjects::advanceOpenDialogue()
{
    dialogue->advance();
    markStageRestartIfFlagClosed();
}

void GameObjects::markStageRestartIfFlagClosed()
{
    if (dialogue->isOpen() || !flagDialogue) {
        return;
    }

    flagDialogue = false;
    stageRestartPending = true;
}

bool GameObjects::consumeStageRestart()
{
    if (!stageRestartPending) {
        return false;
    }

    stageRestartPending = false;
    return true;
}

bool GameObjects::isFlagDialogue() const
{
    return flagDialogue;
}

void GameObjects::collectFlag()
{
    gs->collectFlag();
    startFlagDialogue();
}

void GameObjects::startFlagDialogue()
{
    startDialogue(fabric->createDialogue(fabric->getDialogueFragments("flag")), true);
}

void GameObjects::startIntroDialogue()
{
    std::vector<std::string> firstStart = fabric->getDialogueFragments("intro");
    std::vector<std::string> continuation = fabric->getDialogueFragments("intro_continue");
    std::vector<std::string> fragments =
        buildIntroFragments(firstStart, continuation, gs->getFlags(), gs->getStage());

    startDialogue(fabric->createDialogue(fragments), false);
}

void GameObjects::startDialogue(Dialogue* newDialogue, bool isFlag)
{
    if (!newDialogue->hasFragments()) {
        delete newDialogue;
        return;
    }

    beginDialogue(newDialogue, isFlag);
}

void GameObjects::beginDialogue(Dialogue* newDialogue, bool isFlag)
{
    dialogue = newDialogue;
    flagDialogue = isFlag;
    dialogue->open();
}

std::vector<std::string> GameObjects::buildIntroFragments(
    const std::vector<std::string>& firstStart,
    const std::vector<std::string>& continuation,
    int flags,
    int stage)
{
    if (flags > 0) {
        return continuedIntro(continuation, flags, stage);
    }

    return firstStart;
}

std::vector<std::string> GameObjects::continuedIntro(
    const std::vector<std::string>& continuation, int flags, int stage)
{
    std::vector<std::string> fragments;
    for (const std::string& line : continuation) {
        fragments.push_back(replaceProgressTokens(line, flags, stage));
    }
    return fragments;
}

std::string GameObjects::replaceProgressTokens(const std::string& line, int flags, int stage)
{
    std::string result = replaceToken(line, "{flags}", std::to_string(flags));
    return replaceToken(result, "{stage}", std::to_string(stage));
}

std::string GameObjects::replaceToken(const std::string& line, const std::string& token,
                                      const std::string& value)
{
    std::string result = line;
    size_t position = result.find(token);
    if (position == std::string::npos) {
        return result;
    }

    result.replace(position, token.size(), value);
    return result;
}

void GameObjects::draw(Window* window, float dt)
{
    sf::Vector2f backgroundCoords = window->mapPixelToCoords(sf::Vector2i(0, 0));
    CObject currentWindow (backgroundCoords.x, backgroundCoords.y, 900, 600);

    //y must be up 300px, because background images is very low
    backgroundSprite->setPosition(backgroundCoords.x, backgroundCoords.y-300);

    window->draw(*backgroundSprite);
    
    if (gs->isGameStarted) {
        if (groundsSprite == nullptr) {
            grounds->create(5000, 2500);

            for (unsigned i = 0; i < background.size(); i++) {
                groundSprite->setTextureRect(sf::IntRect(background[i][2], background[i][3], 25, 25));
                groundSprite->setPosition(background[i][0], background[i][1]);
                grounds->draw(*groundSprite);
            }

            grounds->display();
            const sf::Texture& texture = grounds->getTexture();
            
            groundsSprite = new sf::Sprite(texture);
        }
        
       window->draw(*groundsSprite);
        
        for (unsigned i = 0; i < all.size(); i++) {
            if (currentWindow.collideRect(all[i])) {
                all[i]->draw(window, dt);
            }
        }
        
        drawPlayerUi(window);
    }
    
    drawDialogue(window);
    
    // Кнопки отрисовываются поверх всего (включая экран смерти)
    for (unsigned i = 0; i < buttons.size(); i++) {
        buttons[i]->draw(window, dt);
    }
}

void GameObjects::drawDialogue(Window* window)
{
    if (!isDialogueOpen()) {
        return;
    }

    window->setView(window->getDefaultView());
    dialogue->draw(*window, window->getSize().x, window->getSize().y);
    window->setView(*gs->view);
}

void GameObjects::correctBackgroundSprite()
{
    
}

void GameObjects::drawPlayerUi(Window* window)
{
       
    sf::Vector2f cristalWindowCoords = window->mapPixelToCoords(sf::Vector2i(150, 50));
    sf::Vector2f crystalCountCoords = window->mapPixelToCoords(sf::Vector2i(180, 50));
    
    sf::Vector2f ammoWindowCoords = window->mapPixelToCoords(sf::Vector2i(220, 50));
    sf::Vector2f ammoCountCoords = window->mapPixelToCoords(sf::Vector2i(260, 50));

    sf::Vector2f flagWindowCoords = window->mapPixelToCoords(sf::Vector2i(320, 50));
    sf::Vector2f flagCountCoords = window->mapPixelToCoords(sf::Vector2i(350, 50));

    cristalCountText->message->setString(std::to_string(player->getCrystals()));
    cristalCountText->message->setPosition(crystalCountCoords.x, crystalCountCoords.y);
    crystalSprite->setPosition(cristalWindowCoords.x, cristalWindowCoords.y);
    
    ammoCountText->message->setString(std::to_string(player->getAmmo()));
    ammoCountText->message->setPosition(ammoCountCoords.x, ammoCountCoords.y);
    ammoSprite->setPosition(ammoWindowCoords.x, ammoWindowCoords.y);
    
    flagCountText->message->setString(std::to_string(gs->getFlags()));
    flagCountText->message->setPosition(flagCountCoords.x, flagCountCoords.y);
    flagSprite->setPosition(flagWindowCoords.x, flagWindowCoords.y);


    sf::Vector2f windowCoords = window->mapPixelToCoords(sf::Vector2i(50, 50));
    for (int i = 0; i < player->getHealth(); i++) {
        heartSprite->setPosition(windowCoords.x, windowCoords.y);
        window->draw(*heartSprite);
        windowCoords.x += 20;
    }
        
    
    window->draw(*crystalSprite);
    window->draw(*cristalCountText->message);
    
    window->draw(*ammoSprite);
    window->draw(*ammoCountText->message);
    
    window->draw(*flagSprite);
    window->draw(*flagCountText->message);
        
    
}

void GameObjects::showGameOverScreen() {
    gameOver = true;
    
    // Останавливаем фоновую музыку
    gs->stopBackgroundMusic();
    
    openDeathDialogue();
}

void GameObjects::openDeathDialogue() {
    Dialogue* death = fabric->createDialogue(fabric->getDialogueFragments("death"));
    death->setChoices({"Yes", "No"});
    startDialogue(death, false);
    deathDialogue = isDialogueOpen();
}

bool GameObjects::isDeathDialogueOpen() {
    return deathDialogue && isDialogueOpen();
}

int GameObjects::deathChoiceAt(int x, int y) {
    if (!isDeathDialogueOpen()) {
        return -1;
    }

    return dialogue->choiceAt(static_cast<float>(x), static_cast<float>(y));
}

void GameObjects::hideGameOverScreen() {
    gameOver = false;
    deathDialogue = false;
}

void GameObjects::restartGame() {
    // Смерть сбрасывает прогресс: флаги обнуляются, этап начинается заново
    gs->resetProgress();
    
    // Очищаем текущее состояние игры (в том числе диалог смерти)
    clear();
    gameOver = false;
    
    // Запускаем новую игру через GameState
    gs->startNewGame();
}