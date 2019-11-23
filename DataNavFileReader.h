#ifndef DATANAVFILEREADER_H
#define DATANAVFILEREADER_H

#include <iostream>
#include <fstream>
#include "NavPlatStatusStruct.h"
//#include "InfraLayer/RECORDING/MsgsNAV.h"

//using namespace infra;
//using namespace app;
using namespace std;


class DataNavFileReader
{
public:
	/// <summary>
	/// Constructor for data plot reader
	/// </summary>
	DataNavFileReader(string fileName);

	/// <summary>
	/// Destructor for data plot reader
	/// </summary>
	~DataNavFileReader();

	/// <summary>
	/// fills a given plotArr with plots read from a file located at "fileName"
	/// </summary>
	void ReadDataNav(int dwellToRead = 0);

private:

	string					m_fileName;
	ifstream				m_fileStream;
	bool					m_firstNavInDwell;
	NavPlatStatusStruct*	m_pLastDataNav;
	int						m_lastDwell;



};
#endif //DATANAVFILEREADER_H