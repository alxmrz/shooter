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
    
    // Инициализация UI элементов смерти (скрыты по умолчанию)
    gameOverText = fabric->createText("You are dead", 0.f, 0.f);
    restartButton = fabric->createButton("restart", "Restart", 0.f, 0.f, 200, 50);
    gameOverContainer = new sf::RectangleShape(sf::Vector2f(400, 200));
    gameOverContainer->setFillColor(sf::Color::White);
    gameOverContainer->setOutlineThickness(2);
    gameOverContainer->setOutlineColor(sf::Color::Black);
    gameOver = false;
    aim = NULL;
    dialogue = NULL;
    
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
    dialogue->advance();
}

void GameObjects::startIntroDialogue()
{
    dialogue = fabric->createDialogue("intro");

    if (dialogue->hasFragments()) {
        dialogue->open();
        return;
    }

    delete dialogue;
    dialogue = NULL;
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
        
        // Если игра окончена, отображаем контейнер, текст и кнопки поверх всего
        if (gameOver) {
            window->draw(*gameOverContainer);
            window->draw(*gameOverText->message);
        }
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

    cristalCountText->message->setString(std::to_string(player->getCrystals()));
    cristalCountText->message->setPosition(crystalCountCoords.x, crystalCountCoords.y);
    crystalSprite->setPosition(cristalWindowCoords.x, cristalWindowCoords.y);
    
    ammoCountText->message->setString(std::to_string(player->getAmmo()));
    ammoCountText->message->setPosition(ammoCountCoords.x, ammoCountCoords.y);
    ammoSprite->setPosition(ammoWindowCoords.x, ammoWindowCoords.y);


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
        
    
}

void GameObjects::showGameOverScreen() {
    gameOver = true;
    
    // Останавливаем фоновую музыку
    gs->stopBackgroundMusic();
    
    // Получаем центр камеры (view)
    sf::Vector2f center = gs->view->getCenter();
    
    // Позиционируем контейнер
    gameOverContainer->setSize(sf::Vector2f(400, 200));
    gameOverContainer->setOrigin(200, 100); // центр контейнера
    gameOverContainer->setPosition(center.x, center.y);
    gameOverContainer->setFillColor(sf::Color::White);
    gameOverContainer->setOutlineThickness(2);
    gameOverContainer->setOutlineColor(sf::Color::Black);
    
    // Позиционируем текст "Вы проиграли" по центру экрана (внутри контейнера)
    gameOverText->message->setCharacterSize(48);
    gameOverText->message->setFillColor(sf::Color::Red);
    // Центрируем текст по горизонтали
    sf::FloatRect textBounds = gameOverText->message->getLocalBounds();
    gameOverText->message->setOrigin(textBounds.width / 2, textBounds.height / 2);
    gameOverText->message->setPosition(center.x, center.y - 40);
    
    // Позиционируем кнопку "Начать заново" ниже текста
    restartButton->setCoords(center.x - 100, center.y + 40);
    
    // Добавляем кнопку в вектор buttons для отрисовки
    // Проверим, не добавлена ли уже
    bool alreadyAdded = false;
    for (auto btn : buttons) {
        if (btn == restartButton) {
            alreadyAdded = true;
            break;
        }
    }
    if (!alreadyAdded) {
        buttons.push_back(restartButton);
    }
}

void GameObjects::hideGameOverScreen() {
    gameOver = false;
    
    // Удаляем кнопку из вектора buttons
    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        if (*it == restartButton) {
            buttons.erase(it);
            break;
        }
    }
}

void GameObjects::restartGame() {
    hideGameOverScreen();
    
    // Очищаем текущее состояние игры
    clear();
    
    // Запускаем новую игру через GameState
    gs->startNewGame();
}