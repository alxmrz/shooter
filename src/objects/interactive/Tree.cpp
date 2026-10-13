#include "Tree.h"
#include "Platform.h"
#include <cstdlib>
#include <algorithm>

constexpr int Tree::TRUNK_WIDTH;
constexpr int Tree::BRANCH_HEIGHT;
constexpr int Tree::TILE_WIDTH;
constexpr int Tree::LEAVES_WIDTH_MULTIPLIER;
constexpr int Tree::LEAVES_X_OFFSET_MULTIPLIER;

Tree::Tree() {
}

Tree::Tree(const Tree& orig) {
}

Tree::Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength) : CObject(x, y, 0, 0) {
    const int trunkTop = y - trunkHeight;

    // Create trunk (dark brown, NO collision - will not be added to all)
    trunk = new Platform(x, trunkTop, TRUNK_WIDTH, trunkHeight,
                         sf::Color(85, 45, 25), sf::Color::Black);
    trunk->setCollidable(false);

    // Generate branches (light brown, WITH collision - will be added to all)
    generateBranches(x, trunkTop, trunkHeight, numBranches, minBranchLength, maxBranchLength);

    // Create leaves as a single rectangle (light green, NO collision - will not be added to all)
    const int leavesWidth = LEAVES_WIDTH_MULTIPLIER * TRUNK_WIDTH;
    const int leavesHeight = trunkHeight / 2;
    const int leavesX = x - LEAVES_X_OFFSET_MULTIPLIER * TRUNK_WIDTH;
    const int leavesY = trunkTop - leavesHeight;
    leaves = new Platform(leavesX, leavesY, leavesWidth, leavesHeight,
                          sf::Color(144, 238, 144), sf::Color::Black);
    leaves->setCollidable(false);

    updateBounds();
}

Tree::Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength,
           int branchStep) : Tree(x, y, trunkHeight, numBranches, minBranchLength, maxBranchLength,
                                  branchStep, 0) {
}

Tree::Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength,
           int branchStep, int topBranchSide, int topBranchLength) : CObject(x, y, 0, 0) {
    const int trunkTop = y - trunkHeight;

    trunk = new Platform(x, trunkTop, TRUNK_WIDTH, trunkHeight,
                         sf::Color(85, 45, 25), sf::Color::Black);
    trunk->setCollidable(false);

    if (branchStep > 0) {
        generateLadderBranches(x, trunkTop, trunkHeight, branchStep, minBranchLength, maxBranchLength,
                               topBranchSide, topBranchLength);
    } else {
        generateBranches(x, trunkTop, trunkHeight, numBranches, minBranchLength, maxBranchLength);
    }

    const int leavesWidth = LEAVES_WIDTH_MULTIPLIER * TRUNK_WIDTH;
    const int leavesHeight = trunkHeight / 2;
    const int leavesX = x - LEAVES_X_OFFSET_MULTIPLIER * TRUNK_WIDTH;
    const int leavesY = trunkTop - leavesHeight;
    leaves = new Platform(leavesX, leavesY, leavesWidth, leavesHeight,
                          sf::Color(144, 238, 144), sf::Color::Black);
    leaves->setCollidable(false);

    updateBounds();
}

Tree::~Tree() {
    delete trunk;
    for (Platform* branch : branches) {
        delete branch;
    }
    delete leaves;
}

void Tree::update(Event event) {
    // Trees are static
}

void Tree::draw(Window* window, float dt) {
    drawPart(window, trunk, dt);
    drawPart(window, leaves, dt);
    // Branches are rendered by the shared loop in GameObjects::draw,
    // because they are stored in all for collision. Drawing them here would duplicate them.
}

void Tree::drawPart(Window* window, Platform* part, float dt) {
    if (part) {
        part->draw(window, dt);
    }
}

std::vector<Platform*> Tree::getBranches() const {
    return branches;
}

Platform* Tree::getTrunk() const {
    return trunk;
}

Platform* Tree::getLeaves() const {
    return leaves;
}

void Tree::updateBounds() {
    float minX = trunk->getX();
    float minY = trunk->getY();
    float maxX = trunk->getX() + trunk->getWidth();
    float maxY = trunk->getY() + trunk->getHeight();

    includeBranches(minX, minY, maxX, maxY);
    includeLeaves(minX, minY, maxX, maxY);

    x = minX;
    y = minY;
    width = static_cast<int>(maxX - minX);
    height = static_cast<int>(maxY - minY);
}

