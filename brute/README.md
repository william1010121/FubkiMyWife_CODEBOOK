# Template brute-force validation

Every source artifact under `codebook/` (excluding examples and documentation)
is referenced by an executable test under `brute/`; this includes all 104 active
main code listings plus the NTT prime/root table in `codebook/content.tex`.
Tests include or wrap the source,
generate bounded random/exhaustive cases, and compare it with a deliberately
simple oracle.  The Java harness performs a source contract when `javac` is not
installed and automatically upgrades to a compiled differential test when it is.

Run the complete local suite with:

```sh
rbox bash -lc 'JOBS=4 ./brute/run_all.sh'
```

The runner defaults to two processes and is hard-capped at four.  `JOBS` may
lower the limit, but values above four are clamped to four; every child also
gets one thread. `TIMEOUT_SECONDS` defaults to 60 per harness. A non-zero exit
status means the corresponding template is not verified.

Documentation-only material is listed explicitly in the relevant manifest.
`audit_coverage.py`, which runs before the suite, makes both active and all-source
coverage hard failures so a new template cannot silently omit its brute. It also
requires every active `\inputcodeex` to name one existing, unique example;
unlisted files left in `codebook/examples/` are ignored.
