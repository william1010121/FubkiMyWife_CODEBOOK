#!/usr/bin/env python3
"""Serial verification entrypoint for the geometry-else partition."""
from pathlib import Path
import csv
import os
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
MANIFEST = HERE / "manifest.tsv"

def read_manifest():
    with MANIFEST.open(newline="") as f:
        rows = list(csv.DictReader(f, delimiter="\t"))
    expected = {p.relative_to(ROOT).as_posix() for p in (ROOT / "codebook/8_Geometry").glob("*") if p.is_file()}
    expected |= {p.relative_to(ROOT).as_posix() for p in (ROOT / "codebook/9_Else").glob("*") if p.is_file()}
    got = {r["path"] for r in rows}
    missing, extra = sorted(expected - got), sorted(got - expected)
    if missing or extra:
        raise SystemExit(f"manifest coverage mismatch: missing={missing}, extra={extra}")
    return rows

def run_one(name, source, binary):
    env = os.environ.copy()
    env.update({"OMP_NUM_THREADS":"1", "OPENBLAS_NUM_THREADS":"1", "MKL_NUM_THREADS":"1"})
    compile_cmd = ["g++", "-std=c++20", "-O0", "-pipe", str(source), "-o", str(binary)]
    c = subprocess.run(compile_cmd, cwd=ROOT, env=env, text=True, capture_output=True)
    if c.returncode:
        print(f"{name}: COMPILE_FAIL\n{c.stderr}")
        return False
    p = subprocess.run([str(binary)], cwd=ROOT, env=env, text=True, capture_output=True)
    if p.returncode:
        print(f"{name}: RUN_FAIL\n{p.stdout}{p.stderr}")
        return False
    print(f"{name}: PASS {p.stdout.strip()}")
    return True

def main():
    rows = read_manifest()
    with tempfile.TemporaryDirectory(prefix="geometry-else-") as td:
        td = Path(td)
        ok = run_one("geometry_harness", HERE / "geometry_harness.cpp", td / "geometry_harness")
        ok &= run_one("else_harness", HERE / "else_harness.cpp", td / "else_harness")
    runnable = [r["path"] for r in rows if r["harness"] != "not-runnable"]
    blocked = [r["path"] for r in rows if r["harness"] == "not-runnable"]
    print(f"manifest: {len(rows)} files; exercised={len(runnable)}; documented-not-runnable={len(blocked)}")
    print("execution: serial, one compiler process at a time, no worker threads")
    return 0 if ok else 1

if __name__ == "__main__":
    sys.exit(main())
