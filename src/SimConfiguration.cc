#include "SimConfiguration.hh"
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>

const GeneratorConfiguration SimConfiguration::defaultGen = { 
  0, //delta
  0, //x0
  0  //y0
};

const DetectorConfiguration SimConfiguration::defaultDet = { 
  45, //nPlanes
  0.18 //fieldIndex
};
  
  
SimConfiguration::SimConfiguration():
  gen(defaultGen),
  det(defaultDet)
{
}

template<typename T>
void updateFromConfig(boost::optional<T> opt, T& oldValue){
  if(opt){
    oldValue = *opt;
  }
}

SimConfiguration::SimConfiguration(std::string conf):
  SimConfiguration()
{
  confFile_ = conf;
  readConfig();
}
 
void SimConfiguration::update(){
  if(confFile_.size() != 0){
    readConfig();
  }
}

void SimConfiguration::readConfig(){
  boost::property_tree::ptree fullTree;
  read_json(confFile_, fullTree);

  //read generator tree
  auto genTreeOpt = fullTree.get_child_optional("generator");
  if(genTreeOpt){
    auto genTree = *genTreeOpt;

    auto optDelta = genTree.get_optional<double>("delta");
    updateFromConfig(optDelta, gen.delta);
    auto optx0 = genTree.get_optional<double>("x0");
    updateFromConfig(optx0, gen.x0);
    auto opty0 = genTree.get_optional<double>("y0");
    updateFromConfig(opty0, gen.y0);
  }

  //read detector tree
  auto detTreeOpt = fullTree.get_child_optional("detector");
  if(detTreeOpt){
    auto detTree = *detTreeOpt;

    auto optNPlanes = detTree.get_optional<int>("nPlanes");
    updateFromConfig(optNPlanes, det.nPlanes);
    auto optFieldIndex= detTree.get_optional<double>("fieldIndex");
    updateFromConfig(optFieldIndex, det.fieldIndex);
  }
}
