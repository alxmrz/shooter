## 1. Red — tests first

- [x] 1.1 Add a failing test `RealGeneratorSkipsTreeWhenClearanceTooSmall`: build synthetic objects with a wide `GroundBlock` and another `GroundBlock` directly above it whose bottom edge is < 200px above the lower block's top, call `generateTrees`, and assert no `Tree` appears on the lower block. Verify it fails (`make test`).
- [x] 1.2 Add a failing test `RealGeneratorStretchesTreeToClearance`: build a wide `GroundBlock` with a `GroundBlock` above leaving a clearance `C >= 200`, call `generateTrees`, and assert a `Tree` exists with trunk height `≈ 2/3·C`, leaves height `≈ 1/3·C` and total height `≈ C` (tolerance 2px, never above `C`). Verify it fails.
- [x] 1.3 Add a failing test `RealGeneratorUsesMaxHeightWhenNoUpperBlock`: a wide `GroundBlock` with nothing above yields a `Tree` whose total height is 300px. Verify it fails.
- [x] 1.4 Update the existing `TreesAreGeneratedWithBranches` test (currently expects trunk height 100–200) so it matches the clearance-driven height instead of the old fixed range; verify it still fails against current code.

## 2. Green — implement

- [x] 2.1 Add public constants `TREE_MIN_CLEARANCE = 200` and `MAX_TREE_HEIGHT = 300` to `src/LevelGenerator.h`.
- [x] 2.2 Declare static helpers on `LevelGenerator`: `findCeilingBlock(blocks, block, trunkX, trunkWidth)`, `treeClearance(above, block)`, `trunkHeightForClearance(clearance)`; verify the project compiles.
- [x] 2.3 Implement the helpers in `src/LevelGenerator.cpp` per `design.md` decisions 1, 2 and 3 (nearest overlapping block above; fallback to `MAX_TREE_HEIGHT`; snap clearance down to a multiple of 3).
- [x] 2.4 Rework `LevelGenerator::generateTrees` to choose the trunk x first, skip the block when clearance < `TREE_MIN_CLEARANCE`, and set `trunkHeight` from the clearance; verify tasks 1.1–1.4 tests pass (`make test`).

## 3. Refactor and verify

- [x] 3.1 Refactor the new code so no function exceeds the project's cyclomatic-complexity limit of 2 (single loop / single branch per helper); re-run `make test`.
- [x] 3.2 Run the full `make test` suite and confirm all existing level-generation and tree tests are green.
- [x] 3.3 Build the game with `make` and confirm it links (no SFML/runtime changes); spot-check a generated level that trees no longer overlap the platform above.

## 4. Fixed 500px tree ladder and platform raising

- [x] 4.1 Add failing `Tree` tests for ladder branches: evenly spaced within jump height (<=110px), alternating sides, several per side.
- [x] 4.2 Add failing `LevelGenerator` tests: a tree is always 500px tall; an overhead ground block closer than 500px is raised until the clearance is at least 500px.
- [x] 4.3 Update `Tree` with a `branchStep` constructor and `generateLadderBranches`; keep the legacy 6-argument constructor unchanged.
- [x] 4.4 Add `branchStep` to `GeneratedObject` and a `createTree` overload in `Fabric`; keep the TMX path (`src/Map.cpp`) on the legacy call.
- [x] 4.5 Update `LevelGenerator`: `TREE_HEIGHT`, `TREE_BRANCH_STEP`, fixed trunk height, raise the nearest overhead block.
- [x] 4.6 Update the existing tree tests to the fixed-height/raise behavior and run `make test`.
- [x] 4.7 Build with `make` and spot-check a generated level for climbable 500px trees.

## 5. Per-column generator with reserved tree room

- [x] 5.1 Lower `TREE_HEIGHT` to 400 and keep `TREE_BRANCH_STEP` within jump height.
- [x] 5.2 Rewrite `generatePlatforms` bottom-up: each block gets one upward neighbour; a tree-candidate block's neighbour is placed a full `TREE_HEIGHT` away and the tree's branches bridge the gap.
- [x] 5.3 Update the reachability/spacing tests to account for tree-bridged gaps (branches are platforms).
- [x] 5.4 Add a test that every tree-candidate block gets reserved room and a tree.
- [x] 5.5 Run `make test`, update the `level-generation` spec for tree-bridged reachability, and spot-check.

## 6. Branches confined to the trunk

- [x] 6.1 Add a failing `Tree` test that no ladder branch is above the trunk top and the top branch is near it.
- [x] 6.2 Generate ladder branches from the trunk top downward so they stay on the trunk and never enter the foliage.
- [x] 6.3 Update the `level-generation/trees` ladder requirement and run `make test`.

## 7. Platform on the tree's top-branch side

- [x] 7.1 Record a topBranchSide on each tree-carrying block during platform generation and place its reserved platform on that side.
- [x] 7.2 Add a top-branch-side parameter to `Tree`/`Fabric` so the topmost ladder branch honours it.
- [x] 7.3 Add tests (Tree + LevelGenerator) that the platform and the top branch share a side; run `make test` and spot-check.
