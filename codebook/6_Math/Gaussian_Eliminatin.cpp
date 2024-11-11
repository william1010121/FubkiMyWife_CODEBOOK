void GAS(V<V<double>>&vc) {
    int len = vc.size();
    for( int i = 0; i < len; ++i ) {
        int idx = find_if(vc.begin()+i,vc.end(),[&](auto&v) {return v[i] != 0;} ) - vc.begin();
        if( idx == len ) continue;
        if( i != idx ) swap( vc[idx], vc[i] );
        double pivot = vc[i][i];
        for_each( vc[i].begin(), vc[i].end(), [&]( auto &a ) { a/=pivot; } );
        for( int j = 0; j < len; ++j ) {
            if( i == j ) continue;
            if( vc[j][i] != 0 ) {
                double mul = vc[j][i]/vc[i][i];
                transform( vc[j].begin(), vc[j].end(), vc[i].begin(), vc[j].begin(),
                        [&](auto &a, auto &b ) {
                        return a-b*mul;
                        });
            }
        }
    }
};
