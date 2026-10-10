// Minimise pure score; State is copy-constructible and copy-assignable.
// next_state preserves input; finite double score/acceptance/cooling math.
// O(iterations * (proposal + scoring + state-copy cost)).
template<class State, class Score, class NextState>
State simulated_annealing(State current, Score score, NextState next_state,
  mt19937_64 &rng, int iterations = 100000,
  double temperature = 100000.0, double cooling = 0.99995) {
  State best = current;
  double cur = score(current), best_score = cur;
  uniform_real_distribution<double> unit(0.0, 1.0);
  for (int it = 0; it < iterations; ++it) {
    State candidate = next_state(current, rng);
    double cand = score(candidate);
    if (cand < cur || exp((cur - cand) / max(temperature, 1e-15))
        >= unit(rng)) current = candidate, cur = cand;
    if (cur < best_score) best = current, best_score = cur;
    temperature *= cooling;
  } return State(best); // Explicit copy also allows a deleted move ctor.
}
