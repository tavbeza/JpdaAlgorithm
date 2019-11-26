#ifndef DATAPLOTFILEREADER_H
#define DATAPLOTFILEREADER_H

#include <iostream>
#include <fstream>
//#include "DataTypes\DataPlotList.h"
#include "DataPlotList.h"

using namespace std;

class DataPlotFileReader
{
public:
	/// <summary>
	/// Constructor for data plot reader
	/// </summary>
	DataPlotFileReader(string fileName);

	/// <summary>
	/// Destructor for data plot reader
	/// </summary>
	~DataPlotFileReader();

	/// <summary>
	/// fills a given plotArr with plots read from a file located at "fileName"
	/// </summary>
	void ReadDataPlot(DataPlotList* plotArr, int dwellToRead = 0);

	/// <summary>
	/// get m_fileStream
	/// </summary>
	int readNumberOfDwell();

private:

	string		m_fileName;
	ifstream	m_fileStream;
	bool		m_firstPlotInDwell;
	DataPlot*	m_pLastDataPlot;
	int			m_lastDwell;

};
#endif //DATAPLOTFILEREADER_H