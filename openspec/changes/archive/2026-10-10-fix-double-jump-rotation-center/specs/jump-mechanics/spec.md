## ADDED Requirements

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
