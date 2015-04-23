#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>

typedef struct{
  double delta;
  double x0;
  double y0;
} GeneratorConfiguration;

typedef struct{
  int nPlanes;
  double fieldIndex;
} DetectorConfiguration;

class SimConfiguration{
public:
  SimConfiguration();
  SimConfiguration(std::string);
  
  void update();
  
  GeneratorConfiguration gen;
  DetectorConfiguration det;

  static const GeneratorConfiguration defaultGen;
  static const DetectorConfiguration defaultDet;

private:
  void readConfig();
  std::string confFile_;
};
  

#endif
