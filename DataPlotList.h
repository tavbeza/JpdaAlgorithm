#ifndef _DATA_PLOT_LIST_H
#define _DATA_PLOT_LIST_H

#include "DataPlot.h"
#include <list>

#define MAX_PLOTS 500

/// <summary>
/// Class with a list of data plot that obtained from the sensor
/// </summary>
class DataPlotList
{
public:

	/// <summary>
	/// Empty Constructor
	/// </summary>
	DataPlotList();

	/// <summary>
	/// Destructor
	/// </summary>
	~DataPlotList();

	/// <summary>
	/// Add plot to DataPlotList
	/// </summary>
	void AddPlot(DataPlot* m_pLastDataPlot);

	/// <summary>
	/// Get m_dataPlotList
	/// </summary>
	DataPlot* getDataPlotList();

	/// <summary>
	/// clear m_dataPlotList
	/// </summary>
	void Clear();

	int GetCount() const;

	DataPlot* operator[](const int index) const;

private:

	DataPlot* m_dataPlotList[MAX_PLOTS];
	int m_nPlots;
};

#endif // _DATA_PLOT_LIST_H

