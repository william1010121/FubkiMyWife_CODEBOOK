#!/usr/bin/env python3
from __future__ import annotations

from pathlib import Path
import re
import shutil
import stat
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[2]
SHELL_COMPILE = ROOT / "codebook/1_Basic/Shell_script.cpp"
HASH_SCRIPT = ROOT / "codebook/9_Else/hash.sh"
STRESS_SCRIPT = ROOT / "codebook/9_Else/Stress_Test.sh"
JAVA_TEMPLATE = ROOT / "codebook/10_JAVA/Big_number.cpp"


def run(command: list[str], **kwargs: object) -> subprocess.CompletedProcess[str]:
    return subprocess.run(command, text=True, capture_output=True, check=False, **kwargs)


def make_executable(path: Path, source: str) -> None:
    path.write_text(source)
    path.chmod(path.stat().st_mode | stat.S_IXUSR)


def test_compile_script(work: Path) -> None:
    stem = work / "smoke"
    stem.with_suffix(".cpp").write_text(
        "#include <iostream>\nint main(){std::cout << 42 << '\\n';}\n"
    )
    result = run(["bash", str(SHELL_COMPILE), str(stem)])
    assert result.returncode == 0, result.stderr
    executed = run([str(stem)])
    assert executed.returncode == 0 and executed.stdout == "42\n"


def test_hash_script(work: Path) -> None:
    a = work / "a.cpp"
    b = work / "b.cpp"
    c = work / "c.cpp"
    a.write_text("int add(int x,int y){return x+y;}\n")
    b.write_text("// same tokens\n int add ( int x, int y ) { return x + y ; }\n")
    c.write_text("int add(int x,int y){return x-y;}\n")
    hashes = [run(["bash", str(HASH_SCRIPT), str(path)]) for path in (a, b, c)]
    assert all(result.returncode == 0 for result in hashes)
    values = [result.stdout.strip() for result in hashes]
    assert re.fullmatch(r"[0-9a-f]{6}", values[0])
    assert values[0] == values[1] and values[0] != values[2]


def test_stress_script(work: Path) -> None:
    generator = work / "generator"
    brute = work / "brute"
    solution = work / "solution"
    wrong = work / "wrong"
    make_executable(generator, "#!/usr/bin/env bash\nprintf '7 9\\n'\n")
    make_executable(brute, "#!/usr/bin/env bash\nread -r a b\necho $((a+b))\n")
    make_executable(solution, "#!/usr/bin/env bash\nread -r a b\necho $((a+b))\n")
    make_executable(wrong, "#!/usr/bin/env bash\necho 0\n")
    passed = run(
        ["bash", str(STRESS_SCRIPT), str(generator), str(brute), str(solution), "5"],
        cwd=work,
    )
    assert passed.returncode == 0, passed.stderr
    failed = run(
        ["bash", str(STRESS_SCRIPT), str(generator), str(brute), str(wrong), "1"],
        cwd=work,
    )
    assert failed.returncode == 1
    assert (work / "stress-failure.txt").read_text() == "7 9\n"


def java_divide(a: int, b: int) -> int:
    magnitude = abs(a) // abs(b)
    return -magnitude if (a < 0) != (b < 0) else magnitude


def test_java_template(work: Path) -> str:
    source = JAVA_TEMPLATE.read_text()
    required = {
        "+": "ia.add(ic)",
        "-": "ia.subtract(ic)",
        "*": "ia.multiply(ic)",
        "/": "ia.divide(ic)",
    }
    assert "import java.math.BigInteger;" in source
    assert "while(cin.hasNext())" in source
    for operator, expression in required.items():
        assert f"b.charAt(0) == '{operator}'" in source and expression in source

    javac = shutil.which("javac")
    java = shutil.which("java")
    if not javac or not java:
        return "source contract (javac unavailable in rbox)"
    java_source = work / "JAVA.java"
    java_source.write_text(source)
    compiled = run([javac, str(java_source)], cwd=work)
    assert compiled.returncode == 0, compiled.stderr
    cases = [
        (10**80 + 123, "+", -(10**75 + 9)),
        (-(10**70 + 5), "-", 10**65 + 7),
        (10**45 + 11, "*", -(10**35 + 3)),
        (-(10**90 + 99), "/", 10**40 + 13),
    ]
    input_text = "".join(f"{a} {operator} {b}\n" for a, operator, b in cases)
    expected = []
    for a, operator, b in cases:
        expected.append(
            str(
                a + b if operator == "+" else
                a - b if operator == "-" else
                a * b if operator == "*" else
                java_divide(a, b)
            )
        )
    executed = subprocess.run(
        [java, "-cp", str(work), "JAVA"],
        input=input_text,
        text=True,
        capture_output=True,
        check=False,
    )
    assert executed.returncode == 0, executed.stderr
    assert executed.stdout.splitlines() == expected
    return "compiled differential test"


def main() -> None:
    with tempfile.TemporaryDirectory(prefix="codebook-language-tools-") as directory:
        work = Path(directory)
        test_compile_script(work)
        test_hash_script(work)
        test_stress_script(work)
        java_status = test_java_template(work)
    print(f"language tools PASS; Java: {java_status}")


if __name__ == "__main__":
    main()
