## 1. Тесты (Red)

- [x] 1.1 Расширить DSL `tests/GameObjectsTestFixture.h` хелперами без циклов/условий: `spawnFlag(x, y)`, `touchFlag()`, `flagPresent()`, `expectFlagCount(n)`, `expectStage(n)`, `setStage(n)`, `startFlagDialogue(fragments)`, `flagDialogueOpen()`, `startStageIntro(stage)`, `introFragmentsContain(text)`; добавить `StageProgress_test.cpp`, `Flag_test.cpp`, `FlagDialogue_test.cpp` в `tests/CMakeLists.txt`; запустить `make test` и убедиться, что набор собирается
- [x] 1.2 `StageStartsAtStageOneAndZeroFlags - начальные этап и счётчик флагов` — `StageProgress` сразу после создания: `getStage() == 1` и `getFlags() == 0`; `make test` падает
- [x] 1.3 `CollectFlagIncrementsFlagsAndAdvancesStage - взятие флага увеличивает счётчик и этап` — после `collectFlag()` `getFlags() == 1` и `getStage() == 2`; повторный вызов даёт `2` и `3`; `make test` падает
- [x] 1.4 `FlagsAccumulateAcrossStages - счётчик флагов не сбрасывается` — после двух взятий флагов на разных этапах `getFlags() == 2`, при этом `getStage() == 3`; `make test` падает
- [x] 1.5 `FlagPlacedOnHighestPlatform - флаг на блоке земли с минимальным y` — `generateFlag` на подготовленном векторе блоков ставит флаг на блок с минимальной `y` (при равенстве — с минимальным `x`); `make test` падает
- [x] 1.6 `FlagRestsOnPlatformTop - флаг стоит на верхней грани платформы` — `y` флага равен `верх блока − высота флага`, флаг горизонтально центрирован на блоке; `make test` падает
- [x] 1.7 `ExactlyOneFlagGenerated - ровно один флаг на уровень` — после `generateFlag` в векторе ровно один объект типа `Flag`; `make test` падает
- [x] 1.8 `PlayerCapturesFlagOnTouch - игрок касается флага, счётчик растёт` — игрок, коснувшийся флага, вызывает рост `getFlags()` на 1, а флаг исчезает из `go->all`; `make test` падает
- [x] 1.9 `PlayerCaptureOpensFlagDialogue - взятие флага открывает диалог завершения этапа` — после касания флага `flagDialogueOpen()` истинно; `make test` падает
- [x] 1.10 `EnemyDoesNotCaptureFlag - враг не берёт флаг` — касание флага врагом не меняет `getFlags()` и не удаляет флаг; `make test` падает
- [x] 1.11 `ClosingFlagDialogueRestartsGame - закрытие диалога перезапускает игру` — Space на последнем фрагменте флаг-диалога делает диалог закрытым и начинает новый этап (номер этапа увеличился, флаг-диалог больше не открыт); `make test` падает
- [x] 1.12 `SpaceDuringFlagTypingRevealsFragment - Space во время печати не перезапускает игру` — Space при неполной печати показывает фрагмент целиком, этап не меняется; `make test` падает
- [x] 1.13 `WorldPausedDuringFlagDialogue - мир на паузе при диалоге завершения этапа` — при открытом флаг-диалоге `UpdateSpy` не получает `update`; `make test` падает
- [x] 1.14 `FirstStageIntroHasNoStageNote - вступление этапа 1 без реплики про этап` — вступительные фрагменты для этапа 1 не содержат текста про номер этапа; `make test` падает
- [x] 1.15 `LaterStageIntroMentionsStageNumber - вступление этапа 2 содержит номер этапа` — вступительные фрагменты для этапа 2 содержат фразу с числом `2`; `make test` падает

## 2. Реализация (Green)

- [x] 2.1 Создать `src/StageProgress.h/.cpp` с `getStage()`, `getFlags()`, `collectFlag()`; проверить тесты 1.2–1.4 через `make test`
- [x] 2.2 Зарегистрировать текстуру `flag` в `Resources::texturesLoadQueue`, добавить `flag` и `intro_stage` в `dialogueLoadQueue`; создать `resources/dialogue/flag.txt` (поздравление) и `resources/dialogue/intro_stage.txt` (фраза с токеном `{stage}`); убедиться, что `./run` не падает без/с файлами
- [x] 2.3 Создать `Flag` (`src/objects/interactive/Flag.h/.cpp`) и `Fabric::createFlag(...)` со спрайтом из текстуры `flag`; добавить вспомогательный метод `Fabric` для диалога из готового набора фрагментов; проверить сборку `make`
- [x] 2.4 Добавить `LevelGenerator::generateFlag(std::vector<GeneratedObject>&)` (самый верхний блок земли, минимальный `y`, тай-брейк по `x`) и создание типа `Flag` в цикле объектов `generateLevel`; проверить тесты 1.5–1.7 через `make test`
- [x] 2.5 В `Unit::collectItems` добавить взятие флага через приватный помощник, вызывающий `GameObjects::collectFlag()`; реализовать `GameObjects::collectFlag()` (счётчики + открытие диалога) и удаление флага из `go->all`; проверить тесты 1.8–1.10 через `make test`
- [x] 2.6 В `GameObjects` добавить `startFlagDialogue()` и признак `flagDialogue`; в `advanceDialogue()` разложить логику так, чтобы закрытие флаг-диалога вызывало `gs->startNewGame()` (не обращаясь к удалённому диалогу); проверить тесты 1.11–1.13 через `make test`
- [x] 2.7 В `GameState` связать `StageProgress` (делегирующие `getStage()`, `getFlags()`, `collectFlag()`); в `GameObjects::startIntroDialogue()` собирать вступление: базовые фрагменты `intro` и, при `getStage() > 1`, фрагменты `intro_stage` с подстановкой `{stage}`; проверить тесты 1.14–1.15 через `make test`
- [x] 2.8 Добавить в `GameObjects` `flagSprite` (текстура `flag`) и `flagCountText` (шрифт `arial`); в `drawPlayerUi` рисовать иконку и число `gs->getFlags()` рядом с кристаллами и патронами; проверить визуально в `./run`, что счётчик начинается с 0 и растёт при взятии флага

