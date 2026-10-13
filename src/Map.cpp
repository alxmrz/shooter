#include <string>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../include/tinyxml/tinyxml2.h"
#include "Map.h"
#include "GameObjects.h"
#include "objects/backgrounds/Ground.h"
#include "objects/interactive/Crystal.h"
#include "objects/interactive/Heart.h"
#include "objects/interactive/Ammunition.h"
#include "objects/interactive/Platform.h"
#include "objects/backgrounds/GroundBlock.h"
#include "objects/interactive/Tree.h"
#include "objects/interactive/Aim.h"
#include "objects/units/Shooter.h"
#include "Fabric.h"
#include "LevelGenerator.h"

namespace {
/**
 * TODO: need to remove strict values from level template files
 * Current values are stored in files: training.tmx and *.tsx
 */
const int TILEMAP_WIDTH = 1972;
const int TILEMAP_HEIGHT = 1184;
const int TILEMAP_COLUMNS = 78;
const int TILEMAP_MARGIN = 0;
const int TILEMAP_TILE_WIDTH = 25;
const int TILEMAP_TILE_HEIGHT = 25;

const int SPAWN_SIZE = 50;
const int AIM_SIZE = 10;
const int TREE_PLATFORM_WIDTH = 100;
const int TREE_PLATFORM_HEIGHT = 25;
}

Map::Map() 
{
}

Map::Map(const Map& orig) 
{
}

Map::Map(GameObjects* go) :go(go)
{
}

Map::~Map() 
{
    
}

void Map::generateLevel(const char* level)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(level);

    tinyxml2::XMLElement* map = doc.FirstChildElement("map");
    generateTIleIdPositions();
    generateBackground(map);
    generatePlayable(map);
}

void Map::generateLevel(LevelGenerator& generator)
{
    generator.generateLevel(this);
}

void Map::generateBackground(tinyxml2::XMLElement* map)
{
    for (tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
            layer != NULL;
            layer = layer->NextSiblingElement()) {
        appendLayer(layer);
    }
}

void Map::appendLayer(tinyxml2::XMLElement* layer)
{
    if (!hasLayerData(layer)) {
        return;
    }

    appendLayerTiles(layerDataText(layer));
}

bool Map::hasLayerData(tinyxml2::XMLElement* layer)
{
    return layer->FirstChildElement("data") != NULL;
}

std::string Map::layerDataText(tinyxml2::XMLElement* layer)
{
    return layer->FirstChildElement("data")->GetText();
}

void Map::appendLayerTiles(const std::string& data)
{
    std::vector<std::string> lines = splitLayerLines(data);
    int y = 0;

    for (std::string line : lines) {
        appendTileLine(line, y);
        y += 25;
    }
}

std::vector<std::string> Map::splitLayerLines(const std::string& data)
{
    std::vector<std::string> lines;
    std::string trimmed = data;
    boost::trim(trimmed);
    boost::split(lines, trimmed, boost::algorithm::is_any_of("\n"), boost::token_compress_on);
    return lines;
}

void Map::appendTileLine(const std::string& line, int y)
{
    std::vector<std::string> lineChars;
    boost::split(lineChars, line, boost::algorithm::is_any_of(","), boost::token_compress_on);
    int x = 0;

    for (std::string cell : lineChars) {
        appendTile(cell, x, y);
        x += 25;
    }
}

void Map::appendTile(const std::string& cell, int x, int y)
{
    unsigned id = atoi(cell.c_str()) - 1;

    if (id > 0 && id < go->globalTileIds.size()) {
        go->background.push_back({x, y, go->globalTileIds[id][0], go->globalTileIds[id][1]});
    }
}

void Map::generatePlayable(tinyxml2::XMLElement* map)
{
    for (tinyxml2::XMLElement* objectgroup = map->FirstChildElement("objectgroup");
            objectgroup != NULL;
            objectgroup = objectgroup->NextSiblingElement()) {
        spawnGroupObjects(objectgroup);
    }
}

void Map::spawnGroupObjects(tinyxml2::XMLElement* objectgroup)
{
    for (tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
            object;
            object = object->NextSiblingElement()) {
        spawnObject(object);
    }
}

void Map::spawnObject(tinyxml2::XMLElement* object)
{
    if (!hasType(object)) {
        return;
    }

    runSpawner(object, object->Attribute("type"));
}

bool Map::hasType(tinyxml2::XMLElement* object)
{
    return object->Attribute("type") != 0;
}

void Map::runSpawner(tinyxml2::XMLElement* object, const std::string& type)
{
    ObjectSpawner spawner = spawnerFor(type);

    if (spawner == nullptr) {
        return;
    }

    (this->*spawner)(object);
}

