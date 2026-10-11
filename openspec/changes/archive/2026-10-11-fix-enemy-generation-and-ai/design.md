## Context

See proposal.md — Why. Relevant current state:

- `Unit::move` resolves collisions with `Platform` objects, but the whole collision branch is guarded by `if (isPlayer())` (`src/objects/units/Unit.cpp:49`). Non-player units fall through.
- `Movable::move` consults `CObject::collideObjectAfterMove`, which only iterates `go->borders` (`src/CObject.cpp:155`). Generated ground blocks are added to `go->all` (as `GroundBlock`, a `Platform` subclass) and never to `borders`, so they are invisible to that path.
- `AI::think` tracks the player using ±300 px X / ±25 px Y, and `AI::move` combines movement with `isNextFalling`, which both mutates velocity and cannot see ground blocks (it also uses `collideObjectAfterMove`).
- `LevelGenerator::generateEnemies` restricts placement to every 10th platform and `MAX_ENEMIES = 10`, choosing a random X; trees are generated before enemies, and `platformHasTree` is already available.
- `AGENTS.md`: cyclomatic complexity MUST NOT exceed 2; private helpers are defined immediately after their first caller; tests use a DSL and each task item is `{test name} - {case}`.

## Goals / Non-Goals

**Goals:**
- All units (player and enemies) collide with ground blocks and stand on them.
- One enemy per every second crystal-carrying platform, chosen deterministically.
- Enemy patrols its platform, reverses at the ledge, approaches the player and stops at 100 px to fire, and never steps into a gap.

**Non-Goals:**
- Enemy-vs-enemy interaction, friendly fire, or multi-platform pathfinding/jumping AI.
- Changing bullet/enemy damage rules or item placement.
- Redesigning the border collision path beyond what enemies need.

## Decisions

### D1: Resolve platform collision for every unit, not just the player
Extract the platform-collision body of `Unit::move` into a private helper (e.g. `resolvePlatformCollision(Platform* obj, float x, float y)`) that returns whether the move was resolved, and call it for every unit. The existing top/bottom/side semantics are preserved verbatim so player behavior does not change. Ground blocks then behave as solids for enemies.

- *Alternative considered:* register ground blocks in `go->borders`. Rejected: the border path is an AABB point test with no top/bottom/side distinction, and would run in addition to the existing platform loop, duplicating and conflicting with player handling.

### D2: One solid-surface query shared by collision and ledge detection
Add a helper that answers "is the given rectangle supported / is there solid ground ahead?" by scanning `go->all` for collidable `Platform` instances. Enemy ledge detection and the enemy falling/landing check both use it, so "the platform ended" means exactly the surfaces the unit actually collides with. Keep it a separate predicate from movement so deciding to reverse never mutates the unit as a side effect (today `AI::isNextFalling` calls `setVelocity(0)`).

- *Alternative considered:* reuse `collideObjectAfterMove`. Rejected: it only sees `borders`, so it cannot detect generated platform edges (the root cause of the current ledge bug).

### D3: Deterministic enemy placement in `generateEnemies`
Iterate ground blocks in creation order, tracking a running ordinal of the platforms that carry a crystal (a platform carries a crystal when `effectiveItemMask(...) & ITEM_CRYSTAL` is set, which already excludes tree platforms). Place one enemy centered on the top edge (`y = platform.y - enemyHeight`) of every second crystal platform (ordinal 2, 4, 6, ...), skipping platforms inside the initial camera view. Remove the `MAX_ENEMIES` cap and the random X.

- *Alternative considered:* keep the random X. Rejected: the request is one enemy on its own platform; centering is deterministic, testable, and keeps the enemy away from the platform edges.
- *Alternative considered:* use `(index + 1) % 4 == 0`. Rejected: it counts tree platforms, which carry no crystal, so it would not mean "every second crystal platform" when trees are present; the ordinal over crystal platforms is the faithful reading.

### D4: Decompose AI behavior under the complexity budget
Model `AI::think` as small predicates and actions, each within complexity 2:
- `detectsPlayer()` — reuses the current ±300 px X / ±25 px Y test.
- `desiredDirection()` — sign of `player.x - unit.x`.
- `distanceToPlayer()` — absolute horizontal distance; compare against a named `ENGAGEMENT_DISTANCE = 100`.
- `hasSupportAhead(direction)` — D2 query one step ahead.
- `patrol()` — reverse the current direction when `!hasSupportAhead(direction)`.
- `engage()` — move toward the player only while distance > 100 and `hasSupportAhead(direction)`; otherwise hold position and `attack()`.

`think()` selects between `patrol()` and `engage()`, keeping a single branch. Direction changes also update the sprite/`direction` used by `Shooter::attack`.

- *Alternative considered:* enlarge the existing if/else chain. Rejected: it violates the complexity budget and mixes concerns.

### D5: Tests through a DSL
Extend `tests/LevelGenerator_test.cpp` and add AI/collision tests. Shared builders (e.g. a fixture that assembles `GameObjects` with platforms, a player, and an enemy) live in helpers so each test states only the case. Register every new test file in `tests/CMakeLists.txt`. Tests run with `make test` (CMake/gtest). Naming follows `{test name} - {case}`.

## Risks / Trade-offs

- [Generalizing `Unit::move` changes the player path] → Keep the existing platform-resolution semantics intact; rely on the current collision test suites plus new enemy tests as regression guards.
- [Ledge detection and collision could disagree] → Both use the same D2 query, so they cannot diverge.
- [Complexity budget forces heavy decomposition] → Prefer many tiny private helpers defined right after their caller, as the existing `LevelGenerator` does.
- [Enemy may visually overlap a generated item on its platform] → Acceptable; items are non-solid and the request does not restrict them. Noted for later if needed.
- [Enemy stuck between the 100 px stop distance and a ledge] → Covered by the spec: hold position and fire rather than stepping off.

## Migration Plan

No data or API migration. Behavior-only change; existing levels regenerate at startup. Rollback is reverting the source edits.

## Open Questions

None.
