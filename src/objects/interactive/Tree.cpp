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
    if (trunk) {
        trunk->draw(window, dt);
    }
    if (leaves) {
        leaves->draw(window, dt);
    }
    // Branches are rendered by the shared loop in GameObjects::draw,
    // because they are stored in all for collision. Drawing them here would duplicate them.
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

    auto include = [&](Platform* part) {
        minX = std::min(minX, part->getX());
        minY = std::min(minY, part->getY());
        maxX = std::max(maxX, part->getX() + part->getWidth());
        maxY = std::max(maxY, part->getY() + part->getHeight());
    };

    for (Platform* branch : branches) {
        include(branch);
    }
    if (leaves) {
        include(leaves);
    }

    x = minX;
    y = minY;
    width = static_cast<int>(maxX - minX);
    height = static_cast<int>(maxY - minY);
}

void Tree::generateBranches(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength) {
    // At most one branch per side: one on the left and one on the right.
    const int branchCount = std::min(numBranches, 2);

    for (int i = 0; i < branchCount; i++) {
        // Random height along trunk
        int branchY = y + trunkHeight * (rand() % 80 + 10) / 100;

        // Random length in tiles
        int tiles = minBranchLength + rand() % (maxBranchLength - minBranchLength + 1);
        int branchWidth = tiles * TILE_WIDTH;

        // First branch extends left, second extends right; base lies inside the trunk x-range
        int baseX = x + rand() % (TRUNK_WIDTH + 1);
        bool leftSide = (i == 0);
        int branchX = leftSide ? baseX - branchWidth : baseX;

        Platform* branch = new Platform(branchX, branchY, branchWidth, BRANCH_HEIGHT,
                                        sf::Color(160, 82, 45), sf::Color::Black);
        branches.push_back(branch);
    }
}
