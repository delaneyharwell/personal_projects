#include "stats.h"
#include <cmath>
#include <numeric>
#include <algorithm>

double compute_mean(const std::vector<int> &v){
    double sum = 0;
    for (auto &n: v)
        sum += n;
    return sum / v.size();
}

double compute_stddev(const std::vector<int> &v){
    double sum_squares = 0;
    double mean = compute_mean(v);
    double len = v.size();
    for (auto n : v){
        double diff = n - mean;
        sum_squares += diff*diff;
    }
    return std::sqrt(sum_squares / len);
}
std::map<int,int> make_histogram(const std::vector<int> &v){
    std::map<int,int> histogram;
    for( size_t i = 0;  i < v.size(); ++i)
        ++histogram[v[i]];
    return histogram;
}
Stats::Stats(string nm, const vector<int>& cl):
    name(nm),
    entries(std::accumulate(cl.begin(), cl.end(), 0)),
    chains(cl.size()),
    load_factor((double)entries / (double)chains),
    min(*std::min_element(cl.begin(), cl.end())),
    max(*std::max_element(cl.begin(), cl.end())),
    span(max-min),
    mean(compute_mean(cl)),
    stddev(compute_stddev(cl)),
    histogram(make_histogram(cl)),
    chain_lengths(cl)
    {
        
    }