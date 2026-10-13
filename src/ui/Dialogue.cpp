#include <algorithm>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Dialogue.h"

namespace {
struct PointInChoice {
    float x;
    float y;

    bool operator()(const sf::FloatRect& rect) const {
        return rect.contains(x, y);
    }
};

const float BOX_WIDTH_RATIO = 0.8f;
const float BOX_HEIGHT_RATIO = 0.3f;
const float TOP_MARGIN = 20.f;
const float PADDING = 12.f;
const float CHAR_WIDTH_RATIO = 0.5f;
const unsigned TEXT_SIZE = 14;
const unsigned PROMPT_SIZE = 12;
const char* PROMPT_TEXT = "Press space to continue";

const float CHOICE_WIDTH = 96.f;
const float CHOICE_HEIGHT = 28.f;
const float CHOICE_GAP = 12.f;
const unsigned CHOICE_SIZE = 14;
const sf::Color CHOICE_FILL(238, 240, 244);
const sf::Color CHOICE_OUTLINE(90, 95, 105);
const sf::Color CHOICE_TEXT(35, 38, 44);
}

constexpr float Dialogue::CHARS_PER_SECOND;

Dialogue::Dialogue()
: currentIndex(0), charsPerSecond(CHARS_PER_SECOND), typingElapsed(0.f),
  openState(false), font(nullptr), portraitTexture(nullptr)
{
}

Dialogue::Dialogue(std::vector<std::string> fragments, float charsPerSecond)
: fragments(fragments), currentIndex(0), charsPerSecond(charsPerSecond),
  typingElapsed(0.f), openState(false), font(nullptr), portraitTexture(nullptr)
{
}

Dialogue::~Dialogue()
{
}

void Dialogue::open()
{
    if (!hasFragments()) {
        return;
    }

    openState = true;
    currentIndex = 0;
    typingElapsed = 0.f;
}

void Dialogue::update(float dt)
{
    if (!isOpen()) {
        return;
    }

    typingElapsed += dt;
}

void Dialogue::advance()
{
    if (hasChoices()) {
        return;
    }

    advanceFragmentOrType();
}

void Dialogue::advanceFragmentOrType()
{
    if (isTyping()) {
        completeTyping();
        return;
    }

    advanceFragment();
}

void Dialogue::completeTyping()
{
    typingElapsed = (currentFragmentSize() + 1) / charsPerSecond;
}

void Dialogue::advanceFragment()
{
    if (!hasNext()) {
        close();
        return;
    }

    showNext();
}

bool Dialogue::isTyping() const
{
    return visibleCharCount() < currentFragmentSize();
}

int Dialogue::visibleCharCount() const
{
    int shown = static_cast<int>(typingElapsed * charsPerSecond);

    return std::min(shown, currentFragmentSize());
}

int Dialogue::currentFragmentSize() const
{
    return static_cast<int>(currentFragment().size());
}

const std::string& Dialogue::currentFragment() const
{
    return fragments[currentIndex];
}

bool Dialogue::hasNext() const
{
    return currentIndex + 1 < static_cast<int>(fragments.size());
}

void Dialogue::showNext()
{
    currentIndex++;
    typingElapsed = 0.f;
}

void Dialogue::close()
{
    openState = false;
}

bool Dialogue::isOpen() const
{
    return openState;
}

bool Dialogue::hasFragments() const
{
    return !fragments.empty();
}

bool Dialogue::promptShown() const
{
    return isOpen();
}

int Dialogue::getCurrentIndex() const
{
    return currentIndex;
}

std::string Dialogue::getVisibleText() const
{
    return currentFragment().substr(0, visibleCharCount());
}

void Dialogue::setChoices(std::vector<std::string> labels)
{
    choices = labels;
}

bool Dialogue::hasChoices() const
{
    return !choices.empty();
}

int Dialogue::choiceAt(float x, float y) const
{
    PointInChoice hit = {x, y};
    std::vector<sf::FloatRect>::const_iterator it =
        std::find_if(choiceRects.begin(), choiceRects.end(), hit);

    if (it == choiceRects.end()) {
        return -1;
    }

    return static_cast<int>(std::distance(choiceRects.begin(), it));
}

void Dialogue::setFont(sf::Font* font)
{
    this->font = font;
    textMessage.setFont(*font);
    promptMessage.setFont(*font);
}

void Dialogue::setPortraitTexture(sf::Texture* texture)
{
    portraitTexture = texture;
    portrait.setTexture(*texture);
}

void Dialogue::draw(sf::RenderTarget& target, float screenWidth, float screenHeight)
{
    if (!isOpen()) {
        return;
    }

    layoutBox(screenWidth, screenHeight);
    drawContent(target);
}

void Dialogue::drawContent(sf::RenderTarget& target)
{
    target.draw(box);
    target.draw(portrait);
    target.draw(textMessage);
    drawFooter(target);
}

void Dialogue::drawFooter(sf::RenderTarget& target)
{
    if (hasChoices()) {
        drawChoices(target);
        return;
    }

    target.draw(promptMessage);
}

