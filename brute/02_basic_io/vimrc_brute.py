#!/usr/bin/env python3
"""Load vimrc.cpp in Vim and check each printed Hash abbreviation."""
from pathlib import Path
import hashlib
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
text = (ROOT / "codebook/1_Basic/vimrc.cpp").read_text()
lines = text.splitlines()
first = next(i for i, line in enumerate(lines) if line.startswith("ca Hash"))

with tempfile.TemporaryDirectory() as directory:
    temp = Path(directory)
    for i in range(3):
        config = temp / "vimrc"
        output = temp / "mapping"
        config.write_text("\n".join(lines[:first] +
                                    lines[first + 2*i:first + 2*i + 2]))
        result = subprocess.run(
            ["vim", "-Nu", str(config), "-n", "-i", "NONE", "-es",
             "-c", f"call writefile([execute('cabbrev Hash')], '{output}')",
             "-c", "qa!"], capture_output=True, text=True, timeout=10)
        assert result.returncode == 0, result.stdout + result.stderr
        mapping = output.read_text()
        assert "tr -d '[:space:]' | md5sum | cut -c-6" in mapping
        assert ["cpp -dD -P -fpreprocessed", "cpp -dD -P | sed",
                "g++ -E - | sed"][i] in mapping
        # Execute the actual abbreviation in Vim, including explicit ranges.
        # Errors outside the selection must not reach the preprocessor.
        samples = [
            ("", "1,$", ""),
            ("int main() { /* ignored */ return 0; }\n", "1,$",
             "intmain(){return0;}"),
            ("\tint main(){\nreturn 0; // ignored\n}\n", "1,$",
             "intmain(){return0;}"),
            ("#error outside\nint f(){return 1;}\n#error outside\n",
             "2,2", "intf(){return1;}"),
        ]
        for source, selection, normalized in samples:
            sample = temp / "sample.cpp"
            sample.write_text(source)
            result = subprocess.run(
                ["vim", "-Nu", str(config), "-n", "-i", "NONE", "-es",
                 str(sample), "-c",
                 f"call feedkeys(':{selection} Hash' . nr2char(13), 'xt')",
                 "-c", "qa!"], capture_output=True, text=True, timeout=15)
            digest = hashlib.md5(normalized.encode()).hexdigest()[:6]
            assert result.returncode == 0, result.stdout + result.stderr
            assert result.stdout.strip() == digest, (i, source, result)
            assert not result.stderr, result.stderr
print("PASS vimrc: load and execute 3 Hash pipelines on 4 edge selections")
