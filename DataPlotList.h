#ifndef _DATA_PLOT_LIST_H
#define _DATA_PLOT_LIST_H

#include "DataPlot.h"
#include <list>

class DataPlotList
{
public:

	/// <summary>
	/// Empty Constructor
	/// </summary>
	DataPlotList();

	~DataPlotList();

	/// <summary>
	/// Add plot to DataPlotList
	/// </summary>
	void AddPlot(DataPlot* m_pLastDataPlot);

	/// <summary>
	/// Get m_dataPlotList
	/// </summary>
	std::list<DataPlot*>* getDataPlotList();

	/// <summary>
	/// clear m_dataPlotList
	/// </summary>
	void clear();

private:

	std::list<DataPlot*> m_dataPlotList;

};

#endif // _DATA_PLOT_LIST_H