## 3. Рефакторинг

- [x] 3.1 Разложить новые методы (`collectFlag`, `advanceDialogue`, разбор вступления, сборку флага) так, чтобы цикломатическая сложность каждого не превышала 2, а приватные методы определялись сразу после вызывающего; проверить `make test`
- [x] 3.2 Вынести признак «диалог открыт» и сборку/открытие диалога из готового набора фрагментов в общие методы, переиспользуемые вступлением, флаг-диалогом и паузой; проверить `make test`
- [x] 3.3 Убедиться, что тексты диалогов читаются из внешних файлов без дублирования строк в коде, а отсутствие файла даёт пустой список без сбоя; проверить `make test`

## 4. Верификация

- [x] 4.1 Собрать проект `make` и убедиться в отсутствии ошибок компиляции
- [x] 4.2 Прогнать `make test` и убедиться, что весь набор тестов зелёный
- [x] 4.3 Запустить `./run` и визуально проверить: флаг стоит на самой высокой платформе, касание флага открывает диалог с поздравлением, мир на паузе, Space на последнем фрагменте перезагружает игру, вступление этапа 2 содержит номер этапа, а иконка и счётчик флагов видны и увеличиваются
- [x] 4.4 Убедиться, что после закрытия флаг-диалога движение, прыжок, стрельба, ИИ, коллизии, генерация уровней и сбор кристаллов/патронов/сердец работают без изменений

## 5. Доработки после проверки

- [x] 5.1 Исправить устаревший указатель `player` в `EventHandler` (обновлять в начале каждого `handle()`), чтобы после перезапуска этапа персонаж снова управлялся; проверить сборку `make` и визуально `./run`
- [x] 5.2 Перевести текст диалога взятия флага `resources/dialogue/flag.txt` на английский и проверить фактический текст через `Resources::getDialogue`
- [x] 5.3 `FirstStartShowsBaseIntro - при отсутствии флагов показывается базовое вступление` — `buildIntroFragments(flags=0)` возвращает базовый текст и не содержит сообщения продолжения; `make test` зелёный
- [x] 5.4 `ContinuationReportsFlagsAndStage - при наличии флагов показывается короткое сообщение о прогрессе` — `buildIntroFragments(flags>0)` возвращает продолжение с числом флагов и номером этапа и не содержит базового текста; `make test` зелёный

## 6. Диалог смерти и сброс прогресса

- [x] 6.1 `ResetClearsProgress - сброс прогресса обнуляет этап и флаги` — `StageProgress::reset()` возвращает этап 1 и флаги 0; `make test` зелёный
- [x] 6.2 `ResetProgressClearsFlags - сброс прогресса через GameState` — `GameState::resetProgress()` обнуляет флаги и этап; `make test` зелёный
- [x] 6.3 Сделать диалог смерти по виду как стартовый: добавить в `Dialogue` режим выбора (`setChoices`/`hasChoices`/`choiceAt`, кнопки действия внутри окна под серый фон, `advance()` отключён), `GameObjects::openDeathDialogue()` открывает диалог из `death.txt` с кнопками `{"Yes","No"}`; проверить сборку `make` и хит-тест кнопок на отрисованном окне
- [x] 6.4 `restartGame()` сбрасывает прогресс (`gs->resetProgress()`); клик по «Yes» переигрывает забег, по «No» закрывает игру (обработка `deathChoiceAt`/`applyDeathChoice` в `EventHandler`), Space не листает диалог смерти; проверить сборку `make` и визуально `./run`
- [x] 6.5 Прогнать `make test` — весь набор зелёный (размещение флага, взятие, прогресс, диалог этапа, режим выбора, сброс прогресса)
- [x] 6.6 `DialogueChoicesDisableAdvance - диалог с кнопками не перелистывается по Space` — при `hasChoices()` вызов `advance()` не закрывает диалог и не меняет индекс; `make test` зелёный
- [x] 6.7 `DialogueWithoutChoicesKeepsAdvancing - диалог без кнопок закрывается по Space` — без кнопок `advance()` на последнем фрагменте закрывает диалог; `make test` зелёный