Map::ObjectSpawner Map::spawnerFor(const std::string& type)
{
    const SpawnerTable& table = spawnerTable();
    SpawnerTable::const_iterator it = table.find(type);

    if (it == table.end()) {
        return nullptr;
    }

    return it->second;
}

const Map::SpawnerTable& Map::spawnerTable()
{
    static const SpawnerTable table = {
        {"Shooter", &Map::spawnShooter},
        {"Crystal", &Map::spawnCrystal},
        {"Border", &Map::spawnBorder},
        {"Heart", &Map::spawnHeart},
        {"Ammunition", &Map::spawnAmmunition},
        {"Tree", &Map::spawnTree},
        {"GroundBlock", &Map::spawnGroundBlock}
    };
    return table;
}

int Map::attrInt(tinyxml2::XMLElement* object, const char* name)
{
    return atoi(object->Attribute(name));
}

void Map::spawnShooter(tinyxml2::XMLElement* object)
{
    Shooter* shooter = go->fabric->createShooter(attrInt(object, "x"), attrInt(object, "y"),
                                                 SPAWN_SIZE, SPAWN_SIZE);
    setupPlayerIfNamed(object, shooter);
    go->all.push_back(shooter);
}

void Map::setupPlayerIfNamed(tinyxml2::XMLElement* object, Shooter* shooter)
{
    if (isPlayerNamed(object)) {
        makeMainPlayer(object, shooter);
    }
}

bool Map::isPlayerNamed(tinyxml2::XMLElement* object)
{
    return std::string(object->Attribute("name")) == "Player";
}

void Map::makeMainPlayer(tinyxml2::XMLElement* object, Shooter* shooter)
{
    shooter->setMain(true);
    go->player = shooter;
    go->aim = go->fabric->createAim(
        attrInt(object, "x"), attrInt(object, "y"), AIM_SIZE, AIM_SIZE);
    go->all.push_back(go->aim);
}

void Map::spawnCrystal(tinyxml2::XMLElement* object)
{
    go->all.push_back(
        go->fabric->createCrystal(attrInt(object, "x"), attrInt(object, "y"), SPAWN_SIZE, SPAWN_SIZE)
    );
}

void Map::spawnBorder(tinyxml2::XMLElement* object)
{
    go->borders.insert({attrInt(object, "x"),
        go->fabric->createBorder(
            attrInt(object, "x"),
            attrInt(object, "y"),
            attrInt(object, "width"),
            attrInt(object, "height")
        )}
    );
}

void Map::spawnHeart(tinyxml2::XMLElement* object)
{
    go->all.push_back(
        go->fabric->createHeart(attrInt(object, "x"), attrInt(object, "y"), SPAWN_SIZE, SPAWN_SIZE)
    );
}

void Map::spawnAmmunition(tinyxml2::XMLElement* object)
{
    go->all.push_back(
        go->fabric->createAmmo(attrInt(object, "x"), attrInt(object, "y"), SPAWN_SIZE, SPAWN_SIZE)
    );
}

void Map::spawnTree(tinyxml2::XMLElement* object)
{
    Tree* tree = createTreeFromAttributes(object);
    // Add only branches to all (for collision)
    for (Platform* branch : tree->getBranches()) {
        go->all.push_back(branch);
    }
    go->all.push_back(tree);
    appendTreePlatform(object);
}

Tree* Map::createTreeFromAttributes(tinyxml2::XMLElement* object)
{
    return go->fabric->createTree(
        attrInt(object, "x"),
        attrInt(object, "y"),
        attrInt(object, "trunkHeight"),
        attrInt(object, "numBranches"),
        attrInt(object, "minBranchLength"),
        attrInt(object, "maxBranchLength")
    );
}

void Map::appendTreePlatform(tinyxml2::XMLElement* object)
{
    go->all.push_back(
        go->fabric->createPlatform(
            attrInt(object, "x"),
            attrInt(object, "y"),
            TREE_PLATFORM_WIDTH,
            TREE_PLATFORM_HEIGHT
        )
    );
}

void Map::spawnGroundBlock(tinyxml2::XMLElement* object)
{
    go->all.push_back(
        go->fabric->createGroundBlock(
            attrInt(object, "x"),
            attrInt(object, "y"),
            attrInt(object, "width"),
            attrInt(object, "height")
        )
    );
}

void Map::generateTIleIdPositions()
{
    for (int y = TILEMAP_MARGIN; y < TILEMAP_HEIGHT - TILEMAP_MARGIN; y += TILEMAP_TILE_HEIGHT) {
        appendTileRow(y);
    }
}

void Map::appendTileRow(int y)
{
    for (int x = TILEMAP_MARGIN, column = 1;
            x < TILEMAP_WIDTH - TILEMAP_MARGIN && column <= TILEMAP_COLUMNS;
            x += TILEMAP_TILE_WIDTH, column++) {
        go->globalTileIds.push_back({x, y});
    }
}
