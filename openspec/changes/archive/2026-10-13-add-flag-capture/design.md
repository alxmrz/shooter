## Context

См. `proposal.md` — «Why»; требования — в `specs/flag-capture/spec.md`, `specs/dialogue-system/spec.md`, `specs/level-generation/spec.md`. Текущее состояние, важное для подхода:

- `Scene::initNewGame` (`src/Scene.cpp:41`) очищает объекты, генерирует уровень через `LevelGenerator`/`Map`, затем вызывает `GameObjects::startIntroDialogue()`.
- `LevelGenerator::generateLevel` (`src/LevelGenerator.cpp:61`) строит `std::vector<GeneratedObject>` (`generateGround` → `generatePlatforms` → `generateTrees` → `generatePlatformItems` → `generateEnemies`), после чего создаёт объекты через `Fabric` по полю `GeneratedObject::type` (ветки `Platform`, `GroundBlock`, `Tree`, `Heart`, `Ammunition`, `Crystal`, `Shooter`).
- `GameObjects` (`src/GameObjects.*`) хранит мир (`all`), оверлеи и UI-спрайты (`crystalSprite`, `cristalCountText`, `ammoSprite`, …). `drawPlayerUi` (`src/GameObjects.cpp:175`) рисует сердца, кристаллы и патроны в экранных координатах через `window->mapPixelToCoords`. `clear()` (`src/GameObjects.cpp:58`) сбрасывает `all`, `player`, `aim`, `dialogue`.
- Диалог: единственный объект `GameObjects::dialogue`; `isDialogueOpen()`; `update()` (`src/GameObjects.cpp:68`) при открытом диалоге обновляет только диалог и возвращается; `advanceDialogue()` (`src/GameObjects.cpp:90`) вызывает `dialogue->advance()`. Текст грузится через `Resources::getDialogue(type)` из `dialogueLoadQueue` (`src/Resources.h:78`); вступительный текст — `resources/dialogue/intro.txt`.
- `EventHandler::handle` (`src/EventHandler.cpp:29`) на `KeyPressed` вызывает `handleDialogueKey` (Space → `advanceDialogue`, флаг `dialogueKeyConsumed`), а игровой ввод подавляется через `isGameplayActive()` при открытом диалоге.
- Сбор предметов: `Unit::collectItems` (`src/objects/units/Unit.cpp:55`) в `move()` у игрока (`isPlayer()`) обрабатывает `Crystal`/`Ammunition`/`Heart` через `dynamic_cast` и удаляет объект из `go->all`.
- `Resources::texturesLoadQueue` (`src/Resources.h:65`) — карта «тип → путь»; `flag.png` есть в проекте, но не зарегистрирован. Текстуры грузятся в `load()`.
- `GameState` (`src/GameState.*`) живёт весь запуск приложения; `startNewGame()` (`src/GameState.cpp:124`) включает игру, запускает музыку и вызывает `scene->initNewGame()`. `GameObjects::restartGame()` (`src/GameObjects.cpp:264`) уже переиспользует `gs->startNewGame()`.
- Тесты headless: `GameObjectsFixture` (`tests/GameObjectsTestFixture.h`) с DSL без циклов и условий; `LevelGenerator_test.cpp` конструирует `LevelGenerator(nullptr)` и проверяет `GeneratedObject`-векторы. Ограничения проекта: цикломатическая сложность новых методов ≤ 2; приватные методы определяются сразу после вызывающего; TDD.

## Goals / Non-Goals

**Goals:**
- Один флаг на самой высокой платформе уровня; взятие флага игроком увеличивает суммарный счётчик флагов и переводит на следующий этап.
- Память о пройденных этапах только на время запуска процесса (без ввода-вывода на диск).
- Диалог аналитика при взятии флага; Space на последнем фрагменте перезагружает игру на новый этап.
- Вступительный диалог учитывает номер текущего этапа.
- Иконка флага и суммарный счётчик в интерфейсе игрока; счётчик не сбрасывается между этапами.

**Non-Goals:**
- Сохранение прогресса на диск или между запусками приложения.
- Несколько флагов на уровень, разное содержимое этапов, рост сложности.
- Изменение базовой механики диалога (побуквенный вывод, Space, пауза мира).
- Отдельный экран выбора этапа.

## Decisions

