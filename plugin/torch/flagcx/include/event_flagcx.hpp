/*************************************************************************
 * Copyright (c) 2025 by MetaX Integrated Circuits (Shanghai) Co., Ltd. All
 *Rights Reserved. Copyright (c) 2025 by DU. All Rights Reserved.
 ************************************************************************/
#pragma once

#include "flagcx.h"

#ifdef USE_NVIDIA_ADAPTOR
#include <ATen/cuda/CUDAEvent.h>
#include <cuda_runtime.h>
#elif USE_ASCEND_ADAPTOR
#include "torch_npu/csrc/core/npu/NPUStream.h"
#include "torch_npu/csrc/core/npu/NPUEvent.h"
#elif USE_ILUVATAR_COREX_ADAPTOR
#include <ATen/cuda/CUDAEvent.h>
#include <cuda_runtime.h>
#elif USE_CAMBRICON_ADAPTOR
#include "framework/core/MLUEvent.h"
#include "framework/core/MLUStream.h"
#elif USE_METAX_ADAPTOR
#include <ATen/cuda/CUDAEvent.h>
#include <cuda_runtime.h>
#elif USE_MUSA_ADAPTOR
#include <torch_musa/csrc/core/MUSAEvent.h>
#include <musa_runtime.h>
#elif USE_DU_ADAPTOR
#include <ATen/cuda/CUDAEvent.h>
#include <cuda_runtime.h>
#elif USE_KUNLUNXIN_ADAPTOR
#include <ATen/cuda/CUDAEvent.h>
#include <cuda_runtime.h>
#endif

namespace c10d {

class flagcxEvent {
public:
  virtual ~flagcxEvent() = default;

  virtual void record(const int deviceId) = 0;
  virtual void record(const flagcxStream_t &stream, const int deviceId) = 0;

  virtual void block(const int deviceId) = 0;
  virtual void block(const flagcxStream_t &stream, const int deviceId) = 0;
};

#ifdef USE_NVIDIA_ADAPTOR
class flagcxCudaEvent : public flagcxEvent {
public:
  flagcxCudaEvent() {
    cudaEvent_ = at::cuda::CUDAEvent(cudaEventDisableTiming);
  }

  void record(const int deviceId) override {
    cudaEvent_.record(at::cuda::getCurrentCUDAStream(deviceId));
  }

  void record(const flagcxStream_t &stream, const int deviceId) override {
    cudaEvent_.record(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, deviceId));
  }

  void block(const int deviceId) override {
    cudaEvent_.block(at::cuda::getCurrentCUDAStream(deviceId));
  }

  void block(const flagcxStream_t &stream, const int deviceId) override {
    cudaEvent_.block(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, deviceId));
  }

private:
  at::cuda::CUDAEvent cudaEvent_;
};
#elif USE_ILUVATAR_COREX_ADAPTOR
class flagcxIxcudaEvent : public flagcxEvent {
public:
  flagcxIxcudaEvent() {
    ixcuda_event = at::cuda::CUDAEvent(cudaEventDisableTiming);
  }

  void record(const int device_id) override {
    ixcuda_event.record(at::cuda::getCurrentCUDAStream(device_id));
  }

  void record(const flagcxStream_t &stream, const int device_id) override {
    ixcuda_event.record(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, device_id));
  }

  void block(const int device_id) override {
    ixcuda_event.block(at::cuda::getCurrentCUDAStream(device_id));
  }

  void block(const flagcxStream_t &stream, const int device_id) override {
    ixcuda_event.block(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, device_id));
  }

private:
  at::cuda::CUDAEvent ixcuda_event;
};
#elif USE_ASCEND_ADAPTOR
class flagcxCannEvent : public flagcxEvent {
public:
  flagcxCannEvent() { npu_event = c10_npu::NPUEvent(); }

  void record(const int device_id) override {
    npu_event.record(c10_npu::getCurrentNPUStream(device_id));
  }

  void record(const flagcxStream_t &stream, const int device_id) override {
    npu_event.record(c10_npu::getNPUStreamFromPool(device_id));
  }

  void block(const int device_id) override {
    npu_event.block(c10_npu::getCurrentNPUStream(device_id));
  }

