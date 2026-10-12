## 1. Тесты (Red)

- [x] 1.1 Создать `tests/ShooterSprite_test.cpp` через `GameObjectsFixture` (`tests/GameObjectsTestFixture.h`) и зарегистрировать его в `tests/CMakeLists.txt`; запустить `make test` и убедиться, что новый файл собирается
- [x] 1.2 `SetMainSpriteInitializesIdleFrame` - после `setMainSprite()` `textureRect` спрайта стрелка равен кадру покоя для направления `right` (`sf::IntRect(24, 144, 50, 50)`), а не полной текстуре; запустить `make test` и убедиться, что тест падает
- [x] 1.3 `MovingFrameShownBeforeInterval` - после `move("right")` вызов `refreshSpriteFrame(0.f)` задаёт первый кадр бега `right` (`sf::IntRect(26, 315, 50, 50)`); запустить `make test` и убедиться, что тест падает
- [x] 1.4 `MovingFrameAdvancesAfterInterval` - после `move("right")` вызов `refreshSpriteFrame(animationTime)` задаёт следующий кадр бега `right` (`sf::IntRect(76, 315, 50, 50)`); запустить `make test` и убедиться, что тест падает
- [x] 1.5 `IdleFrameStableOnRefresh` - без движения повторный `refreshSpriteFrame(dt)` сохраняет кадр покоя `right`; запустить `make test` и убедиться, что тест падает или подтверждает уже корректное поведение
- [x] 1.6 Вынести повторяющиеся проверки кадра в DSL-функции без циклов и условий (например, `expectSpriteFrame(sprite, rect)`) и переиспользовать их во всех тестах раздела; запустить `make test` и убедиться, что тесты по-прежнему падают только по причине отсутствия фикса

## 2. Реализация (Green)

- [x] 2.1 Переопределить `Shooter::setMainSprite()`: после `Unit::setMainSprite()` установить `mainSprite->setTextureRect(noMotionSprites[direction])`; проверить прохождение теста 1.2 через `make test`
- [x] 2.2 Объявить публичный `void refreshSpriteFrame(float dt)` в `Shooter.h` и реализовать: для живого стрелка при `falling || jumping` — кадр прыжка, при `moving` — текущий кадр бега, иначе — кадр покоя; проверить прохождение тестов 1.3–1.5 через `make test`
- [x] 2.3 Разделить показ и продвижение кадра бега: назначение `runSprites[direction][currentFrame]` выполнять всегда при `moving`, а продвижение `currentFrame` и сброс `elapsedTime` — только при `elapsedTime >= animationTime`; проверить тест 1.4 через `make test`
- [x] 2.4 Вызвать `refreshSpriteFrame(dt)` из `Shooter::draw()` для живого стрелка, сохранив путь взрыва для мёртвого; проверить, что весь набор `make test` зелёный

## 3. Рефакторинг

- [x] 3.1 Декомпозировать `refreshSpriteFrame()` на мелкие приватные методы (`refreshJumpFrame()`, `refreshMovingFrame()`, `advanceRunFrame()`, `refreshIdleFrame()`) так, чтобы цикломатическая сложность каждого метода не превышала 2, а приватные методы были объявлены сразу после вызывающего; проверить `make test`
- [x] 3.2 Удалить дублирование назначения кадра из `animateRun()` (или заменить его на новые методы), сохранив поведение анимации взрыва; проверить `make test`

## 4. Верификация

- [x] 4.1 Собрать проект командой `make` и убедиться в отсутствии ошибок компиляции
- [x] 4.2 Запустить `./run` и визуально проверить: при появлении движущегося врага нет вспышки полного атласа, первый кадр — корректный кадр бега
- [x] 4.3 Убедиться, что анимации бега, прыжка, покоя и взрыва, а также движение, стрельба и коллизии работают без изменений
