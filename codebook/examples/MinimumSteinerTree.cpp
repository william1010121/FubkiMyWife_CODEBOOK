static SteinerTree st; st.init(n); st.vcst[i] = nodeCost;
st.add_edge(u, v, w); // DIRECTED; 0 <= edge/vertex costs < INF
int c = st.solve(terminals); // once/init; do not pre-call shortest_path
// c == INF for disconnection or cost >= INF; empty terminals give 0.
