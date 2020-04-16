#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"
#include "modules/liyuan_module/proto/test.pb.h"

using apollo::cyber::Component;
using apollo::cyber::ComponentBase;
using apollo::modules::liyuan_module::Driver;

class HippoSample : public Component<Driver> {
 public:
  bool Init() override;
  bool Proc(const std::shared_ptr<Driver>& msg0) override;
};

CYBER_REGISTER_COMPONENT(HippoSample)