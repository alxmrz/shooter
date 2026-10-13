#!/usr/bin/env python3
"""Enforce the AGENTS.md code rules for src/:
  * no lambdas
  * cyclomatic complexity must not exceed 2
  * functions must not exceed 50 lines

Heuristic C++ parser: strips comments and string literals, then walks brace
blocks, attributing decision points to the enclosing function.
"""

import re
import sys
import glob

MAX_COMPLEXITY = 2
MAX_FUNCTION_LINES = 50

LAMBDA_RE = re.compile(r"\[\s*[&=\w ,.\*]*\s*\]\s*\(")
# Cyclomatic complexity counts control-flow decisions. Short boolean predicates
# (`a && b`) are intentionally not counted: AGENTS.md permits small predicates
# with a few conditions when splitting them would hurt readability.
DECISION_RE = re.compile(r"\b(if|for|while|case|catch)\b|\?")
CONTROL_PREFIXES = ("if", "for", "while", "switch", "return", "else", "catch", "case")


def strip_comments_and_strings(lines):
    cleaned = []
    in_block = False
    for line in lines:
        if in_block:
            if "*/" in line:
                line = line.split("*/", 1)[1]
                in_block = False
            else:
                cleaned.append("")
                continue
        if "/*" in line:
            pre = line.split("/*", 1)[0]
            if "*/" in line.split("/*", 1)[1]:
                line = pre + line.split("*/", 1)[1]
            else:
                line = pre
                in_block = True
        line = re.sub(r"//.*", "", line)
        line = re.sub(r"'(?:\\.|[^'\\])'", "''", line)
        line = re.sub(r'"(?:\\.|[^"\\])*"', '""', line)
        cleaned.append(line)
    return cleaned


def find_functions(lines):
    """Yield (name, start_line, end_line, body_lines)."""
    functions = []
    i = 0
    n = len(lines)
    header_re = re.compile(r"^\s*[A-Za-z_].*\(.*$")
    while i < n:
        line = lines[i]
        stripped = line.strip()
        is_header_candidate = header_re.match(line) and not stripped.startswith(CONTROL_PREFIXES)
        if not is_header_candidate:
            i += 1
            continue
        # Gather signature until the parameter list closes.
        header = ""
        j = i
        while j < n and ")" not in header:
            header += lines[j] + " "
            j += 1
        if j >= n or "(" not in header:
            i += 1
            continue
        # Find the opening brace on the current or following lines.
        k = j - 1
        body_start = None
        while k < n and k <= j + 2:
            if "{" in lines[k]:
                body_start = k
                break
            k += 1
        if body_start is None:
            i += 1
            continue
        name_match = re.search(r"([A-Za-z_]\w*)\s*\([^;]*\)\s*(?:const)?\s*(?:override)?\s*$", header)
        if not name_match:
            name_match = re.search(r"([A-Za-z_]\w*)\s*\(", header)
        name = name_match.group(1) if name_match else "?"
        # Walk to matching closing brace.
        depth = 0
        started = False
        end = body_start
        for kk in range(body_start, n):
            depth += lines[kk].count("{") - lines[kk].count("}")
            if "{" in lines[kk]:
                started = True
            end = kk
            if started and depth <= 0:
                break
        functions.append((name, i + 1, end + 1, lines[i:end + 1]))
        i = end + 1
    return functions


def check_file(path):
    raw = open(path, encoding="utf-8", errors="ignore").read().split("\n")
    cleaned = strip_comments_and_strings(raw)
    problems = []

    for idx, line in enumerate(cleaned, start=1):
        if LAMBDA_RE.search(line):
            problems.append(f"{path}:{idx}: lambda is forbidden")

    for name, start, end, body in find_functions(cleaned):
        decisions = sum(len(DECISION_RE.findall(line)) for line in body)
        complexity = decisions + 1
        length = end - start + 1
        if complexity > MAX_COMPLEXITY:
            problems.append(f"{path}:{start}: {name} has cyclomatic complexity {complexity} (> {MAX_COMPLEXITY})")
        if length > MAX_FUNCTION_LINES:
            problems.append(f"{path}:{start}: {name} is {length} lines (> {MAX_FUNCTION_LINES})")
    return problems


TEST_CASE_RE = re.compile(r"^\s*(TEST|TEST_F)\s*\(")
TEST_CONTROL_RE = re.compile(r"\b(if|for|while|switch|catch)\b")


def check_test_bodies(path):
    raw = open(path, encoding="utf-8", errors="ignore").read().split("\n")
    lines = strip_comments_and_strings(raw)
    problems = []
    i = 0
    n = len(lines)
    while i < n:
        if not TEST_CASE_RE.match(lines[i]):
            i += 1
            continue
        j = i
        header = ""
        while j < n and "{" not in header:
            header += lines[j] + " "
            j += 1
        if j >= n:
            break
        depth = 0
        started = False
        body = []
        k = j
        while k < n:
            depth += lines[k].count("{") - lines[k].count("}")
            if "{" in lines[k]:
                started = True
            body.append(lines[k])
            if started and depth <= 0:
                break
            k += 1
        for offset, line in enumerate(body):
            if TEST_CONTROL_RE.search(line):
                problems.append(f"{path}:{j + offset + 1}: test body contains conditional/loop")
        i = k + 1
    return problems


def main():
    files = sorted(glob.glob("src/**/*.cpp", recursive=True) + glob.glob("src/**/*.h", recursive=True))
    problems = []
    for path in files:
        problems.extend(check_file(path))
    for path in sorted(glob.glob("tests/**/*.cpp", recursive=True)):
        problems.extend(check_test_bodies(path))
    for problem in problems:
        print(problem)
    if problems:
        print(f"\n{len(problems)} violation(s) found")
        return 1
    print("OK: no lambda, complexity or length violations")
    return 0


if __name__ == "__main__":
    sys.exit(main())