  void block(const flagcxStream_t &stream, const int device_id) override {
    npu_event.block(c10_npu::getNPUStreamFromPool(device_id));
  }

private:
  c10_npu::NPUEvent npu_event;
};
#elif USE_CAMBRICON_ADAPTOR
class flagcxMluEvent : public flagcxEvent {
public:
  flagcxMluEvent() { mlu_event = torch_mlu::MLUEvent(); }

  void record(const int device_id) override {
    mlu_event.place(torch_mlu::getCurrentMLUStream(device_id));
  }

  void record(const flagcxStream_t &stream, const int device_id) override {
    mlu_event.place(
        torch_mlu::getStreamFromExternal(*(cnrtQueue_t *)stream, device_id));
  }

  void block(const int device_id) override {
    mlu_event.wait(torch_mlu::getCurrentMLUStream(device_id));
  }

  void block(const flagcxStream_t &stream, const int device_id) override {
    mlu_event.wait(
        torch_mlu::getStreamFromExternal(*(cnrtQueue_t *)stream, device_id));
  }

private:
  torch_mlu::MLUEvent mlu_event;
};
#elif USE_METAX_ADAPTOR
class flagcxMacaEvent : public flagcxEvent {
public:
  flagcxMacaEvent() {
    maca_event = at::cuda::CUDAEvent(cudaEventDisableTiming);
  }

  void record(const int device_id) override {
    maca_event.record(at::cuda::getCurrentCUDAStream(device_id));
  }

  void record(const flagcxStream_t &stream, const int device_id) override {
    maca_event.record(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, device_id));
  }

  void block(const int device_id) override {
    maca_event.block(at::cuda::getCurrentCUDAStream(device_id));
  }

  void block(const flagcxStream_t &stream, const int device_id) override {
    maca_event.block(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, device_id));
  }

private:
  at::cuda::CUDAEvent maca_event;
};
#elif USE_MUSA_ADAPTOR
class flagcxMusaEvent : public flagcxEvent {
public:
  flagcxMusaEvent() {
    musa_event = at::musa::MUSAEvent(musaEventDisableTiming);
  }

  void record(const int device_id) override {
    musa_event.record(at::musa::getCurrentMUSAStream(device_id));
  }

  void record(const flagcxStream_t &stream, const int device_id) override {
    musa_event.record(
        at::musa::getStreamFromExternal(*(musaStream_t *)stream, device_id));
  }

  void block(const int device_id) override {
    musa_event.block(at::musa::getCurrentMUSAStream(device_id));
  }

  void block(const flagcxStream_t &stream, const int device_id) override {
    musa_event.block(
        at::musa::getStreamFromExternal(*(musaStream_t *)stream, device_id));
  }

private:
  at::musa::MUSAEvent musa_event;
};
#elif USE_DU_ADAPTOR
class flagcxDuEvent : public flagcxEvent {
public:
  flagcxDuEvent() { cudaEvent_ = at::cuda::CUDAEvent(cudaEventDisableTiming); }

  void record(const int deviceId) override {
    cudaEvent_.record(at::cuda::getCurrentCUDAStream(deviceId));
  }

  void record(const flagcxStream_t &stream, const int deviceId) override {
    cudaEvent_.record(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, deviceId));
  }

  void block(const int deviceId) override {
    cudaEvent_.block(at::cuda::getCurrentCUDAStream(deviceId));
  }

  void block(const flagcxStream_t &stream, const int deviceId) override {
    cudaEvent_.block(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, deviceId));
  }

private:
  at::cuda::CUDAEvent cudaEvent_;
};
#elif USE_KUNLUNXIN_ADAPTOR
class flagcxXpuEvent : public flagcxEvent {
public:
  flagcxXpuEvent() { cudaEvent_ = at::cuda::CUDAEvent(cudaEventDisableTiming); }

  void record(const int deviceId) override {
    cudaEvent_.record(at::cuda::getCurrentCUDAStream(deviceId));
  }

  void record(const flagcxStream_t &stream, const int deviceId) override {
    cudaEvent_.record(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, deviceId));
  }

  void block(const int deviceId) override {
    cudaEvent_.block(at::cuda::getCurrentCUDAStream(deviceId));
  }

  void block(const flagcxStream_t &stream, const int deviceId) override {
    cudaEvent_.block(
        at::cuda::getStreamFromExternal(*(cudaStream_t *)stream, deviceId));
  }

private:
  at::cuda::CUDAEvent cudaEvent_;
};
#endif

} // namespace c10d
