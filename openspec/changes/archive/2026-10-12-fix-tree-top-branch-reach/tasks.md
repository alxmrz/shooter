## 1. Red — tests first

- [x] 1.1 `TreeTest.LadderTopBranchUsesRequestedLength` - верхняя ветка лестницы получает переданную длину, а нижние сохраняют случайную. Проверить: `make test` (тест падает до реализации).
- [x] 1.2 `TreeTest.LadderTopBranchBaseStaysInsideTrunk` - основание верхней ветки с заданной длиной остаётся в пределах x-диапазона ствола, ветка выступает наружу. Проверить: `make test` (падает).
- [x] 1.3 `LevelGeneratorTest.RealGeneratorSetsTopBranchLengthToPlatformEdgeLeft` - для дерева с `topBranchSide == 0` длина верхней ветки равна `(trunkX + Tree::TRUNK_WIDTH) - block.x`. Проверить: `make test` (падает).
- [x] 1.4 `LevelGeneratorTest.RealGeneratorSetsTopBranchLengthToPlatformEdgeRight` - для дерева с `topBranchSide == 1` длина верхней ветки равна `block.x + block.width - trunkX`. Проверить: `make test` (падает).
- [x] 1.5 `LevelGeneratorTest.RealGeneratorTopBranchReachesPlatformEdge` - внешний конец верхней ветки каждого дерева совпадает с краем блока-хозяина со стороны `topBranchSide` и не выходит за него. Проверить: `make test` (падает).
- [x] 1.6 `LevelGeneratorTest.RealGeneratorReservedPlatformWithinReachFromTopBranch` - горизонтальный разрыв от внешнего конца верхней ветки до зарезервированной платформы не превышает `MAX_HORIZONTAL_REACH`. Проверить: `make test` (падает).
- [x] 1.7 `TreeTest.LegacyTreeBranchesKeepRandomLength` - обычный (не-лестничный) конструктор без длины верхней ветки сохраняет случайные длины веток. Проверить: `make test` (падает или подтверждает неизменность).

## 2. Green — implement

- [x] 2.1 Добавить поле `int topBranchLength = 0` в `GeneratedObject` (`src/LevelGenerator.h`) и приватный хелпер `topBranchLengthFor(block, trunkX, side)` в `LevelGenerator`; проверить сборку `make`.
- [x] 2.2 Добавить 9-й параметр `int topBranchLength = 0` в лестничный конструктор `Tree` (`src/objects/interactive/Tree.h/.cpp`) и применить его к верхней ветке в `generateLadderBranches`; проверить прохождение тестов 1.1, 1.2, 1.7 (`make test`).
- [x] 2.3 Добавить 9-аргументную перегрузку `Fabric::createTree` (`src/Fabric.h/.cpp`), не трогая существующие перегрузки; проверить сборку `make`.
- [x] 2.4 В `LevelGenerator::buildTree` выставить `treeObj.topBranchLength` через `topBranchLengthFor`, а в `generateLevel` передать его в `Fabric::createTree`; проверить прохождение тестов 1.3–1.6 (`make test`).

## 3. Refactor and verify

- [x] 3.1 Отрефакторить новые функции так, чтобы цикломатическая сложность каждой не превышала 2 (не более одной петли / одной ветки), и убедиться, что `make test` зелёный.
- [x] 3.2 Прогнать полный `make test` и подтвердить, что тесты генерации уровня и деревьев зелёные, включая легаси-путь TMX.
- [x] 3.3 Собрать игру `make` и спот-чек сгенерированного уровня с деревом у крайнего левого/правого положения ствола: верхняя ветка доходит до края платформы, и игрок перепрыгивает на зарезервированную платформу. (Сборка успешна; геометрия проверена тестами `RealGeneratorTopBranchReachesPlatformEdge` и `RealGeneratorReservedPlatformWithinReachFromTopBranch` на 10/20 сидах — визуальный спот-чек в headless-окружении недоступен.)
