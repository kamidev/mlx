// Copyright © 2023-2024 Apple Inc.

#pragma once

namespace mlx::core::detail {

std::pair<std::vector<array>, std::vector<array>> vmap_trace(
    const std::function<std::vector<array>(const std::vector<array>&)>& fun,
    const std::vector<array>& inputs,
    const std::vector<int>& in_axes);

std::vector<array> vmap_replace(
    const std::vector<array>& inputs,
    const std::vector<array>& s_inputs,
    const std::vector<array>& s_outputs,
    const std::vector<int>& in_axes,
    const std::vector<int>& out_axes);

// This is not part of the general C++ API as calling with a bad id is a bad
// idea.
std::function<std::vector<array>(const std::vector<array>&)> compile(
    const std::function<std::vector<array>(const std::vector<array>&)>& fun,
    std::uintptr_t fun_id,
    bool shapeless = false,
    std::vector<uint64_t> constants = {});

// Erase cached compile functions
void compile_erase(std::uintptr_t fun_id);

// Clear the compiler cache causing a recompilation of all compiled functions
// when called again.
void compile_clear_cache();

// Create an InTracing object during tracing operations to signify to the rest
// of the codebase that we are during tracing so evals should not throw away
// the graph.
struct InTracing {
  InTracing() {
    tracing_counter++;
  }
  ~InTracing() {
    tracing_counter--;
  }

  static bool in_tracing() {
    return tracing_counter > 0;
  }

 private:
  static int tracing_counter;
};

struct RetainGraph {
  RetainGraph() {
    tracing_counter++;
  }
  ~RetainGraph() {
    tracing_counter--;
  }

  static bool retain_graph() {
    return tracing_counter > 0;
  }

 private:
  static int tracing_counter;
};

} // namespace mlx::core::detail
