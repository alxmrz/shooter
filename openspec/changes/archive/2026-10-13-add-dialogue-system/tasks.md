## 1. Тесты (Red)

- [x] 1.1 Расширить DSL `tests/GameObjectsTestFixture.h` хелперами без циклов/условий в тестах: `spawnDialogue(fragments, charsPerSecond)`, `advanceDialogue()`, `dialogueOpen()`, `dialogueVisibleText()`, `expectDialogueText(is)`, `dialogueIndex()`, `dialoguePromptShown()`, `updateDialogue(dt)`, `expectObjectNotUpdated(...)`; добавить `Dialogue_test.cpp` и `DialoguePause_test.cpp` в `tests/CMakeLists.txt`; запустить `make test` и убедиться, что набор собирается
- [x] 1.2 `TypewriterRevealsPrefix - текст появляется побуквенно` — после `updateDialogue(dt)` с малым `dt` видимый текст равен префиксу фрагмента, а не всему фрагменту; `make test` падает
- [x] 1.3 `TypewriterCompletesAfterEnoughTime - по прошествии времени виден весь фрагмент` — после достаточного суммарного `dt` видимый текст равен полному фрагменту; `make test` падает
- [x] 1.4 `AdvanceDuringTypingRevealsWholeFragment - Space во время печати показывает фрагмент целиком` — при неполной печати `advanceDialogue()` делает видимым весь текущий фрагмент и не переходит к следующему; `make test` падает
- [x] 1.5 `AdvanceAfterCompleteShowsNextFragment - Space при полном фрагменте переходит к следующему` — после полной печати `advanceDialogue()` увеличивает индекс фрагмента и начинает новый с пустого/короткого префикса; `make test` падает
- [x] 1.6 `AdvanceOnLastFragmentClosesDialogue - Space на последнем фрагменте закрывает диалог` — после полной печати последнего фрагмента `advanceDialogue()` делает `dialogueOpen()` ложным; `make test` падает
- [x] 1.7 `EmptyFragmentsKeepDialogueClosed - пустой список фрагментов не открывает диалог` — при `spawnDialogue({})` `dialogueOpen()` ложно; `make test` падает
- [x] 1.8 `WorldPausedWhileDialogueOpen - мир не обновляется при открытом диалоге` — при открытом диалоге объект-спай в `go->all` не получает `update`, но диалог продолжает печатать; `make test` падает
- [x] 1.9 `WorldResumesAfterDialogueClosed - после закрытия диалога мир обновляется` — после закрытия диалога объект-спай снова получает `update`; `make test` падает
- [x] 1.10 `DialogueLinesIgnoreBlankLines - пустые строки не становятся фрагментами` — разбор текста файла отбрасывает пустые строки и сохраняет порядок непустых; `make test` падает
- [x] 1.11 `PromptShownWhileOpenHiddenAfterClose - подсказка видна только при открытом диалоге` — `dialoguePromptShown()` истинно при открытом диалоге и ложно после закрытия; `make test` падает

## 2. Реализация (Green)

- [x] 2.1 Зарегистрировать текстуру `analytic` (`resources/images/analytic.png`) в `Resources::texturesLoadQueue` и добавить `dialogueLoadQueue` (`{"intro", "resources/dialogue/intro.txt"}`) с `getDialogue(type)`, читающим непустые строки; проверить, что `make` собирается и нет исключений на загрузке
- [x] 2.2 Создать `resources/dialogue/intro.txt` с вступительным текстом (несколько фрагментов по строкам); запуск `./run` не падает без файла и с ним
- [x] 2.3 Добавить класс `Dialogue` (`src/ui/Dialogue.h/.cpp`): фрагменты, `update(dt)` с побуквенным выводом, `advance()` с разложением на методы сложности ≤ 2, `isOpen()`, `getVisibleText()`, признак подсказки; проверить тесты 1.2–1.7, 1.10 через `make test`
- [x] 2.4 Реализовать отрисовку `Dialogue::draw(target, w, h)` в экранных координатах: окно шириной 4/5 экрана сверху, слева изображение `analytic.png` (с сохранением пропорций, без рамки), справа текст с переносом строк, подсказка «Press space to continue»; проверить визуально в `./run`
- [x] 2.5 Добавить `Fabric::createDialogue(type)`, поле `GameObjects::dialogue`, `GameObjects::startIntroDialogue()` (создаёт и открывает диалог при непустом списке) и сброс `dialogue` в `clear()`; проверить сборку `make`
- [x] 2.6 Вызвать `startIntroDialogue()` из `Scene::initNewGame` после генерации уровня; проверить в `./run`, что в начале каждой новой игры и при рестарте диалог появляется с первого фрагмента
- [x] 2.7 Ввести паузу в `GameState::update`: при открытом диалоге обновлять только диалог и делать `resetElapsedTime()`, не обновляя мир; проверить тесты 1.8–1.9 через `make test`
- [x] 2.8 В `EventHandler` обрабатывать `Space` как дискретное событие `KeyPressed` для диалога (`advance`), подавлять игровой ввод при открытом диалоге (условие в `isGameplayActive`), не стрелять на кадре закрытия; проверить тест 1.11 и сборку `make`, визуально в `./run` — во время диалога игрок не двигается и не стреляет

## 3. Рефакторинг

- [x] 3.1 Разложить новые методы (`advance`, побуквенный вывод, сборка окна, разбор строк) так, чтобы цикломатическая сложность каждого не превышала 2, а приватные методы объявлялись сразу после вызывающего; проверить `make test`
- [x] 3.2 Вынести общий признак «диалог открыт» в один метод и переиспользовать его в `GameState`, `EventHandler` и `GameObjects::draw`; проверить `make test`

## 4. Верификация

- [x] 4.1 Собрать проект `make` и убедиться в отсутствии ошибок компиляции
- [x] 4.2 Прогнать `make test` и убедиться, что весь набор тестов зелёный
- [x] 4.3 Запустить `./run` и визуально проверить: окно сверху игровой области шириной 4/5, изображение `analytic.png` слева без рамки, текст справа печатается побуквенно, подсказка про пробел видна, `Space` завершает печать / листает фрагменты / закрывает диалог, до закрытия игрок и враги замерли
- [x] 4.4 Убедиться, что после закрытия диалога движение, прыжок, стрельба, ИИ, коллизии и генерация уровней работают без изменений, и что отсутствие/пустота файла диалога не мешает начать игру
