## 1. Твёрдые коллизии блоков земли (TDD)

- [x] 1.1 (Red) Добавить в `tests/GroundBlockCollision_test.cpp` падающий тест: персонаж `Shooter` падает сверху на `GroundBlock`, ожидается остановка на верхней грани; подключить файл в `tests/CMakeLists.txt`. Запустить `make test` и убедиться, что тест падает (персонаж проваливается).
- [x] 1.2 (Green) Изменить `src/objects/backgrounds/GroundBlock.h/.cpp`: унаследовать `GroundBlock` от `Platform`, вызывать цветной конструктор `Platform(...)` с прежним цветом и убрать дублирующий `rect`/`draw`/`getDrawForm`. Запустить `make test` — тест 1.1 зелёный.
- [x] 1.3 (Red→Green) Добавить тесты: движение вбок в блок отклоняется; прыжок снизу в блок отклоняется; движение без пересечения не отклоняется. Убедиться, что все проходят при реализации 1.2.
- [x] 1.4 Регрессия: прогнать `tests/GroundBlock_test.cpp`, `tests/ShooterPlatformCollision_test.cpp`, `tests/PlatformCollision_test.cpp` — все зелёные, поведение `GroundBlock` и цвет отрисовки сохранены.

## 2. Минимальный зазор между блоками земли (TDD)

- [x] 2.1 (Red) Добавить в `tests/LevelGenerator_test.cpp` тест: для результата `LevelGenerator::generatePlatforms()` никакая пара `GroundBlock` не нарушает зазор — расширенные на 50px прямоугольники не пересекаются (100px по одной из осей). Запустить `make test` и убедиться, что тест падает на текущем случайном размещении.
- [x] 2.2 (Green) Реализовать в `src/LevelGenerator.cpp` пространственный хэш-грид (размер ячейки = 100px): для каждого кандидата проверять только блоки из пересекаемых ячеек, блоки с недостаточным зазором отклонять с ограниченным числом повторных попыток `x`. Запустить `make test` — тест 2.1 зелёный.
- [x] 2.3 Добавить тесты: блоки на одной высоте имеют горизонтальный зазор ≥100px; блоки с пересечением по X имеют вертикальный зазор ≥100px; диагональные пары имеют зазор ≥100px хотя бы по одной оси. Все проходят.

## 3. Проходимость и отсутствие O(n²)

- [x] 3.1 Добавить тест: вертикальный шаг между соседними уровнями блоков не превышает `MAX_JUMP_HEIGHT` (110px). Запустить `make test` — зелёный.
- [x] 3.2 Добавить тест/инструментарий, считающий число проверок расстояния при генерации: при увеличении числа размещаемых блоков проверки растут не квадратично (например, счётчик проверок на блок ограничен константой). Запустить `make test` — зелёный.

## 4. Обновление существующих тестов и финальная проверка

- [x] 4.1 Обновить устаревшую копию `testGeneratePlatforms` в `tests/LevelGenerator_test.cpp` (или заменить её вызовом реального `LevelGenerator`) так, чтобы она отражала новое размещение с зазором; тесты `PlatformsAreGeneratedWithVaryingLengths` и `PlatformVerticalSpacingDoesNotExceedMaxJumpHeight` проходят.
- [x] 4.2 Финальная проверка: `make` собирает проект без ошибок, `make test` — все тесты зелёные.

## 5. Приведение размеров уровня к спеке (выявлено при реализации)

- [x] 5.1 Привести константы `LevelGenerator` к спеке «3000x3000»: `LEVEL_WIDTH = 3000`, `LEVEL_HEIGHT = 3000`, `GROUND_Y = 2950`; проверить, что `RealGeneratorPlacesAtMostOneTreePer500Pixels` и остальные тесты проходят.
- [x] 5.2 Вынести проверку зазора в класс `SpacingGrid` (`src/SpacingGrid.h`) и покрыть его тестами (`tests/SpacingGrid_test.cpp`), включая линейный рост числа проверок.

## 6. Доработки генерации (по запросу)

- [x] 6.1 Определить константы прыжка и `MAX_JUMP_HEIGHT = DOUBLE_JUMP_HEIGHT - PLAYER_HEIGHT` (110px); тест `MaxVerticalDistanceFollowsDoubleJumpMinusPlayerHeight` проходит, вертикальный шаг не превышает `MAX_JUMP_HEIGHT`.
- [x] 6.2 Уменьшить число блоков вдвое: ровно один блок на уровень; тест `RealGeneratorPlacesSingleBlockPerLevel` проходит.
- [x] 6.3 Увеличить ширину блоков в 1.2-2.0 раза (случайно); тест `PlatformsAreGeneratedWithVaryingLengths` учитывает новые диапазоны.
- [x] 6.4 Добавить фиксированный стартовый блок в нижнем левом углу и создавать персонажа на нём; тесты `RealGeneratorPlacesFixedStartBlockInBottomLeft` и `RealGeneratorPlacesSingleBlockPerLevel` проходят.
- [x] 6.5 Размещать деревья на блоках земли шириной ≥500px, не более одного дерева на блок; тесты `RealGeneratorPlacesTreesOnWideGroundBlocks` и `RealGeneratorPlacesAtMostOneTreePerBlock` проходят.
- [x] 6.6 Финальная проверка: `make` собирает проект, `make test` — все тесты зелёные (159).
- [x] 6.7 Ограничить появление деревьев: не чаще, чем на каждом третьем блоке земли; тест `RealGeneratorPlacesAtMostOneTreePerThreeBlocks` проходит.
- [x] 6.8 Размещать врагов на блоках земли не чаще одного на каждые десять блоков, вне области видимости камеры; тесты `RealGeneratorPlacesAtMostOneEnemyPerTenPlatforms` и `RealGeneratorPlacesEnemiesOnPlatformsOutsideCamera` проходят.

