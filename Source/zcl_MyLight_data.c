/**************************************************************************************************
  Filename:       zcl_MyLight_data.c
  Revised:        $Date: 2014-05-12 13:14:02 -0700 (Mon, 12 May 2014) $
  Revision:       $Revision: 38502 $


  Description:    Zigbee Cluster Library - sample device application.


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"
#include "AF.h"
#include "ZDConfig.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"

/* MYLIGHT_TODO: Include any of the header files below to access specific cluster data
#include "zcl_poll_control.h"
#include "zcl_electrical_measurement.h"
#include "zcl_diagnostic.h"
#include "zcl_meter_identification.h"
#include "zcl_appliance_identification.h"
#include "zcl_appliance_events_alerts.h"
#include "zcl_power_profile.h"
#include "zcl_appliance_control.h"
#include "zcl_appliance_statistics.h"
#include "zcl_hvac.h"
*/

#include "zcl_MyLight.h"

/*********************************************************************
 * CONSTANTS
 */

#define MYLIGHT_DEVICE_VERSION     1
#define MYLIGHT_FLAGS              0

#define MYLIGHT_HWVERSION          1
#define MYLIGHT_ZCLVERSION         1

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
int16 zclMyLight_MeasuredValue;
uint8 ledState = 0;
// Global attributes
const uint16 zclMyLight_clusterRevision_all = 0x0001; 

// Basic Cluster
const uint8 zclMyLight_HWRevision = MYLIGHT_HWVERSION;
const uint8 zclMyLight_ZCLVersion = MYLIGHT_ZCLVERSION;
const uint8 zclMyLight_ManufacturerName[] = { 4, 'Q', 'u', 'o', 'c' };
const uint8 zclMyLight_ModelId[] = { 7, 'Q', 'u', 'o', 'c', '_', 'S', 'W'};
const uint8 zclMyLight_DateCode[] = { 8, '2','0','2','1','1','2','1','9' };
const uint8 zclMyLight_PowerSource = POWER_SOURCE_MAINS_1_PHASE;

uint8 zclMyLight_LocationDescription[17] = { 16, ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
uint8 zclMyLight_PhysicalEnvironment = 0;
uint8 zclMyLight_DeviceEnable = DEVICE_ENABLED;

// Identify Cluster
uint16 zclMyLight_IdentifyTime;

/* MYLIGHT_TODO: declare attribute variables here. If its value can change,
 * initialize it in zclMyLight_ResetAttributesToDefaultValues. If its
 * value will not change, initialize it here.
 */

#if ZCL_DISCOVER
CONST zclCommandRec_t zclMyLight_Cmds[] =
{
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    COMMAND_BASIC_RESET_FACT_DEFAULT,
    CMD_DIR_SERVER_RECEIVED
  },

};

CONST uint8 zclCmdsArraySize = ( sizeof(zclMyLight_Cmds) / sizeof(zclMyLight_Cmds[0]) );
#endif // ZCL_DISCOVER

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */
CONST zclAttrRec_t zclMyLight_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GEN_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&zclMyLight_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclMyLight_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclMyLight_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_MODEL_ID,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclMyLight_ModelId
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DATE_CODE,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclMyLight_DateCode
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclMyLight_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_LOCATION_DESC,
      ZCL_DATATYPE_CHAR_STR,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)zclMyLight_LocationDescription
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENV,
      ZCL_DATATYPE_ENUM8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclMyLight_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    { // Attribute record
      ATTRID_BASIC_DEVICE_ENABLED,
      ZCL_DATATYPE_BOOLEAN,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclMyLight_DeviceEnable
    }
  },

#ifdef ZCL_IDENTIFY
  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclMyLight_IdentifyTime
    }
  },
#endif
  {
    ZCL_CLUSTER_ID_GEN_BASIC,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclMyLight_clusterRevision_all
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_IDENTIFY,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclMyLight_clusterRevision_all
    }
  },
   {
    ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
    { // ???????? ???????????
      ATTRID_DEV_TEMP_CURRENT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ | ACCESS_REPORTABLE,
      (void *)&zclMyLight_MeasuredValue
    }
  },
  {
    ZCL_CLUSTER_ID_GEN_ON_OFF,
    { // ?????????
      ATTRID_ON_OFF,
      ZCL_DATATYPE_BOOLEAN,
      ACCESS_CONTROL_READ,
      (void *)&ledState
    }
  },
};

uint8 CONST zclMyLight_NumAttributes = ( sizeof(zclMyLight_Attrs) / sizeof(zclMyLight_Attrs[0]) );

/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t zclMyLight_InClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_IDENTIFY,
  
  // MYLIGHT_TODO: Add application specific Input Clusters Here. 
  //       See zcl.h for Cluster ID definitions
  
};
#define ZCLMYLIGHT_MAX_INCLUSTERS   (sizeof(zclMyLight_InClusterList) / sizeof(zclMyLight_InClusterList[0]))


const cId_t zclMyLight_OutClusterList[] =
{
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_ON_OFF
  // MYLIGHT_TODO: Add application specific Output Clusters Here. 
  //       See zcl.h for Cluster ID definitions
};
#define ZCLMYLIGHT_MAX_OUTCLUSTERS  (sizeof(zclMyLight_OutClusterList) / sizeof(zclMyLight_OutClusterList[0]))


SimpleDescriptionFormat_t zclMyLight_SimpleDesc =
{
  MYLIGHT_ENDPOINT,                  //  int Endpoint;
  ZCL_HA_PROFILE_ID,                     //  uint16 AppProfId;
  // MYLIGHT_TODO: Replace ZCL_HA_DEVICEID_ON_OFF_LIGHT with application specific device ID
  ZCL_HA_DEVICEID_SIMPLE_SENSOR,          //  uint16 AppDeviceId; 
  MYLIGHT_DEVICE_VERSION,            //  int   AppDevVer:4;
  MYLIGHT_FLAGS,                     //  int   AppFlags:4;
  ZCLMYLIGHT_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclMyLight_InClusterList, //  byte *pAppInClusterList;
  ZCLMYLIGHT_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)zclMyLight_OutClusterList //  byte *pAppInClusterList;
};

// Added to include ZLL Target functionality
#if defined ( BDB_TL_INITIATOR ) || defined ( BDB_TL_TARGET )
bdbTLDeviceInfo_t tlMyLight_DeviceInfo =
{
  MYLIGHT_ENDPOINT,                  //uint8 endpoint;
  ZCL_HA_PROFILE_ID,                        //uint16 profileID;
  // MYLIGHT_TODO: Replace ZCL_HA_DEVICEID_ON_OFF_LIGHT with application specific device ID
  ZCL_HA_DEVICEID_SIMPLE_SENSOR,          //uint16 deviceID;
  MYLIGHT_DEVICE_VERSION,                    //uint8 version;
  MYLIGHT_NUM_GRPS                   //uint8 grpIdCnt;
};
#endif

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */
  
void zclMyLight_ResetAttributesToDefaultValues(void)
{
  int i;
  
  zclMyLight_LocationDescription[0] = 16;
  for (i = 1; i <= 16; i++)
  {
    zclMyLight_LocationDescription[i] = ' ';
  }
  
  zclMyLight_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;
  zclMyLight_DeviceEnable = DEVICE_ENABLED;
  
#ifdef ZCL_IDENTIFY
  zclMyLight_IdentifyTime = 0;
#endif
  
  /* MYLIGHT_TODO: initialize cluster attribute variables. */
}

/****************************************************************************
****************************************************************************/


