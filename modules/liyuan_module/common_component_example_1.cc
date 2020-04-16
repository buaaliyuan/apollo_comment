#include "modules/liyuan_module/common_component_example_1.h"
#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"

bool CommonComponentSample::Init() {
  std::cout << "++++++ Init" << std::endl;
  return true;
}

bool CommonComponentSample::Proc(const std::shared_ptr<Driver>& msg0,
                               const std::shared_ptr<Driver>& msg1) {
  AINFO << "Start common component Proc [" << msg0->msg_id() << "] ["
        << msg1->msg_id() << "]";
  return true;
}