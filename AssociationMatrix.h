#ifndef TRACKER_ASSOCIATIONMATRIX_H_
#define TRACKER_ASSOCIATIONMATRIX_H_

/*
#include "DataTypes\DataPlot.h"
include "InfraLayer\General\Matrix.h"
#include "ServiceLayer\SrvNavigation.h"
#include "Track.h"
*/

#include "DataPlot.h"
#include "Matrix.h"
#include "Tracker.h"
#include "NavPlatStatusStruct.h"

#define MAX_NUM_TRACKS      1000
#define MAX_NUM_PLOTS       1000


/// <summary>
/// This class encapsulate the association matrix data structure and contains two dimensional array of tracks and plots
/// It also contains the gain of track and plot associated
/// </summary>
class AssociationMatrix
{
public:
	/// <summary>
	/// Default constructor for association matrix
	/// </summary>
	AssociationMatrix();

	/// <summary>
	/// Destructor for association matrix
	/// </summary>
	virtual ~AssociationMatrix();

	/// <summary>
	/// Returns the global objects
	/// </summary>
	static AssociationMatrix *GetInstance();

	/// <summary>
	/// Clean the plots and tracks
	/// </summary>
	void Clean();

	/// <summary>
	/// Associate track and plot and set the gain
	/// </summary>
	void Associate(unsigned int trkID,
		unsigned int pltID,
		bool assocFlg,
		double g);

	/// <summary>
	/// Check the track association to plot and calculate the gain
	/// </summary>
	void CheckAssociation(DataTrack &track,
		const DataPlot &plot,
		const NavPlatStatusStruct &platData,
		bool &isAsocFlagVec,
		double &g);

public:
	/// <summary>
	/// Indicate whether the track is associated to plot
	/// </summary>
	bool            m_MatTracksPlots[MAX_NUM_TRACKS][MAX_NUM_PLOTS];

	/// <summary>
	/// the gain value of the track and plot associated
	/// </summary>
	double          m_Gain[MAX_NUM_TRACKS][MAX_NUM_PLOTS];

	/// <summary>
	/// Contains the list of tracks IDs
	/// </summary>
	int             m_TrkIds[MAX_NUM_TRACKS];

	/// <summary>
	/// Counts the number of tracks used in the association
	/// </summary>
	unsigned int    m_nTracks;

	/// <summary>
	/// Contains the list of plots IDs
	/// </summary>
	int             m_PlotIds[MAX_NUM_PLOTS];

	/// <summary>
	/// Counts the number of plots used in the association
	/// </summary>
	unsigned int    m_nPlots;

	/// <summary>
	/// Contains the probabilities matrix
	/// </summary>
	double          m_MatTracksPlotsProb[MAX_NUM_TRACKS][MAX_NUM_PLOTS];


private:
	/// <summary>
	/// Set the angle between -pi:+pi
	/// </summary>
	double Mod7(double angle);

};


#endif // TRACKER_ASSOCIATIONMATRIX_H_
