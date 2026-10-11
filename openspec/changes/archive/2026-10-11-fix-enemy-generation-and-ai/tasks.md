## 1. Enemy collision participation (red)

- [x] 1.1 Create `tests/EnemyCollision_test.cpp` with test `EnemyLandsOnGroundBlock - enemy falling onto a ground block stops on its top edge`; verify it fails with the current player-only collision handling
- [x] 1.2 Add test `EnemyDoesNotFallToBottom - enemy above a platform stays on it after gravity`; verify it fails
- [x] 1.3 Add test `EnemyBlockedFromSide - horizontal move into a ground block is reverted`; verify it fails
- [x] 1.4 Add test `EnemyBlockedFromBelow - upward move into a ground block is reverted`; verify it fails

## 2. Enemy collision participation (green)

- [x] 2.1 Extract the platform-collision body of `Unit::move` into a private resolver helper defined right after its caller and apply it to every unit, not only the player
- [x] 2.2 Verify tests 1.1-1.4 pass and that the existing `GroundBlockCollision`, `PlatformCollision`, and `ShooterPlatformCollision` suites still pass
- [x] 2.3 Register `EnemyCollision_test.cpp` in `tests/CMakeLists.txt` and verify `make test` builds and runs it
- [x] 2.4 Confirm each changed method keeps cyclomatic complexity at most 2 (extract helpers as needed)

## 3. Solid-surface query for ledge detection (red)

- [x] 3.1 Create `tests/AI_test.cpp` with a GameObjects fixture DSL (platforms + player + enemy); add test `HasSupportAhead - returns true while ground continues under the next step`; verify it fails
- [x] 3.2 Add test `HasSupportAhead - returns false when the next step is past the platform edge`; verify it fails

## 4. Solid-surface query (green)

- [x] 4.1 Implement the shared solid-surface query over collidable platforms in `go->all` and use it from `AI` ledge detection
- [x] 4.2 Verify tests 3.1-3.2 pass and that the query does not mutate the unit (no side effects)
- [x] 4.3 Register `AI_test.cpp` in `tests/CMakeLists.txt` and verify it builds and runs

## 5. Enemy generation (red)

- [x] 5.1 Add test `EnemyOnEverySecondCrystalPlatform - one enemy on every 2nd crystal platform`; verify it fails against the every-third rule
- [x] 5.2 Add test `EnemySkippedWithoutCrystal - a platform without a crystal gets no enemy`; verify it fails
- [x] 5.3 Add test `OneEnemyPerPlatform - no two enemies share a platform`; verify it fails
- [x] 5.4 Add test `NoMaxEnemiesCap - enemy count follows the every-second-crystal rule, not MAX_ENEMIES`; verify it fails
- [x] 5.5 Add test `EnemyStandsOnPlatformTop - enemy y equals platform top minus enemy height`; verify it fails
- [x] 5.6 Add test `EnemyOutsideCameraView - every generated enemy is outside the initial camera view`; verify it fails

## 6. Enemy generation (green)

- [x] 6.1 Rewrite `LevelGenerator::generateEnemies` to place one centered enemy on every second crystal-carrying platform outside the camera view, tracking the crystal ordinal, and drop the `MAX_ENEMIES` cap and random X
- [x] 6.2 Verify tests 5.1-5.6 pass and the existing `LevelGenerator` tree/item suites still pass
- [x] 6.3 Confirm the changed generator methods keep cyclomatic complexity at most 2

## 7. Enemy AI behavior (red)

- [x] 7.1 Add test `EnemyReversesAtRightEdge - enemy moving right reverses when the platform ends ahead`; verify it fails
- [x] 7.2 Add test `EnemyReversesAtLeftEdge - enemy moving left reverses when the platform ends ahead`; verify it fails
- [x] 7.3 Add test `EnemyApproachesPlayer - moves toward a detected player farther than 100 pixels`; verify it fails
- [x] 7.4 Add test `EnemyStopsAt100 - stops moving and fires once within 100 pixels of the player`; verify it fails
- [x] 7.5 Add test `EnemyDoesNotStepOffLedge - holds position and fires when closing on the player would step into a gap`; verify it fails
- [x] 7.6 Add test `EnemyDetection - ignores a player beyond the X radius or off the Y band`; verify it fails

## 8. Enemy AI behavior (green)

- [x] 8.1 Refactor `AI::think` into small predicates/actions (`detectsPlayer`, `desiredDirection`, `distanceToPlayer`, `patrol`, `engage`, `hasSupportAhead`) as described in design.md
- [x] 8.2 Verify tests 7.1-7.6 pass and that player-controlled `think` behavior is unchanged
- [x] 8.3 Confirm each `AI` method keeps cyclomatic complexity at most 2

## 9. Verification

- [x] 9.1 Run `make test` and verify the full suite passes with no regressions
- [x] 9.2 Run `make && ./run` and verify enemies each stand on their own platform, patrol and reverse at platform edges, and stop 100 px from the player to shoot
