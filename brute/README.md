# Template brute-force validation

Each template has its own test file somewhere under `brute/`.  A test is a
small executable: it includes or wraps one codebook template, generates bounded
random/exhaustive cases, and compares the template against a deliberately
simple oracle.  Problem references and the exact covered contract are recorded
in `brute/manifest.tsv`.

Run the complete local suite with:

```sh
./brute/run_all.sh
```

The runner is deliberately capped at four processes.  `JOBS` may lower the
limit, but values above four are clamped to four; every child also gets one
thread.  A non-zero exit status means the corresponding template is not
verified.

Documentation-only material is listed explicitly in the relevant manifest.
Every active callable code entry in `content.tex` has a passing executable
harness; the suite also keeps extra inactive helpers under test when they are
useful regression coverage.
