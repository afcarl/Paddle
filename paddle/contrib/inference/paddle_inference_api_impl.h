/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#pragma once

#include <glog/logging.h>
#include <memory>
#include <string>
#include <vector>

#include "paddle/contrib/inference/paddle_inference_api.h"

#include "paddle/fluid/framework/ddim.h"
#include "paddle/fluid/framework/init.h"
#include "paddle/fluid/framework/lod_tensor.h"
#include "paddle/fluid/inference/io.h"
#include "paddle/fluid/platform/profiler.h"

namespace paddle {

struct ConfigImpl : public PaddlePredictor::Config {
  int device;
  float fraction_of_gpu_memory;
  std::string prog_file;
  std::string param_file;
  bool share_variables;
};

class PaddlePredictorImpl : public PaddlePredictor {
 public:
  explicit PaddlePredictorImpl(const ConfigImpl &config) : config_(config) {}

  bool Init();

  bool Run(const std::vector<PaddleTensor> &inputs,
           std::vector<PaddleTensor> *output_data) override;

  std::unique_ptr<PaddlePredictor> Clone() override;

  ~PaddlePredictorImpl() override{};

 private:
  bool InitShared() override;
  bool SetFeed(const std::vector<PaddleTensor> &input_datas,
               std::vector<framework::LoDTensor> *feeds);
  bool GetFetch(const std::vector<framework::LoDTensor> &fetchs,
                std::vector<PaddleTensor> *output_data);

  ConfigImpl config_;
  platform::Place place_;
  std::unique_ptr<framework::Executor> executor_;
  std::unique_ptr<framework::Scope> scope_;
  std::unique_ptr<framework::ExecutorPrepareContext> ctx_;
  std::unique_ptr<framework::ProgramDesc> inference_program_;
  std::vector<std::string> feed_target_names_;
  std::vector<std::string> fetch_target_names_;
};

}  // namespace paddle