1. **Прогресс этапов — отдельный класс `StageProgress` (`src/StageProgress.h/.cpp`), а не поля `GameState`.** Класс хранит `stage` (стартует с 1) и `flags` (стартует с 0), даёт `getStage()`, `getFlags()`, `collectFlag()` (увеличивает оба счётчика). `GameState` владеет `StageProgress progress` и делегирует методы. Причина: `GameState` тяжело инстанцировать в headless-тестах (нужен `Application*`, деструктор обращается к `objects`), а `StageProgress` тестируется напрямую без SFML. Альтернатива — «сырые» счётчики в `GameState` (отклонена: плохо тестируется); файл на диске (отклонена пользователем: память только в процессе).

2. **Флаг — `CObject`-объект `Flag` (`src/objects/interactive/Flag.h/.cpp`) в `go->all`, создаётся `Fabric::createFlag(...)` со спрайтом из текстуры `flag`.** Взятие переиспользует существующий путь сбора предметов: в `Unit::collectItems` добавляется ветка `Flag`, которая при `collideRect` вызывает `go->collectFlag()` и удаляет объект из `go->all`. Так как `collectItems` выполняется только при `isPlayer()`, враг флаг не берёт, а дополнительных проверок не нужно. Причина: единообразие с кристаллами/патронами/сердцами. Альтернатива — проверять касание в цикле `GameState::updateWorld` (отклонена: расходится с существующим паттерном и дублирует игрока).

3. **`GameObjects::collectFlag()` — единая точка взятия флага.** Вызывает `gs->collectFlag()` (счётчики) и `startFlagDialogue()` (создание и открытие диалога завершения этапа), после чего мир автоматически встаёт на паузу, потому что `isDialogueOpen()` истинно. Сложность методов разбивается на ≤ 2; ветку `Flag` в `collectItems` выносим в приватный помощник, чтобы не растить сложность существующего метода.

4. **Тип диалога различается полем-признаком `flagDialogue` в `GameObjects`.** При закрытии диалога, если он был флаговым, `advanceDialogue()` вызывает `gs->startNewGame()` (существующий путь перезапуска). `startNewGame()` не сбрасывает `StageProgress`, поэтому этап и флаги сохраняются. `startIntroDialogue()` создаёт вступительный диалог нового этапа. Причина: минимальные изменения, переиспользование существующей паузы и обработки Space. Альтернатива — отдельный объект диалога для флага (отклонена: дублирование состояния и ветвления паузы); колбэк `onClose` (отклонена: усложняет `Dialogue`).

5. **Перезапуск этапа откладывается через флаг `stageRestartPending`.** `advanceDialogue()`: при открытом диалоге вызвать `advanceOpenDialogue()`; `advanceOpenDialogue()`: `dialogue->advance()`, затем `markStageRestartIfFlagClosed()`. Последний: если диалог закрыт и `flagDialogue` — снять признак и выставить `stageRestartPending`. Признак читает `GameState::updateNonDialogue()` через `GameObjects::consumeStageRestart()` и вызывает `startNewGame()`. Причина: `GameObjects` не зависит от `Application`/`Scene`, а перезапуск остаётся в `GameState`, где он уже жил; это также позволяет тестировать закрытие флаг-диалога без `Application`. После `startNewGame()` диалог удаляется в `clear()`, поэтому к нему больше не обращаются. Приватные методы определяются сразу после вызывающего.

5a. **`EventHandler` каждый кадр берёт актуального игрока.** `player = gameState->objects->player` в начале `handle()`. Причина: после перезапуска этапа `generateLevel` создаёт нового `Shooter`, а `EventHandler` кэшировал указатель на старого (он обновлялся только кнопкой restart), из-за чего ввод движения/прицеливания уходил в устаревший объект и персонаж «не реагировал». Альтернатива — обновлять указатель только в местах перезапуска (отклонена: легко забыть новый путь перезапуска).

6. **Флаг размещается в `LevelGenerator::generateFlag(objects)` после `generatePlatforms`.** Метод находит блок земли (`GroundBlock`) с минимальной `y` (при равенстве — с минимальным `x`, для детерминизма), добавляет `GeneratedObject` типа `Flag` размером 50×50 с `y = block.y - 50` и горизонтально центрированного. В цикле создания объектов `generateLevel` тип `Flag` создаётся через `Fabric::createFlag`. Причина: флаг — такой же сгенерированный объект, как предметы; метод тестируется на подготовленном векторе без запуска игры. Альтернатива — размещать во время `generatePlatforms` (отклонена: самый высокий блок известен только после генерации всех блоков).

