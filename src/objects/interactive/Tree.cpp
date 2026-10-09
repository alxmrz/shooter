#include "Tree.h"
#include "Platform.h"
#include <cstdlib>
#include <algorithm>

Tree::Tree() {
}

Tree::Tree(const Tree& orig) {
}

Tree::Tree(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength) : CObject(x, y, 0, 0) {
    const int TRUNK_WIDTH = 25;
    const int LEAF_SIZE = 60;
    const int BRANCH_HEIGHT = 25;
    
    // Create trunk (dark brown, NO collision - will not be added to all)
    trunk = new Platform(x, y - trunkHeight, TRUNK_WIDTH, trunkHeight,
                         sf::Color(85, 45, 25), sf::Color::Black);
    trunk->setCollidable(false);
    
    // Generate branches (light brown, WITH collision - will be added to all)
    generateBranches(x, y - trunkHeight, trunkHeight, numBranches, minBranchLength, maxBranchLength);
    
    // Create leaves (light green, NO collision - will not be added to all)
    leaves = new Platform(x - LEAF_SIZE/2, y - trunkHeight - LEAF_SIZE, LEAF_SIZE, LEAF_SIZE,
                          sf::Color(144, 238, 144), sf::Color::Black);
    leaves->setCollidable(false);
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
    for (Platform* branch : branches) {
        branch->draw(window, dt);
    }
    if (leaves) {
        leaves->draw(window, dt);
    }
}

std::vector<Platform*> Tree::getBranches() const {
    return branches;
}

void Tree::generateBranches(int x, int y, int trunkHeight, int numBranches, int minBranchLength, int maxBranchLength) {
    const int TILE_WIDTH = 50;
    const int BRANCH_HEIGHT = 25;
    const int TRUNK_WIDTH = 25;
    
    for (int i = 0; i < numBranches; i++) {
        // Random height along trunk
        int branchY = y + trunkHeight * (rand() % 80 + 10) / 100;
        
        // Random length in tiles
        int tiles = minBranchLength + rand() % (maxBranchLength - minBranchLength + 1);
        int branchWidth = tiles * TILE_WIDTH;
        
        // Random side: left or right
        bool leftSide = rand() % 2;
        int branchX;
        if (leftSide) {
            branchX = x - branchWidth;
        } else {
            branchX = x + TRUNK_WIDTH;
        }
        
        Platform* branch = new Platform(branchX, branchY, branchWidth, BRANCH_HEIGHT,
                                        sf::Color(160, 82, 45), sf::Color::Black);
        branches.push_back(branch);
    }
}
