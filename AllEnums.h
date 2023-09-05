#pragma once
/// <summary>
/// This file contains all enum classes for the whole project
/// </summary>

enum class DataType
{
    Well,   //Well trajectory
    WellLog, //1D well log
    DownholeRcvSingleSrc, // ZVSP and Offset surveys - Data has limited range of Receiver and one Source 
    DownholeRcvMultiSrc,  // Walkaway, 3D VSP - Source has a broad spatial range
    SurfaceRcvMultiSrc,  //Surface gathers with range of sources and receivers
    Migrated, //Data with uniform spacing traces and ONLY ONE location per trace
    AcousticFiberWithHdrs, //Acoustic data from a fiber, with source and receiver in the SegY headers
    AcousticFiberWithoutHdrs, // Acoustic data from a fiber, without source and receiver in the headers.
                              // Fiber data is actually a 2D well log 
                              // The receiver location is taken from the well or in case of surface fiber an independent ASCII file can be used
    PassiveMonitoring,
    GeoPhysicalProperty,
};