7. **Вступительный диалог развилкой зависит от числа флагов.** `startIntroDialogue()` берёт базовые фрагменты `getDialogue("intro")` и фрагменты продолжения `getDialogue("intro_continue")`, затем `buildIntroFragments(...)`: при `flags == 0` возвращает базовый вступительный текст, при `flags > 0` — только короткое сообщение продолжения с подстановкой `{flags}` и `{stage}` (базовый текст не добавляется). Причина: запрос пользователя — на старте показывать обучение, а после первых флагов коротко информировать о прогрессе и желать удачи. Альтернатива — дописывать реплику к базовому тексту (отклонена: «без intro» в запросе); отдельный файл на каждый этап (отклонена: неограниченное число этапов).

8. **UI флага — по образцу кристаллов/патронов.** `GameObjects` получает `flagSprite` (текстура `flag`) и `flagCountText` (шрифт `arial`), создаваемые в конструкторе; `drawPlayerUi` рисует иконку и число `gs->getFlags()` через `mapPixelToCoords`, рядом с существующими элементами. Причина: единообразие интерфейса. Счётчик берётся из `GameState`, поэтому не сбрасывается при перезапуске уровня; в самой первой игре он равен 0.

9. **Тексты диалогов — внешние и на английском языке.** `resources/dialogue/flag.txt` содержит поздравление («Congratulations! You completed the task successfully. You are ready for the next stage. Shall we begin?»), `resources/dialogue/intro_continue.txt` — продолжение с токенами `{flags}` и `{stage}` («Welcome back, shooter. You have already captured {flags} flag(s) and reached stage {stage}. Good luck out there!»). Причина: игровой текст на английском (как существующий `intro.txt`); текст внешний и меняется без перекомпиляции. Отсутствие/пустота файла не ломает игру (пустой список фрагментов не открывает диалог).

10. **Тестируемость (TDD).** `StageProgress` тестируется напрямую (начальные значения, `collectFlag`, накопление). `LevelGenerator::generateFlag` тестируется на подготовленном векторе блоков (флаг на минимальной `y`, опора на верхней грани, ровно один флаг). В `GameObjectsFixture` добавляются DSL-хелперы без циклов/условий: `spawnFlag(...)`, `touchFlag()`, `expectFlagCount(...)`, `expectStage(...)`, `spawnStage(stage)`, `startFlagDialogue(...)`, `flagDialogueOpen()`, `closeFlagDialogueRestartsGame()`. Проверяемые случаи: взятие флага игроком увеличивает счётчик и открывает диалог; враг флаг не берёт; закрытие флаг-диалога перезапускает игру; вступление этапа 2 содержит номер этапа, этап 1 — нет.

11. **Диалог смерти использует ту же `Dialogue`, что и вступление, в режиме выбора.** В `Dialogue` добавлен режим выбора: `setChoices(labels)` рисует кнопки действия внутри окна (вместо подсказки про Space) и отключает `advance()`; `choiceAt(x, y)` возвращает индекс кнопки под точкой в экранных координатах. `GameObjects::openDeathDialogue()` создаёт диалог из `death.txt`, включает выбор `{"Yes", "No"}` и открывает его; `EventHandler` обрабатывает клик по кнопке. Причина: требование — вид как у стартового диалога (сверху, с картинкой персонажа) плюс кнопки действия; переиспользование `Dialogue` даёт тот же вид без дублирования. Альтернатива — отдельная панель с мировыми кнопками (реализована в первой версии, отклонена: другой вид, не как у вступления).

12. **«Yes» сбрасывает прогресс.** `GameObjects::restartGame()` вызывает `gs->resetProgress()` (`StageProgress::reset` → этап 1, флаги 0), затем `clear()` и `gs->startNewGame()`. Причина: требование обнулять флаги при переигровке; сброс этапа согласован с моделью (флаги и этап растут вместе) и делает вступительный диалог базовым. Альтернатива — сбрасывать только флаги (отклонена: «флаги 0, но этап > 1» ломает логику вступления). «No» обрабатывается в `EventHandler` как `app->close()` — тем же путём, что «Exit».

13. **Сложность новых методов ≤ 2.** В `Dialogue` добавлены `layoutFooter` / `layoutChoices` / `drawContent` / `drawFooter` / `drawChoices` / `choiceBoxShape` / `choiceLabel`, а `choiceAt` использует `std::find_if` и `std::distance`. В `GameObjects` — `openDeathDialogue` / `isDeathDialogueOpen` / `deathChoiceAt`. В `EventHandler` — `handleDeathChoice` / `clickDeathChoice` / `applyDeathChoice`. Приватные методы определены сразу после вызывающего.

## Risks / Trade-offs

