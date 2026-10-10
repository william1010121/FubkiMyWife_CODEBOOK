#!/usr/bin/env python3
"""Load vimrc.cpp in Vim and check each printed Hash abbreviation."""
from pathlib import Path
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
print("PASS vimrc: clean Vim load and all three complete Hash pipelines")
