#ifndef INDI_ALIGNMENTSUBSYSTEM_DUMMYMATHPLUGIN_H
#define INDI_ALIGNMENTSUBSYSTEM_DUMMYMATHPLUGIN_H

#include "AlignmentSubsystemForMathPlugins.h"
#include "ConvexHull.h"

namespace INDI {
namespace AlignmentSubsystem {

class DummyMathPlugin : public AlignmentSubsystemForMathPlugins
{
    public:
        DummyMathPlugin();
        virtual ~DummyMathPlugin();

    virtual bool Initialise();

    virtual bool TransformCelestialToTelescope(const double RightAscension, const double Declination, TelescopeDirectionVector& TelescopeDirectionVector);

    virtual bool TransformTelescopeToCelestial(const TelescopeDirectionVector& TelescopeDirectionVector, double& RightAscension, double& Declination);
};

} // namespace AlignmentSubsystem
} // namespace INDI

#endif // INDI_ALIGNMENTSUBSYSTEM_DUMMYMATHPLUGIN_H
