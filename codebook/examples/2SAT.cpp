SAT sat(n); // var i true=i, false=i+n
sat.add_clause(a, b); // (a or b)
if (sat.solve()) use sat.istrue[i];
