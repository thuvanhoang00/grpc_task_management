#include <benchmark/benchmark.h>
#include "../include/admin.h"
#include "../include/user.h"
static void benchmark_test(benchmark::State& s)
{
    for(auto _ : s)
    {
        thu::Admin adm("thu");
        adm.addUser(std::make_shared<thu::User>("a", 1));
    }
}
BENCHMARK(benchmark_test);
BENCHMARK_MAIN();