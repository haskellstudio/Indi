/*!
 * \file AlignmentSubsystemForDrivers.h
 *
 * \author Roger James
 * \date 13th November 2013
 *
 * This file provides a shorthand way for drivers to include all the
 * functionality they need to use the INDI Alignment Subsystem
 * Clients should inherit this class alongside INDI::Telescope or a similar class
 */

#ifndef INDI_ALIGNMENTSUBSYSTEM_ALIGNMENTSUBSYSTEMFORDRIVERS_H
#define INDI_ALIGNMENTSUBSYSTEM_ALIGNMENTSUBSYSTEMFORDRIVERS_H

#include "MapPropertiesToInMemoryDatabase.h"
#include "MathPluginManagement.h"
#include "TelescopeDirectionVectorSupportFunctions.h"

namespace INDI {
namespace AlignmentSubsystem {

/*!
 * \class AlignmentSubsystemForDrivers
 * \brief This class encapsulates all the alignment subsystem classes that are useful to driver implementations.
 * Drivers should inherit from this class.
 */
class AlignmentSubsystemForDrivers : public MapPropertiesToInMemoryDatabase, public MathPluginManagement,
                                    public TelescopeDirectionVectorSupportFunctions
{
public:
    /// \brief Default constructor
    AlignmentSubsystemForDrivers();
    /// \brief Virtual destructor
    virtual ~AlignmentSubsystemForDrivers() {}

    /** \brief Initilize alignment subsystem properties. It is recommended to call this function within initProperties() of your primary device
     * \param[in] pTelescope Pointer to the child INDI::Telecope class
    */
    void InitProperties(Telescope* pTelescope);

    /** \brief Call this function from within the ISNewBlob processing path. The function will
     * handle any alignment subsystem related properties.
     * \param[in] pTelescope Pointer to the child INDI::Telecope class
     * \param[in] name vector property name
     * \param[in] sizes
     * \param[in] blobsizes
     * \param[in] blobs
     * \param[in] formats
     * \param[in] names
     * \param[in] n
    */
    void ProcessBlobProperties(Telescope* pTelescope, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n);

    /** \brief Call this function from within the ISNewNumber processing path. The function will
     * handle any alignment subsystem related properties.
     * \param[in] pTelescope Pointer to the child INDI::Telecope class
     * \param[in] name vector property name
     * \param[in] values value as passed by the client
     * \param[in] names names as passed by the client
     * \param[in] n number of values and names pair to process.
    */
    void ProcessNumberProperties(Telescope* pTelescope, const char *name, double values[], char *names[], int n);

    /** \brief Call this function from within the ISNewSwitch processing path. The function will
     * handle any alignment subsystem related properties.
     * \param[in] pTelescope Pointer to the child INDI::Telecope class
     * \param[in] name vector property name
     * \param[in] states states as passed by the client
     * \param[in] names names as passed by the client
     * \param[in] n number of values and names pair to process.
    */
    void ProcessSwitchProperties(Telescope* pTelescope, const char *name, ISState *states, char *names[], int n);

    /** \brief Call this function from within the ISNewText processing path. The function will
     * handle any alignment subsystem related properties. This only text property at the moment is contained in the
     * config file so this will normally only have work to do when the config file is loaded.
     * \param[in] pTelescope Pointer to the child INDI::Telecope class
     * \param[in] name vector property name
     * \param[in] texts texts as passed by the client
     * \param[in] names names as passed by the client
     * \param[in] n number of values and names pair to process.
    */
    void ProcessTextProperties(Telescope* pTelescope, const char *name, char *texts[], char *names[], int n);

    /** \brief Call this function to save persistent alignment related properties.
     * This function should be called from within the saveConfigItems function of your driver.
     * \param[in] fp File pointer passed into saveConfigItems
    */
    void SaveConfigProperties(FILE *fp);

private:
    /** \brief This static function is registered as a load database callback with
     * the in memory database module. This registration is performed in the constructor of
     * of this class. The callback is called whenever the database is
     * is loaded or reloaded, by default it calls the Initialise function of the MathPluginManagment module.
     * \param[in] ThisPointer Pointer to the instance of this class which registered the callbck
    */
    static void MyDatabaseLoadCallback(void *ThisPointer);
};

} // namespace AlignmentSubsystem
} // namespace INDI

#endif // INDI_ALIGNMENTSUBSYSTEM_ALIGNMENTSUBSYSTEMFORDRIVERS_H
