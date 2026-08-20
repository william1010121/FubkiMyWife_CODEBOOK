// Generic minimising simulated annealing.  `next_state(state, rng)` proposes
// a neighbour and `score(state)` is smaller for better states.
template<class State, class Score, class NextState>
State simulated_annealing(State current, Score score, NextState next_state,
                           mt19937_64 &rng, int iterations = 100000,
                           double temperature = 100000.0,
                           double cooling = 0.99995) {
  State best = current; double cur = score(current), best_score = cur;
  uniform_real_distribution<double> unit(0.0, 1.0);
  for (int it = 0; it < iterations; ++it) {
    State candidate = next_state(current, rng);
    double cand = score(candidate);
    if (cand < cur || exp((cur - cand) / max(temperature, 1e-15)) >= unit(rng))
      current = candidate, cur = cand;
    if (cur < best_score) best = current, best_score = cur;
    temperature *= cooling;
  }
  return best;
}
