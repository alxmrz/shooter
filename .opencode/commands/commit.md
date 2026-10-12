---
description: "Create a Conventional Commit from the current changes"
---

Create a single Conventional Commit from the current working tree changes.

**Provided arguments**: $ARGUMENTS

**Steps**

1. Inspect the current state before doing anything:
   ```bash
   git status --short
   git diff --staged
   git diff
   git log --oneline -10
   ```

2. Stage the intended changes. If files are untracked or modified but not staged, `git add` them (the project rule is that created/edited/deleted files must be staged). Never stage secrets, build artifacts, or unrelated files. If `$ARGUMENTS` names specific paths, stage only those.

3. Analyze the staged diff to determine:
   - the **type**: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, or `revert`
   - the **scope**: the affected area (e.g. `enemy`, `level-generation`, `trees`, `ui`), when it adds clarity
   - the **description**: imperative, lowercase, no trailing period, <= 72 chars

   Match the repository's existing commit style from `git log`. If the diff contains several unrelated concerns, prefer the dominant one; do not invent a scope.

4. Commit with a Conventional Commit message. Use only a header unless the change genuinely needs a body explaining **why**:
   ```bash
   git commit -m "<type>(<scope>): <description>"
   ```

5. Show the resulting commit:
   ```bash
   git log --oneline -1
   ```

Do not push. Do not amend existing commits. If there is nothing to commit, say so and stop.
