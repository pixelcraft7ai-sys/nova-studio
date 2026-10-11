#!/usr/bin/env python3
"""Proves the product-docs checker really fails on bad input."""
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CHECK = ROOT / "tests" / "scripts" / "check_product_docs.py"


def run(root):
    return subprocess.run([sys.executable, str(CHECK), str(root)], capture_output=True, text=True).returncode


def fresh():
    tmp = Path(tempfile.mkdtemp())
    (tmp / "docs").mkdir()
    shutil.copytree(ROOT / "docs" / "product", tmp / "docs" / "product")
    for f in ("product-decisions.md", "risks-and-assumptions.md"):
        shutil.copy(ROOT / "docs" / f, tmp / "docs" / f)
    return tmp


def mutate(rel, fn):
    tmp = fresh()
    p = tmp / rel
    p.write_text(fn(p.read_text(encoding="utf-8")), encoding="utf-8")
    rc = run(tmp)
    shutil.rmtree(tmp, ignore_errors=True)
    return rc


failures = []
base = fresh()
if run(base) != 0:
    failures.append("unmodified docs should pass")
shutil.rmtree(base, ignore_errors=True)

cases = {
    "forbidden claim": ("docs/product/release-1-scope.md", lambda t: t + "\nهذه الميزة هي الأول عالميًا.\n"),
    "requirement without metric": ("docs/product/requirements.md", lambda t: t.replace("| FR-01 |", "| FR-01 |", 1).replace("(M-01)", "", 1).replace("نسبة من ينشئون مشروعًا دون مساعدة ", " ", 1)),
    "bad classification": ("docs/product/feature-inventory.md", lambda t: t.replace("| موجودة في الصناعة |", "| غير معروف |", 1)),
    "unknown question owner": ("docs/product/open-questions.md", lambda t: t.replace("| المالك + مستشار قانوني (يحدده المالك) |", "|  |", 1)),
    "bad decision status": ("docs/product-decisions.md", lambda t: t.replace("| Accepted |", "| Maybe |", 1)),
    "uncovered V1 feature": ("docs/product/requirements.md", lambda t: t.replace("F18", "F17", 1)),
}
for name, (rel, fn) in cases.items():
    if mutate(rel, fn) == 0:
        failures.append(f"checker missed: {name}")

if failures:
    print("self-test FAILED:", *failures, sep="\n  - ")
    sys.exit(1)
print("product-docs checker self-test OK")
