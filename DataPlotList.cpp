#include "DataPlotList.h"

/// <summary>
/// Empty Constructor
/// </summary>
DataPlotList::DataPlotList()
{
}


DataPlotList::~DataPlotList()
{
}

/// <summary>
/// Add plot to DataPlotList
/// </summary>
void DataPlotList::AddPlot(DataPlot* m_pLastDataPlot)
{	
	m_dataPlotList.push_back(m_pLastDataPlot);
}

/// <summary>
/// Get m_dataPlotList
/// </summary>
std::list<DataPlot*> DataPlotList::getDataPlotList()
{
	return m_dataPlotList;
}
