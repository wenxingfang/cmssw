#include "SimG4Core/Geometry/interface/DDDWorld.h"
#include "SimG4Core/Geometry/interface/DDG4Builder.h"

#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/DDCMS/interface/DDDetector.h"
#include "DDG4/Geant4Converter.h"
#include "DDG4/Geant4GeometryInfo.h"
#include "DDG4/Geant4Mapping.h"
#include "DD4hep/Detector.h"
#include "DD4hep/Printout.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4PVPlacement.hh"

using namespace edm;
using namespace dd4hep;
using namespace dd4hep::sim;

DDDWorld::DDDWorld(const DDCompactView *cpv,
                   G4LogicalVolumeToDDLogicalPartMap &lvmap,
                   SensitiveDetectorCatalog &catalog,
                   bool check) {
  LogVerbatim("SimG4CoreApplication") << "DDDWorld: initialization of Geant4 geometry";
  std::unique_ptr<DDG4Builder> theBuilder(new DDG4Builder(cpv, lvmap, check));

  G4LogicalVolume *world = theBuilder->BuildGeometry(catalog);

  m_world = new G4PVPlacement(nullptr, G4ThreeVector(), world, "DDDWorld", nullptr, false, 0);
  LogVerbatim("SimG4CoreApplication") << "DDDWorld: initialization of Geant4 geometry is done.";
}

DDDWorld::DDDWorld(const cms::DDDetector *ddd, dd4hep::sim::Geant4GeometryMaps::VolumeMap &map) {
  LogVerbatim("SimG4CoreApplication") << "DD4hep_DDDWorld: initialization of Geant4 geometry";

  DetElement world = ddd->description()->world();
  const Detector &detector = *ddd->description();
  Geant4Converter g4Geo(detector);
  Geant4GeometryInfo *geometry = g4Geo.create(world).detach();
  map = geometry->g4Volumes;
  m_world = geometry->world();

  LogVerbatim("SimG4CoreApplication") << "DD4hep_DDDWorld: initialization of Geant4 geometry is done.";
}

DDDWorld::~DDDWorld() {}
