#pragma once

//****************************************************************************
//****************************************************************************
//****************************************************************************

#include "ccCallPointer.h"
#include "ccTreeNodeVisitor.h"
#include "ccTreeNode.h"

//****************************************************************************
//****************************************************************************
//****************************************************************************

namespace CC
{
   // This destroys a collection of tree nodes, starting at and including a 
   // subject root node. It recursively traverses the nodes in destruction
   // order, such that child nodes are destroyed before parent nodes. It
   // recursively destroys all of the nodes below the subject root node
   // and then destroys the subject root node. For each node destroyed, 
   // it calls the node destroy function.

   void destroyAllTreeNodes(
      TreeNode*        aSubjectNode);

   // This destroys all child nodes below a subject root node. It is called 
   // by the above recursive destroy function. It recursively traverses 
   // the nodes in destruction order, such that child nodes are 
   // destroyed before parent nodes. For each node destroyed, it calls the
   // node destroy function.

   void destroyAllTreeNodesBelow(
      TreeNode*        aSubjectNode,
      RecursiveAnchor* aRecursiveAnchor);


//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
