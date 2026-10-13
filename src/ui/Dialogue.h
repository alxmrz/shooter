#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

/**
 * Intro dialogue overlay: reveals text letter by letter and advances one
 * fragment per Space press, closing after the last fragment.
 *
 * It is a screen-space overlay independent from the game camera, so it only
 * needs the current screen size when drawing.
 */
class Dialogue {
public:
    static constexpr float CHARS_PER_SECOND = 40.f;

    Dialogue();
    Dialogue(std::vector<std::string> fragments, float charsPerSecond = CHARS_PER_SECOND);
    virtual ~Dialogue();

    /**
     * Start the dialogue from the first fragment. Does nothing when there are
     * no fragments, so an empty dialogue stays closed.
     */
    void open();

    /**
     * Advance the typewriter using the elapsed frame time.
     */
    void update(float dt);

    /**
     * Handle a Space press: reveal the whole fragment while typing, otherwise
     * move to the next fragment or close on the last one.
     */
    void advance();

    bool isOpen() const;
    bool hasFragments() const;
    bool promptShown() const;
    int getCurrentIndex() const;
    std::string getVisibleText() const;

    void setFont(sf::Font* font);
    void setPortraitTexture(sf::Texture* texture);

    /**
     * Draw the overlay in screen coordinates (width/height in pixels).
     */
    void draw(sf::RenderTarget& target, float screenWidth, float screenHeight);

private:
    std::vector<std::string> fragments;
    int currentIndex;
    float charsPerSecond;
    float typingElapsed;
    bool openState;

    sf::Font* font;
    sf::Texture* portraitTexture;
    sf::Text textMessage;
    sf::Text promptMessage;
    sf::RectangleShape box;
    sf::Sprite portrait;

    void completeTyping();
    void advanceFragment();
    bool isTyping() const;
    int visibleCharCount() const;
    int currentFragmentSize() const;
    const std::string& currentFragment() const;
    bool hasNext() const;
    void showNext();
    void close();

    void layoutBox(float screenWidth, float screenHeight);
    void positionPortrait(float x, float y, float side);
    void positionText(float x, float y, float width);
    void positionPrompt(float boxX, float boxY, float boxW, float boxH);
    unsigned maxCharsForWidth(float width) const;
    std::string wrapText(const std::string& text, unsigned maxChars) const;
    void addWord(std::string& result, std::string& line, const std::string& word, unsigned maxChars) const;
    std::string joinWord(const std::string& line, const std::string& word) const;
};

#endif /* DIALOGUE_H */