- **Риск:** обращение к `dialogue` после перезапуска (диалог удаляется в `clear()`) ведёт к висячему указателю. → **Смягчение:** перезапуск отложен (`stageRestartPending`) и выполняется в `GameState` после выхода из `advanceDialogue()`; к `dialogue` после этого не обращаются; тест на закрытие флаг-диалога.
- **Риск:** `EventHandler` держит указатель на старого игрока после перезапуска этапа, и ввод уходит в устаревший объект («персонаж не двигается»). → **Смягчение:** указатель `player` обновляется в начале каждого `handle()`; старый указатель больше не используется.
- **Риск:** взятие флага засчитывается только во время `move()`; стоящий игрок, не двигаясь, не активирует сбор. → **Смягчение:** чтобы попасть на платформу с флагом, игрок двигается или падает (оба пути идут через `move()` → `collectItems`); при необходимости позже добавить проверку по кадру, не меняя требования.
- **Риск:** флаг визуально пересекается с предметом, уже стоящим на самой высокой платформе (кристалл/патрон/сердце). → **Смягчение:** флаг центрируется; возможное наложение безвредно (оба объекта собираемы). Подавление предметов на верхней платформе отклонено, чтобы не менять существующее детерминированное распределение предметов и его тесты.
- **Риск:** незарегистрированная текстура `flag` приводит к `ResourceNotLoaded` при старте. → **Смягчение:** добавить `{"flag", "resources/images/flag.png"}` в `texturesLoadQueue`; файл уже есть; визуальная проверка `./run`.
- **Риск:** вступление становится длиннее из-за добавленной реплики. → **Смягчение:** добавляется один фрагмент; побуквенный вывод и Space обрабатывают его штатно.
- **Риск:** двойной вызов `playBackgroundMusic()` при перезапуске (музыка уже играет). → **Смягчение:** принимаем существующее поведение `startNewGame()`; звук не влияет на требования.
- **Риск:** `GameObjects::clear()` не освобождает объекты `all` (существующее поведение), флаг «утекает» при перезапуске. → **Смягчение:** не расширяем проблему, следуем текущему паттерну `clear()`.

## Migration Plan

1. Добавить `StageProgress` (`src/StageProgress.h/.cpp`), поле в `GameState`, делегирующие методы `getStage()`, `getFlags()`, `collectFlag()`. Тесты `StageProgress` — прямыми вызовами.
2. Зарегистрировать текстуру `flag` и файлы диалогов в `Resources`; создать `resources/dialogue/flag.txt` и `resources/dialogue/intro_stage.txt`.
3. Создать `Flag` (`src/objects/interactive/Flag.h/.cpp`) и `Fabric::createFlag(...)`; добавить вспомогательный метод создания диалога из готового набора фрагментов.
4. В `Unit::collectItems` добавить взятие флага через приватный помощник → `GameObjects::collectFlag()`.
5. В `LevelGenerator` добавить `generateFlag` (самая высокая платформа, минимальный `y`) и создание объекта `Flag` в `generateLevel`; тесты на `generateFlag`.
6. В `GameObjects`: `startFlagDialogue()`, признак `flagDialogue`, отложенный перезапуск (`stageRestartPending`) в `advanceDialogue()`; `startIntroDialogue()` выбирает базовое вступление или продолжение по числу флагов; UI флага (`flagSprite`, `flagCountText`, отрисовка в `drawPlayerUi`). В `GameState` обработать `consumeStageRestart()`. В `EventHandler` обновлять `player` каждый кадр.
7. Диалог смерти: в `Dialogue` добавить режим выбора (`setChoices`/`hasChoices`/`choiceAt`, кнопки внутри окна, отключение `advance()`); `GameObjects::openDeathDialogue()` открывает диалог из `death.txt` с кнопками `{"Yes", "No"}`; `restartGame()` дополняется `gs->resetProgress()`; `EventHandler` обрабатывает клик по кнопке (Yes — переигровка, No — `app->close()`) и не перелистывает диалог смерти по Space; `StageProgress::reset()` и `GameState::resetProgress()`.
8. Прогнать `make test` (всё зелёное), затем `make`; запустить `./run` и визуально проверить: флаг на самой высокой платформе, взятие открывает англоязычный диалог, Space перезагружает на новый этап (персонаж снова управляем), вступление показывает базовый текст без флагов и короткое сообщение о флагах/этапе при их наличии, счётчик флагов и иконка видны и растут, смерть открывает диалог с «Yes»/«No» (Yes сбрасывает флаги, No закрывает игру).
   Откат: удалить `Flag`, `StageProgress` и вызов `generateFlag`, вернуть `Unit::collectItems`, `GameObjects`, `GameState` и `EventHandler` к прежней логике; прочие системы не затронуты.

## Open Questions

Нет.
