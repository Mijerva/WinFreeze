#!/usr/bin/env python3
"""
Guard invariant check.

NtSuspendProcess and NtResumeProcess must only appear as string literals
(i.e. in GetProcAddress calls) inside ProcessGuard.cpp.
No other file may reference these APIs by name as string literals.
"""

import sys
from pathlib import Path

GUARDED_STRINGS_UTF8  = [b'"NtSuspendProcess"', b'"NtResumeProcess"']
GUARDED_STRINGS_UTF16 = [s.decode().encode("utf-16-le") for s in GUARDED_STRINGS_UTF8]


def file_contains_guarded_symbol(path: Path) -> list:
    raw = path.read_bytes()
    hits = []
    for sym_utf8, sym_utf16 in zip(GUARDED_STRINGS_UTF8, GUARDED_STRINGS_UTF16):
        if sym_utf8 in raw or sym_utf16 in raw:
            hits.append(sym_utf8.decode())
    return hits


def main():
    repo_root = Path(__file__).parent.parent
    scan_dirs = [repo_root / "src", repo_root / "tests"]

    violations = []

    for scan_dir in scan_dirs:
        if not scan_dir.exists():
            continue
        for path in scan_dir.rglob("*"):
            if path.suffix not in (".cpp", ".h"):
                continue
            if path.name.lower() == "processguard.cpp":
                continue

            hits = file_contains_guarded_symbol(path)
            for hit in hits:
                violations.append(f"{path.relative_to(repo_root)}: contains {hit}")

    if violations:
        print("Guard invariant FAILED:")
        for v in violations:
            print(f"  {v}")
        sys.exit(1)

    print("Guard invariant passed.")
    sys.exit(0)


if __name__ == "__main__":
    main()