void Dialogue::layoutBox(float screenWidth, float screenHeight)
{
    float boxW = screenWidth * BOX_WIDTH_RATIO;
    float boxH = screenHeight * BOX_HEIGHT_RATIO;
    float boxX = (screenWidth - boxW) / 2.f;
    float boxY = TOP_MARGIN;

    box.setSize(sf::Vector2f(boxW, boxH));
    box.setPosition(boxX, boxY);
    box.setFillColor(sf::Color(200, 208, 220, 255));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::Black);

    float side = boxH - 2.f * PADDING;
    positionPortrait(boxX + PADDING, boxY + PADDING, side);
    positionText(boxX + boxH, boxY + PADDING, boxW - boxH - PADDING);
    layoutFooter(boxX, boxY, boxW, boxH);
}

void Dialogue::layoutFooter(float boxX, float boxY, float boxW, float boxH)
{
    if (hasChoices()) {
        layoutChoices(boxX, boxY, boxW, boxH);
        return;
    }

    positionPrompt(boxX, boxY, boxW, boxH);
}

void Dialogue::layoutChoices(float boxX, float boxY, float boxW, float boxH)
{
    choiceRects.clear();

    const float count = static_cast<float>(choices.size());
    const float total = count * CHOICE_WIDTH + (count - 1.f) * CHOICE_GAP;
    const float y = boxY + boxH - PADDING - CHOICE_HEIGHT;
    const float startX = boxX + boxW - PADDING - total;

    for (size_t i = 0; i < choices.size(); i++) {
        float x = startX + static_cast<float>(i) * (CHOICE_WIDTH + CHOICE_GAP);
        choiceRects.push_back(sf::FloatRect(x, y, CHOICE_WIDTH, CHOICE_HEIGHT));
    }
}

void Dialogue::drawChoices(sf::RenderTarget& target)
{
    for (size_t i = 0; i < choiceRects.size(); i++) {
        sf::RectangleShape shape = choiceBoxShape(choiceRects[i]);
        sf::Text label = choiceLabel(i, choiceRects[i]);
        target.draw(shape);
        target.draw(label);
    }
}

sf::RectangleShape Dialogue::choiceBoxShape(const sf::FloatRect& rect) const
{
    sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
    shape.setPosition(rect.left, rect.top);
    shape.setFillColor(CHOICE_FILL);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(CHOICE_OUTLINE);
    return shape;
}

sf::Text Dialogue::choiceLabel(size_t index, const sf::FloatRect& rect) const
{
    sf::Text label;
    label.setFont(*font);
    label.setCharacterSize(CHOICE_SIZE);
    label.setFillColor(CHOICE_TEXT);
    label.setString(sf::String::fromUtf8(choices[index].begin(), choices[index].end()));

    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    label.setPosition(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
    return label;
}

void Dialogue::positionPortrait(float x, float y, float side)
{
    if (portraitTexture == nullptr) {
        return;
    }

    sf::Vector2u size = portraitTexture->getSize();
    float scale = std::min(side / size.x, side / size.y);
    portrait.setScale(scale, scale);

    sf::FloatRect bounds = portrait.getGlobalBounds();
    portrait.setPosition(x + (side - bounds.width) / 2.f, y + (side - bounds.height) / 2.f);
}

void Dialogue::positionText(float x, float y, float width)
{
    textMessage.setCharacterSize(TEXT_SIZE);
    textMessage.setFillColor(sf::Color::Black);
    std::string wrapped = wrapText(getVisibleText(), maxCharsForWidth(width));
    textMessage.setString(sf::String::fromUtf8(wrapped.begin(), wrapped.end()));
    textMessage.setPosition(x, y);
}

void Dialogue::positionPrompt(float boxX, float boxY, float boxW, float boxH)
{
    promptMessage.setCharacterSize(PROMPT_SIZE);
    promptMessage.setFillColor(sf::Color(90, 90, 90));
    promptMessage.setString(sf::String::fromUtf8(PROMPT_TEXT, PROMPT_TEXT + std::char_traits<char>::length(PROMPT_TEXT)));

    sf::FloatRect bounds = promptMessage.getLocalBounds();
    promptMessage.setPosition(boxX + boxW - bounds.width - PADDING, boxY + boxH - bounds.height - PADDING);
}

unsigned Dialogue::maxCharsForWidth(float width) const
{
    return static_cast<unsigned>(width / (TEXT_SIZE * CHAR_WIDTH_RATIO));
}

std::string Dialogue::wrapText(const std::string& text, unsigned maxChars) const
{
    std::istringstream stream(text);
    std::string result;
    std::string line;
    std::string word;

    while (stream >> word) {
        addWord(result, line, word, maxChars);
    }

    return result + line;
}

void Dialogue::addWord(std::string& result, std::string& line, const std::string& word, unsigned maxChars) const
{
    std::string candidate = joinWord(line, word);

    if (candidate.size() <= maxChars) {
        line = candidate;
        return;
    }

    result += line + "\n";
    line = word;
}

std::string Dialogue::joinWord(const std::string& line, const std::string& word) const
{
    return line.empty() ? word : line + " " + word;
}
