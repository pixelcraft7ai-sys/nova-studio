#!/usr/bin/env python3
"""Consistency checks for the product documents (workplan phase 01 acceptance).

* every feature is classified, and deferred features state a reason;
* every V1 / partial-V1 feature is covered by a requirement, and every
  requirement has a verification method and a metric (goal -> testable task);
* referenced IDs (features, metrics, open questions) exist;
* every open question has an owner; every decision has a status and a source;
* forbidden novelty/superiority claims do not appear in the product docs.
"""
import re
import sys
from pathlib import Path

ROOT = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).resolve().parents[2]
PRODUCT = ROOT / "docs" / "product"
errors = []


def read(p):
    return Path(p).read_text(encoding="utf-8")


def rows(path, prefixes):
    out = []
    for line in read(path).splitlines():
        if not line.startswith("|"):
            continue
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if cells and any(cells[0].startswith(p) for p in prefixes):
            out.append(cells)
    return out


def expand(text):
    ids = set()
    for a, b in re.findall(r"([FD]\d\d)\s*[–-]\s*([FD]?\d\d)", text):
        prefix = a[0]
        lo, hi = int(a[1:]), int(b[-2:])
        ids.update(f"{prefix}{i:02d}" for i in range(lo, hi + 1))
    ids.update(re.findall(r"\b[FD]\d\d\b", text))
    return ids


CLASSES = {"موجودة في الصناعة", "تميّز مقترح يحتاج تحققًا"}
RELEASES = {"V1", "V1 جزئي", "مؤجل"}
STATUSES = {"Accepted", "Accepted (مؤقت)", "Proposed", "Open"}

# ---- features
feats = {}
for c in rows(PRODUCT / "feature-inventory.md", ["F", "D"]):
    if not re.fullmatch(r"[FD]\d\d", c[0]):
        continue
    if len(c) != 7:
        errors.append(f"feature {c[0]}: expected 7 cells, got {len(c)}")
        continue
    fid, _, _, cls, rel, _, note = c
    feats[fid] = rel
    if cls not in CLASSES:
        errors.append(f"feature {fid}: invalid classification '{cls}'")
    if rel not in RELEASES:
        errors.append(f"feature {fid}: invalid release '{rel}'")
    if rel == "مؤجل" and not note:
        errors.append(f"feature {fid}: deferred without a reason")
if not feats:
    errors.append("no features found")

# ---- metrics / questions
metric_ids = {c[0] for c in rows(PRODUCT / "metrics.md", ["M-"])}
q_rows = rows(PRODUCT / "open-questions.md", ["Q-"])
q_ids = {c[0] for c in q_rows}
for c in q_rows:
    if len(c) != 4 or not c[2]:
        errors.append(f"question {c[0]}: missing owner")

# ---- requirements
covered = set()
seen = set()
for c in rows(PRODUCT / "requirements.md", ["FR-", "NFR-"]):
    rid = c[0]
    if rid in seen:
        errors.append(f"duplicate requirement id {rid}")
    seen.add(rid)
    if len(c) != 6:
        errors.append(f"requirement {rid}: expected 6 cells, got {len(c)}")
        continue
    _, text, fts, phase, verify, metric = c
    if not text or not verify or not metric:
        errors.append(f"requirement {rid}: missing text, verification or metric")
    for f in expand(fts):
        if f not in feats:
            errors.append(f"requirement {rid}: unknown feature {f}")
        covered.add(f)
    for m in re.findall(r"M-\d\d", metric):
        if m not in metric_ids:
            errors.append(f"requirement {rid}: unknown metric {m}")
for fid, rel in feats.items():
    if rel in ("V1", "V1 جزئي") and fid not in covered:
        errors.append(f"feature {fid} is in release 1 but has no requirement")

# ---- decisions
dec_rows = rows(ROOT / "docs" / "product-decisions.md", ["DEC-"])
if not dec_rows:
    errors.append("no decisions found")
for c in dec_rows:
    if len(c) != 4 or c[2] not in STATUSES or not c[3]:
        errors.append(f"decision {c[0]}: invalid status or missing source")

# ---- cross references to open questions
alltext = ""
for p in list(PRODUCT.glob("*.md")) + [ROOT / "docs" / "product-decisions.md", ROOT / "docs" / "risks-and-assumptions.md"]:
    alltext += read(p) + "\n"
for q in set(re.findall(r"\bQ-\d\d\b", alltext)):
    if q not in q_ids:
        errors.append(f"reference to unknown open question {q}")

# ---- forbidden claims
FORBIDDEN = ["الأول عالميًا", "الأول في العالم", "أقوى من"]
for p in PRODUCT.glob("*.md"):
    for n, line in enumerate(read(p).splitlines(), 1):
        if "claim-ok" in line:
            continue
        for phrase in FORBIDDEN:
            if phrase in line:
                errors.append(f"{p.name}:{n}: forbidden claim phrase '{phrase}'")

if errors:
    print("Product-docs check FAILED:")
    for e in errors:
        print("  -", e)
    sys.exit(1)
print(f"product docs OK: {len(feats)} features, {len(seen)} requirements, {len(q_ids)} open questions, {len(dec_rows)} decisions")
