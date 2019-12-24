#include "DataPlotList.h"

/// <summary>
/// Empty Constructor
/// </summary>
DataPlotList::DataPlotList()
{
	m_nPlots = 0;
}

/// <summary>
/// Destructor
/// </summary>
DataPlotList::~DataPlotList()
{
}

/// <summary>
/// Add plot to DataPlotList
/// </summary>
void DataPlotList::AddPlot(DataPlot* pDataPlot)
{	
	m_dataPlotList[m_nPlots] = pDataPlot;
	m_nPlots++;
}

/// <summary>
/// Get m_dataPlotList
/// </summary>
DataPlot* DataPlotList::getDataPlotList()
{
	return m_dataPlotList[0];
}

/// <summary>
/// clear m_dataPlotList
/// </summary>
void DataPlotList::Clear()
{
	m_nPlots = 0;
}

int DataPlotList::GetCount() const
{
	return m_nPlots;
}

DataPlot* DataPlotList::operator[](const int index) const
{
	if (index > m_nPlots)
		return 0;
	
	return m_dataPlotList[index];
}
