/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef CYBER_CLASS_LOADER_UTILITY_CLASS_LOADER_UTILITY_H_
#define CYBER_CLASS_LOADER_UTILITY_CLASS_LOADER_UTILITY_H_

#include <Poco/SharedLibrary.h>
#include <cassert>
#include <cstdio>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>
#include <iostream>

#include "cyber/class_loader/utility/class_factory.h"
#include "cyber/common/log.h"

/**
 *  class register implement
 */
namespace apollo {
namespace cyber {
namespace class_loader {

class ClassLoader;

namespace utility {

using PocoLibraryPtr = std::shared_ptr<Poco::SharedLibrary>;
using ClassClassFactoryMap =
    std::map<std::string, utility::AbstractClassFactoryBase*>;
using BaseToClassFactoryMapMap = std::map<std::string, ClassClassFactoryMap>;
using LibpathPocolibVector =
    std::vector<std::pair<std::string, PocoLibraryPtr>>;
using ClassFactoryVector = std::vector<AbstractClassFactoryBase*>;

BaseToClassFactoryMapMap& GetClassFactoryMapMap();
std::recursive_mutex& GetClassFactoryMapMapMutex();
LibpathPocolibVector& GetLibPathPocoShareLibVector();
std::recursive_mutex& GetLibPathPocoShareLibMutex();
ClassClassFactoryMap& GetClassFactoryMapByBaseClass(
    const std::string& typeid_base_class_name);
std::string GetCurLoadingLibraryName();
void SetCurLoadingLibraryName(const std::string& library_name);
ClassLoader* GetCurActiveClassLoader();
void SetCurActiveClassLoader(ClassLoader* loader);
bool IsLibraryLoaded(const std::string& library_path, ClassLoader* loader);
bool IsLibraryLoadedByAnybody(const std::string& library_path);
bool LoadLibrary(const std::string& library_path, ClassLoader* loader);
void UnloadLibrary(const std::string& library_path, ClassLoader* loader);
template <typename Derived, typename Base>
void RegisterClass(const std::string& class_name,
                   const std::string& base_class_name);
template <typename Base>
Base* CreateClassObj(const std::string& class_name, ClassLoader* loader);
template <typename Base>
std::vector<std::string> GetValidClassNames(ClassLoader* loader);

//这个函数会在库加载时被触发,会将类名称和基类名称注册进来,基类名称都是ComponentBase
template <typename Derived, typename Base>
void RegisterClass(const std::string& class_name,
                   const std::string& base_class_name) {
  AINFO << "registerclass:" << class_name << "," << base_class_name << ","
        << GetCurLoadingLibraryName();

  //创建一个class factory
  utility::AbstractClassFactory<Base>* new_class_factrory_obj =
      new utility::ClassFactory<Derived, Base>(class_name, base_class_name);

  //添加classloader和库路径到classfactory,这些参数是通过解析dag文件得到的
  new_class_factrory_obj->AddOwnedClassLoader(GetCurActiveClassLoader());//设置factory关联的loader
  new_class_factrory_obj->SetRelativeLibraryPath(GetCurLoadingLibraryName());//设置类名称

  std::cout << "+++ " << GetCurLoadingLibraryName()  << std::endl;
  GetClassFactoryMapMapMutex().lock();

  //构建一个map,可以从基类索引出一个map,这个map是子类名称到子类classfactory
  ClassClassFactoryMap& factory_map =
      GetClassFactoryMapByBaseClass(typeid(Base).name());


  factory_map[class_name] = new_class_factrory_obj;
  
  GetClassFactoryMapMapMutex().unlock();
}

template <typename Base>
Base* CreateClassObj(const std::string& class_name, ClassLoader* loader) {
  GetClassFactoryMapMapMutex().lock();
  //根据基类名称找到class factory
  ClassClassFactoryMap& factoryMap =
      GetClassFactoryMapByBaseClass(typeid(Base).name());
  AbstractClassFactory<Base>* factory = nullptr;


  if (factoryMap.find(class_name) != factoryMap.end()) {
    factory = dynamic_cast<utility::AbstractClassFactory<Base>*>(factoryMap[class_name]);
  }

  GetClassFactoryMapMapMutex().unlock();

  Base* classobj = nullptr;
  if (factory && factory->IsOwnedBy(loader)) {
    classobj = factory->CreateObj();
  }

  return classobj;
}

template <typename Base>
std::vector<std::string> GetValidClassNames(ClassLoader* loader) {
  std::lock_guard<std::recursive_mutex> lck(GetClassFactoryMapMapMutex());

  ClassClassFactoryMap& factoryMap =
      GetClassFactoryMapByBaseClass(typeid(Base).name());
  std::vector<std::string> classes;
  for (auto& class_factory : factoryMap) {
    AbstractClassFactoryBase* factory = class_factory.second;
    if (factory && factory->IsOwnedBy(loader)) {
      classes.emplace_back(class_factory.first);
    }
  }

  return classes;
}

}  // namespace utility
}  // namespace class_loader
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_CLASS_LOADER_UTILITY_CLASS_LOADER_UTILITY_H_
