ModArithmetic mod(12); // composite moduli also work
mod.norm(-5);          // 7
mod.mul(-5, 5);        // 11
mod.pow(5, 3);         // 5
long long inv = mod.inv(5); // 5; inv(6) = -1
if (inv != -1) mod.mul(7, inv); // 7 / 5 mod 12 = 11
// Never divide by a non-invertible residue.
