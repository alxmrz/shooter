## ADDED Requirements

### Requirement: Enemy spawn frequency on platforms
Враги ДОЛЖНЫ появляться на блоках земли (платформах) не чаще, чем один враг на каждые десять блоков земли.

#### Scenario: Enemy stands on a ground block
- **WHEN** генератор размещает врага
- **THEN** враг стоит на верхней грани блока земли

#### Scenario: At most one enemy per ten platforms
- **WHEN** генератор размещает врагов
- **THEN** число врагов не превышает числа блоков земли, делённого на 10, и не превышает `MAX_ENEMIES`

#### Scenario: Enemies stay outside the camera view
- **WHEN** генератор размещает врага
- **THEN** враг находится за пределами видимости камеры игрока
