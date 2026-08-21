mt19937_64 rng(7122);
int best=simulated_annealing(0,[](int x){return abs(x-3);},
  [](int x,auto&){return x<3?x+1:x-1;},rng); // best=3