void Tree::includeBranches(float& minX, float& minY, float& maxX, float& maxY) {
    for (Platform* branch : branches) {
        includePart(branch, minX, minY, maxX, maxY);
    }
}

void Tree::includeLeaves(float& minX, float& minY, float& maxX, float& maxY) {
    if (leaves) {
        includePart(leaves, minX, minY, maxX, maxY);
    }
}

void Tree::includePart(Platform* part, float& minX, float& minY, float& maxX, float& maxY) {
    minX = std::min(minX, part->getX());
    minY = std::min(minY, part->getY());
    maxX = std::max(maxX, part->getX() + part->getWidth());
    maxY = std::max(maxY, part->getY() + part->getHeight());
}

void Tree::generateBranches(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength) {
    // At most one branch per side: one on the left and one on the right.
    const int branchCount = std::min(numBranches, 2);

    for (int i = 0; i < branchCount; i++) {
        branches.push_back(makeBranch(x, y, trunkHeight, minBranchLength, maxBranchLength, i));
    }
}

Platform* Tree::makeBranch(int x, int y, int trunkHeight, int minBranchLength, int maxBranchLength, int side) {
    // Random height along trunk
    const int branchY = y + trunkHeight * (rand() % 80 + 10) / 100;

    // Random length in tiles
    const int tiles = minBranchLength + rand() % (maxBranchLength - minBranchLength + 1);
    const int branchWidth = tiles * TILE_WIDTH;

    // First branch extends left, second extends right; base lies inside the trunk x-range
    const int baseX = x + rand() % (TRUNK_WIDTH + 1);
    const int branchX = branchXForSide(baseX, branchWidth, side);

    return new Platform(branchX, branchY, branchWidth, BRANCH_HEIGHT,
                        sf::Color(160, 82, 45), sf::Color::Black);
}

int Tree::branchXForSide(int baseX, int branchWidth, int side) {
    return (side == 0) ? baseX - branchWidth : baseX;
}

void Tree::generateLadderBranches(int x, int trunkTop, int trunkHeight, int branchStep,
                                  int minBranchLength, int maxBranchLength, int topBranchSide,
                                  int topBranchLength) {
    // Evenly spaced "step" branches along the trunk only: the topmost branch sits
    // at the trunk top so it never pokes into the leaves, while still staying
    // within a double jump of the reserved block above. The topmost branch is on
    // topBranchSide (0 = left, 1 = right); lower branches alternate. Every branch
    // base stays inside the trunk x-range and extends outward. When a top branch
    // length is given, the topmost branch spans from the trunk to the edge of the
    // host ground block instead of using a random length.
    const int baseY = trunkTop + trunkHeight;

    for (int height = trunkHeight, index = 0; height > 0; height -= branchStep, index++) {
        branches.push_back(makeLadderBranch(x, baseY, trunkHeight, height, index,
                                            minBranchLength, maxBranchLength, topBranchSide,
                                            topBranchLength));
    }
}

Platform* Tree::makeLadderBranch(int x, int baseY, int trunkHeight, int height, int index,
                                 int minBranchLength, int maxBranchLength, int topBranchSide,
                                 int topBranchLength) {
    const int side = (topBranchSide + index) % 2;
    const int branchY = baseY - height;

    if (isStretchedTop(height, trunkHeight, topBranchLength)) {
        return makeStretchedTopBranch(x, branchY, side, topBranchLength);
    }

    return makeRandomBranch(x, branchY, minBranchLength, maxBranchLength, side);
}

bool Tree::isStretchedTop(int height, int trunkHeight, int topBranchLength) {
    return (height == trunkHeight) && (topBranchLength > 0);
}

Platform* Tree::makeStretchedTopBranch(int x, int branchY, int side, int topBranchLength) {
    const int branchX = (side == 0) ? (x + TRUNK_WIDTH) - topBranchLength : x;

    return new Platform(branchX, branchY, topBranchLength, BRANCH_HEIGHT,
                        sf::Color(160, 82, 45), sf::Color::Black);
}

Platform* Tree::makeRandomBranch(int x, int branchY, int minBranchLength, int maxBranchLength, int side) {
    const int tiles = minBranchLength + rand() % (maxBranchLength - minBranchLength + 1);
    const int branchWidth = tiles * TILE_WIDTH;
    const int baseX = x + rand() % (TRUNK_WIDTH + 1);
    const int branchX = branchXForSide(baseX, branchWidth, side);

    return new Platform(branchX, branchY, branchWidth, BRANCH_HEIGHT,
                        sf::Color(160, 82, 45), sf::Color::Black);
}
