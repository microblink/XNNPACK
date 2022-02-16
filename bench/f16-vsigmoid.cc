// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>
#include <cmath>
#include <functional>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>
#include <fp16/fp16.h>
#include "bench/utils.h"
#include <xnnpack/AlignedAllocator.h>
#include <xnnpack/common.h>
#include <xnnpack/params-init.h>
#include <xnnpack/params.h>
#include <xnnpack/vunary.h>


static void f16_vsigmoid(
  benchmark::State& state,
  xnn_f16_vsigmoid_ukernel_function sigmoid,
  xnn_init_f16_sigmoid_params_fn init_params,
  benchmark::utils::IsaCheckFunction isa_check = nullptr)
{
  if (isa_check && !isa_check(state)) {
    return;
  }

  const size_t num_elements = state.range(0);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto f32rng = std::bind(std::uniform_real_distribution<float>(-10.0f, 10.0f), std::ref(rng));
  auto f16rng = std::bind(fp16_ieee_from_fp32_value, f32rng);

  std::vector<uint16_t, AlignedAllocator<uint16_t, 64>> x(num_elements);
  std::vector<uint16_t, AlignedAllocator<uint16_t, 64>> y(num_elements);
  std::generate(x.begin(), x.end(), std::ref(f16rng));
  std::fill(y.begin(), y.end(), UINT16_C(0x7E00) /* NaN */);

  xnn_f16_sigmoid_params params;
  init_params(&params);
  for (auto _ : state) {
    sigmoid(num_elements * sizeof(uint16_t), x.data(), y.data(), &params);
  }

  const uint64_t cpu_frequency = benchmark::utils::GetCurrentCpuFrequency();
  if (cpu_frequency != 0) {
    state.counters["cpufreq"] = cpu_frequency;
  }

  const size_t elements_per_iteration = num_elements;
  state.counters["elements"] =
    benchmark::Counter(uint64_t(state.iterations()) * elements_per_iteration, benchmark::Counter::kIsRate);

  const size_t bytes_per_iteration = 2 * num_elements * sizeof(uint16_t);
  state.counters["bytes"] =
    benchmark::Counter(uint64_t(state.iterations()) * bytes_per_iteration, benchmark::Counter::kIsRate);
}

#if XNN_ARCH_ARM64
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x8,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x8,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x16,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x16,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x24,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x24,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x32,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x32,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x40,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x40,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x48,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x48,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x56,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x56,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_div_x64,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_div_x64,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();

  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x8,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x8,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x16,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x16,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x24,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x24,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x32,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x32,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x40,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x40,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x48,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x48,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x56,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x56,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
  BENCHMARK_CAPTURE(f16_vsigmoid, neonfp16arith_rr1_p3_recpe_x64,
                    xnn_f16_vsigmoid_ukernel__neonfp16arith_rr1_p3_recpe_x64,
                    xnn_init_f16_sigmoid_neonfp16arith_rr1_p3_params,
                    benchmark::utils::CheckNEONFP16ARITH)
    ->Apply(benchmark::utils::UnaryElementwiseParameters<uint16_t, uint16_t>)
    ->UseRealTime();
#endif  // XNN_ARCH_ARM64

#ifndef XNNPACK_BENCHMARK_NO_MAIN
BENCHMARK_MAIN();
#endif
