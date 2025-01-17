#ifndef TRACKER_TREENODE_H_
#define TRACKER_TREENODE_H_
#include "pch.h"
//#include "TreeData.h"

#define MAX_TREE_ITEMS  (50)


/// <summary>
/// Encapsulate the a tree data structure
/// </summary>
class TreeNode
{
public:
	/// <summary>
	/// Default Constructor for tree node
	/// </summary>
	TreeNode();

	/// <summary>
	/// Copy constractor
	/// </summary>
	TreeNode(const TreeNode& old_obj);

	/// <summary>
	/// Destructor for tree node
	/// </summary>
	virtual ~TreeNode();

	/// <summary>
	/// Sets the node parent
	/// </summary>
	void SetParent(TreeNode* pParent);

	/// <summary>
	/// Add child to the tree
	/// </summary>
	void AddChild(TreeNode* pChild);

	/// <summary>
	/// Print the tree
	/// </summary>
	void PrintPathToRoot();

	/// <summary>
	/// Get parent
	/// </summary>
	TreeNode* getParent();

	/// <summary>
	/// Get m_i
	/// </summary>
	int getM_I();

	/// <summary>
	/// Get m_j
	/// </summary>
	int getM_J();

	/// <summary>
	/// Get association data  // 1 = assosiated with prior target  , 0 = false alarm
	/// </summary>
	int getAssociationData();

	/// <summary>
	/// Check if the track is already exist: return true if exist, else return false
	/// </summary>
	bool hasTrackParent(TreeNode* pParent, int track);

	/// <summary>
	/// Returns the data Item
	/// </summary>
	//MatrixItem *GetData() {return &m_Data;}

	int m_i;
	int m_j;
private:
	//MatrixItem              m_Data;

	//children nodes of this node
	TreeNode* m_children[MAX_TREE_ITEMS];
	int                     m_nChilds;
	TreeNode* m_pParent;
	int m_association_data; // 1 = assosiated with prior target  ,  else not assosiated

};

#endif // TRACKER_TREENODE_H_