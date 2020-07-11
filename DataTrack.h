#ifndef _DATA_TRACK_H_
#define _DATA_TRACK_H_

#include "Vector.h"
#include "ExtendedKalman.h"
#include "DataPlot.h"

/// <summary>
/// This class encpsulate the Track entity. 
/// </summary>
class DataTrack
{
public:

	/// <summary>
	/// The time the track is created
	/// UNIX time stamp in milliseconds
	/// </summary>
	int m_TimeCreated;

	/// <summary>
	/// The time of the track last update
	/// UNIX time stamp in milliseconds
	/// </summary>
	int m_TimeUpdated;

	/// <summary>
	/// Initiate the track m_AssocValid in each Track to
	/// false-Not valid for association
	/// true-Valid for association
	/// </summary>
	int m_AssocValid;

	//int m_LastPlots[MAX_PLOT_HISTORY]
	int m_nUpdates;

	/// <summary>
	/// 0 - Tentative
	/// 1 - System Track
	/// </summary>
	int m_Type;

	/// <summary>
	/// Contains the track priority
	/// </summary>
	double m_Priority;

	//int m_Dwell_IDs[MAX_DWELL_HISTORY];
	int m_nDwells;
	Vector3d m_PosEnulast;
	int m_MeasAssProbHist;
	int m_MeasAssProbLast;

	/// <summary>
	/// Track constractor.
	/// </summary>
	DataTrack(int id);

	/// <summary>
	/// Track constractor.
	/// </summary>
	void InitTrack(const DataPlot &pDataPlot);

	/// <summary>
	/// Tracl constractor.
	/// </summary>
	~DataTrack();

// TODO: private
public:

	int m_Id;
	int m_Color;
	ExtendedKalman* m_pKalman;
};

#endif //_DATA_TRACK_H_