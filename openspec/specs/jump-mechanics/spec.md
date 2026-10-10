## Purpose

Определяет механику двойного прыжка с двухуровневой высотой прыжка и блокировкой тройного прыжка.

## Requirements

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

### Requirement: Double jump rotation pivot
При двойном прыжке персонаж ДОЛЖЕН вращаться вокруг центра своего спрайта, а не вокруг координаты верхнего левого угла.

#### Scenario: Rotation around center
- **WHEN** player performs a double jump and rotation begins
- **THEN** the sprite rotates around its own geometric center rather than its top-left corner

#### Scenario: No positional shift from rotation
- **WHEN** the character rotates during a double jump
- **THEN** the on-screen position of the character (its collision box) does not shift as a result of the rotation

### Requirement: Rotation preserves existing behavior
Вращение при двойном прыжке ДОЛЖНО сохранять текущую длительность (360° за 0.5 секунды) и завершаться по достижении полного оборота.

#### Scenario: Full rotation duration
- **WHEN** rotation starts on a double jump
- **THEN** the character completes a full 360° turn in approximately 0.5 seconds at the existing rotation speed

#### Scenario: Rotation stops after full turn
- **WHEN** the character completes a full 360° rotation
- **THEN** rotation stops and the sprite returns to its normal orientation
