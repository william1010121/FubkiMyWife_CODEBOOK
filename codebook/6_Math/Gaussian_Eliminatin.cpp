void GAS(V<V<double>>&vc) {
	int len = vc.size(), row = 0; for (int col = 0; col < len && row < len; ++col) {
		int idx = row; while (idx < len && fabs(vc[idx][col]) < 1e-12) ++idx; if (idx == len) continue;
		swap(vc[idx], vc[row]); double pivot = vc[row][col]; for (double &x : vc[row]) x /= pivot;
		for (int j = 0; j < len; ++j) { if (j == row || fabs(vc[j][col]) < 1e-12) continue; double mul = vc[j][col]; for (int k = 0; k < (int)vc[j].size(); ++k) vc[j][k] -= mul * vc[row][k]; }
		++row; }
};
