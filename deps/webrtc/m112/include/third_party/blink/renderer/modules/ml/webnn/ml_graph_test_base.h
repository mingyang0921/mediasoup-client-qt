// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_ML_WEBNN_ML_GRAPH_TEST_BASE_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_ML_WEBNN_ML_GRAPH_TEST_BASE_H_

#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/blink/renderer/bindings/modules/v8/v8_ml_compute_result.h"
#include "third_party/blink/renderer/core/dom/dom_exception.h"
#include "third_party/blink/renderer/modules/ml/webnn/ml_graph.h"
#include "third_party/blink/renderer/modules/ml/webnn/ml_graph_builder_test.h"
#include "third_party/blink/renderer/modules/ml/webnn/ml_graph_builder_utils.h"
#include "third_party/blink/renderer/modules/ml/webnn/ml_operand.h"
#include "third_party/blink/renderer/platform/heap/persistent.h"

namespace blink {

class MLGraphBuilder;
class V8TestingScope;

// The utility methods for graph test.
enum ExecutionMode { kAsync, kSync };

std::string ExecutionModeParamToString(
    const ::testing::TestParamInfo<ExecutionMode>& execution_mode);

class MLGraphTestBase : public ::testing::Test,
                        public ::testing::WithParamInterface<ExecutionMode> {
 public:
  // BuildResult is returned by Build() method. Only one member of BuildResult
  // is valid. If the graph building is successful, graph points to the MLGraph
  // and exception is a nullptr. Otherwise, exception points to the DOMException
  // and graph is a nullptr.
  struct BuildResult {
    Persistent<MLGraph> graph;
    Persistent<DOMException> exception;
  };

  // Helper method for testing both BuildAsyncImpl() and BuildSyncImpl() with
  // the same named operands and expected results.
  BuildResult BuildGraph(V8TestingScope& scope,
                         MLGraphBuilder* builder,
                         const MLNamedOperands& named_operands);

  // Helper method for testing both ComputeAsync() and ComputeSync() with the
  // same input/output buffers and expected results. If the graph computes
  // successfully, it returns nullptr and the results are produced into the
  // output buffers. Otherwise, it returns the pointer to the DOMException
  // thrown by the graph computing.
  DOMException* ComputeGraph(V8TestingScope& scope,
                             MLGraph* graph,
                             MLNamedArrayBufferViews& inputs,
                             MLNamedArrayBufferViews& outputs);

  // Test operations with different parameters such as tensor dimensions, data
  // layout. Each test case will builds a graph and computes it with input data
  // to check the expected value.
  void TestElementWiseBinary(V8TestingScope& scope);
};

template <typename T>
struct OperandInfo {
  V8MLOperandType::Enum type;
  Vector<uint32_t> dimensions;
  Vector<T> values;
};

// Helper function to set the data of an ArrayBufferView from a vector.
template <typename T>
void SetArrayBufferViewValues(NotShared<DOMArrayBufferView> array_buffer_view,
                              const Vector<T>& values) {
  DCHECK_EQ(array_buffer_view->byteLength(), values.size() * sizeof(T));
  memcpy(array_buffer_view->BaseAddress(), values.data(),
         values.size() * sizeof(T));
}

// Overrode helper function to create an ArrayBufferView given an operand and
// set its data from a vector.
template <typename T>
NotShared<DOMArrayBufferView> CreateArrayBufferViewForOperand(
    const MLOperand* operand,
    const Vector<T>& values) {
  auto array_buffer_view = CreateArrayBufferViewForOperand(operand);
  SetArrayBufferViewValues(array_buffer_view, values);
  return array_buffer_view;
}

// Helper function to get the data of an ArrayBufferView into a vector.
template <typename T>
Vector<T> GetArrayBufferViewValues(
    NotShared<DOMArrayBufferView> array_buffer_view) {
  Vector<T> values(base::checked_cast<wtf_size_t>(
      array_buffer_view->byteLength() / array_buffer_view->TypeSize()));
  memcpy(values.data(), array_buffer_view->BaseAddress(),
         array_buffer_view->byteLength());
  return values;
}

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_ML_WEBNN_ML_GRAPH_TEST_BASE_H_
