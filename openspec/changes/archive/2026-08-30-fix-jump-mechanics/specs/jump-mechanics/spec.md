## Цель

Определяет механику двойного прыжка с двухуровневой высотой прыжка и блокировкой тройного прыжка.

## ADDED Requirements

### Requirement: Ground jump height
Когда игрок находится на земле и нажимает кнопку прыжка, игрок ДОЛЖЕН прыгнуть с базовой высотой, равной высоте игрового персонажа (50 пикселей).

#### Scenario: Valid ground jump
- **WHEN** player is on ground and presses jump button
- **THEN** player jumps 50 pixels above starting position

#### Scenario: Invalid ground jump during air time
- **WHEN** player is in air (has already jumped once)
- **THEN** pressing jump does not trigger a second ground jump

### Requirement: Air jump height
После первого прыжка (наземного), когда игрок нажимает кнопку прыжка снова, находясь в воздухе, игрок ДОЛЖЕН прыгнуть на 20% выше первого прыжка (60 пикселей).

#### Scenario: Valid air jump
- **WHEN** player has performed first jump and presses jump again while in air
- **THEN** player jumps 60 pixels above starting position (20% higher than first jump)

#### Scenario: Air jump with different ground jump heights
- **WHEN** player experiences any valid air jump
- **THEN** second jump height is consistently 20% higher than first jump (60 pixels)

### Requirement: Jump limit
После второго прыжка игрок НЕ ДОЛЖЕН иметь возможности сделать третий прыжок до приземления на землю.

#### Scenario: Triple jump prevention
- **WHEN** player has performed both jumps and is in air
- **THEN** pressing jump has no effect on player vertical velocity

#### Scenario: Ground reset
- **WHEN** player lands on ground after either jump
- **THEN** player's jump count resets to zero and both jumps become available again

### Requirement: Jump completion
Прыжки НЕ ДОЛЖНЫ прерываться или отменяться от повторного нажатия кнопки прыжка в течение одной дуги прыжка.

#### Scenario: Jump completion without interruption
- **WHEN** player performs a jump (ground or air) and releases the jump button
- **THEN** the jump arc completes without external influence from repeated jump button presses

#### Scenario: Jump cancellation prevention
- **WHEN** player presses jump multiple times during a single jump arc
- **THEN** the jump arc is not altered or cancelled
