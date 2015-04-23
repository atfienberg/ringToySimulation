#include "SimConfiguration.hh"
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>

//default values
const CalorimeterConfiguration SimConfiguration::defaultCalo = { 
  6, //nrows
  9, //ncolumns
  25, //width
  25, //height
  140 }; //length
const GeneratorConfiguration SimConfiguration::defaultGen = { 
  "e+", //particle type
  3000, //energy 				     
  0,    //impact x
  0,    //impact y
  0,    //impactAngle
  0 }; //impactPhi
const PhysicsConfiguration SimConfiguration::defaultPhys = { 
  false //em extra physics
};

SimConfiguration::SimConfiguration():
  calo(defaultCalo),
  genVector(0),
  phys(defaultPhys)
{
  genVector.push_back(defaultGen);
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
  
  //read calo tree
  auto calTreeOpt = fullTree.get_child_optional("calorimeter");
  if(calTreeOpt){
    auto calTree = *calTreeOpt;

    auto optNRows = calTree.get_optional<int>("nRows");
    updateFromConfig(optNRows, calo.nRows);
    auto optNCols = calTree.get_optional<int>("nColumns");
    updateFromConfig(optNCols, calo.nCols);
    auto optWidth = calTree.get_optional<double>("width");
    updateFromConfig(optWidth, calo.width);
    auto optHeight = calTree.get_optional<double>("height");
    updateFromConfig(optHeight, calo.height);
    auto optLength = calTree.get_optional<double>("length");
    updateFromConfig(optLength, calo.length);
  }

  //read generator tree
  auto genTreeOpt = fullTree.get_child_optional("generatorList");
  if(genTreeOpt){
    genVector.resize(0);
    auto genTree = *genTreeOpt;
    
    for(const auto& tree : genTree){
      genVector.push_back(defaultGen);
      auto& thisPrimary = genVector[genVector.size() - 1];
      
      auto thisGenTree = tree.second;
      
      auto optType = thisGenTree.get_optional<std::string>("particleType");
      updateFromConfig(optType, thisPrimary.particleType);
      auto optEnergy = thisGenTree.get_optional<double>("energy");
      updateFromConfig(optEnergy, thisPrimary.energy);
      auto optImpactAngle = thisGenTree.get_optional<double>("impactAngle");
      updateFromConfig(optImpactAngle, thisPrimary.impactAngle);
      auto optPPhiHat = thisGenTree.get_optional<double>("impactPhi");
      updateFromConfig(optPPhiHat, thisPrimary.impactPhi);

      //impact position needs to be treated specially because it is an array (x, y)
      auto impactPositionOpt = thisGenTree.get_child_optional("impactPosition");
      if(impactPositionOpt){
	auto impactPosition = *impactPositionOpt;
	int component = 0;
	for(auto& componentTree : impactPosition){
	  if(component == 0){
	    thisPrimary.posX = componentTree.second.get<double>("");
	  }
	  else if(component == 1){
	    thisPrimary.posY = componentTree.second.get<double>("");
	  }
	  else{
	    break;
	  }
	  ++component;
	}	
      }
    }
  }

  //read physics tree
  auto physTreeOpt = fullTree.get_child_optional("physics");
  if(physTreeOpt){
    auto physTree = *physTreeOpt;

    auto optEmExtra = physTree.get_optional<bool>("emExtra");
    updateFromConfig(optEmExtra, phys.emExtra);
  }
}
