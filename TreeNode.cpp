#include "pch.h"
#include "TreeNode.h"
#include "stdio.h"
#include<iostream>

using namespace std;

/// <summary>
/// Default Constructor for tree node
/// </summary>
TreeNode::TreeNode()
{
	m_pParent = 0;
	m_nChilds = 0;
}

/// <summary>
/// Copy constructor
/// </summary>
TreeNode::TreeNode(const TreeNode& old_obj) {
	//this->m_pParent = old_obj.m_pParent;
	this->SetParent(old_obj.m_pParent);
	this->m_nChilds = old_obj.m_nChilds;
	this->m_i = old_obj.m_i;
	this->m_j = old_obj.m_j;
	for (int i = 0; i < m_nChilds; i++) {
		this->m_children[i] = old_obj.m_children[i];
	}
	//m_children = old_obj.m_children;
}

/// <summary>
/// Destructor for tree node
/// </summary>
TreeNode::~TreeNode()
{
}

/// <summary>
/// Sets the node parent
/// </summary>
void TreeNode::SetParent(TreeNode* pParent)
{
	m_pParent = pParent;
}

/// <summary>
/// Get parent
/// </summary>
TreeNode* TreeNode::getParent()
{
	return m_pParent;
}

/// <summary>
/// Get m_i
/// </summary>
int TreeNode::getM_I()
{
	return m_i;
}

/// <summary>
/// Get m_j
/// </summary>
int TreeNode::getM_J()
{
	return m_j;
}

/// <summary>
	/// Get data  // 1 = assosiated with prior target  , 0 = false alarm
	/// </summary>
int TreeNode::getAssociationData()
{
	return m_association_data;
}

/// <summary>
/// Add child to the tree
/// </summary>
void TreeNode::AddChild(TreeNode* pChild)
{
	m_children[m_nChilds] = pChild;
	m_nChilds++;
}

/// <summary>
/// Check if the track is already exist: return true if exist, else return false
/// </summary>
bool TreeNode::hasTrackParent(TreeNode* pParent, int track)
{
	if (pParent->m_j == track)
		return true;
	else
	{
		if (pParent->m_pParent != 0)
			return hasTrackParent(pParent->m_pParent, track);
		else
			return false;
	}
}

/// <summary>
/// Print the tree
/// </summary>
void TreeNode::PrintPathToRoot()
{
	if (m_pParent != 0)
	{
		m_pParent->PrintPathToRoot();
		//cout << "( " << m_i << ", " << m_j << " ) ";
		printf("( %d , %d ) ", m_i, m_j);
	}
	else
	{
		//cout << endl;
		printf("\n");
		printf("( %d , %d ) ", m_i, m_j);
		//cout << " ( " << counter << " )" <<endl;
	}
}