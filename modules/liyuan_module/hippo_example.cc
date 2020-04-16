#include "modules/liyuan_module/hippo_example.h"


bool HippoSample::Init(){
    AINFO << "HippoSample Init";
    AINFO << "HippoSample config " << config_file_path_;
    return true;
}

bool HippoSample::Proc(const std::shared_ptr<Driver>& msg0) {
    return false;
}